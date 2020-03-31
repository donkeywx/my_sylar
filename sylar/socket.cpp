#include "socket.h"
#include "iomanager.h"
#include "fdmanager.h"
#include "log.h"
#include "macro.h"
#include "hook.h"
#include <limits.h>

namespace sylar
{

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

Socket::Socket(int domain, int type, int protocol)
    :m_sock(-1)
    ,m_domain(domain)
    ,m_type(type)
    ,m_protocol(protocol)
    ,m_isConnected(false)
{
}

Socket::~Socket()
{
    close();
}


bool Socket::init(bool needCreateNewFd)
{
    if (needCreateNewFd)
    {
        m_sock = socket(m_domain, m_type, m_protocol);
        if (-1 == m_sock)
        {
            SYLAR_LOG_ERROR(g_logger) << "socket(" << m_domain
                << ", " << m_type << ", " << m_protocol << ") errno="
                << errno << " errstr=" << strerror(errno);
            
            return false;
        }
    }

    int val = 1;
    setOption(SOL_SOCKET, SO_REUSEADDR, val);
    if(TCP == m_type)
    {
        setOption(IPPROTO_TCP, TCP_NODELAY, val);
    }
    else if (UDP == m_type)
    {
        this->m_isConnected = true;
    }
    
    return true;
}

Socket::ptr Socket::CreateTCPSocket(sylar::Socket::Domain domain)
{
    Socket::ptr sock;
    if (Socket::Domain::IPv4 == domain)
    {
        sock.reset(new Socket(IPv4, TCP, 0));
    }
    else if (Socket::Domain::IPv6 == domain)
    {
        sock.reset(new Socket(IPv6, TCP, 0));
    }
    else
    {
         SYLAR_LOG_ERROR(g_logger) << "failed to create tcp socket, invalid socket domain: "
            << domain;
    }

    return sock;
}

Socket::ptr Socket::CreateUDPSocket(sylar::Socket::Domain domain)
{
    Socket::ptr sock;
    if (Socket::Domain::IPv4 == domain)
    {
        sock.reset(new Socket(IPv4, UDP, 0));
    }
    else if (Socket::Domain::IPv6 == domain)
    {
        sock.reset(new Socket(IPv6, UDP, 0));
    }
    else
    {
         SYLAR_LOG_ERROR(g_logger) << "failed to create udp socket, invalid socket domain: "
            << domain;
    }

    if (nullptr != sock)
    {
        sock->init(true);
    }
    return sock;
}

Socket::ptr Socket::CreateUNixSocket(Socket::Type type)
{
    Socket::ptr sock;
    if (Socket::Type::TCP == type)
    {
        sock.reset(new Socket(UNIX, TCP, 0));
    }
    else if (Socket::Type::UDP == type)
    {
        sock.reset(new Socket(UNIX, UDP, 0));
    }
    else
    {
        SYLAR_LOG_ERROR(g_logger) << "failed to create unix socket, invalid socket type: "
            << type;
    }

    return sock;
}

bool Socket::bind(const Address::ptr addr)
{

    if(SYLAR_UNLIKELY(addr->getFamily() != m_domain))
    {
        SYLAR_LOG_ERROR(g_logger) << "bind sock.family("
            << m_domain << ") addr.family(" << addr->getFamily()
            << ") not equal, addr=" << addr->toString();
        return false;
    }

    if(::bind(m_sock, addr->getAddr(), addr->getAddrLen()))
    {
        SYLAR_LOG_ERROR(g_logger) << "bind error errrno=" << errno
            << " errstr=" << strerror(errno);
        return false;
    }
    getLocalAddress();
    return true;
}

bool Socket::listen(const Address::ptr addr, int backlog) 
{
    if(!isValid())
    {
        this->init(true);
        if (SYLAR_UNLIKELY(!isValid()))
        {
            SYLAR_LOG_ERROR(g_logger) << "listen error sock=-1";
            return false;
        }
    }

    if (!bind(addr))
    {
        return false;
    }

    if(::listen(m_sock, backlog))
    {
        SYLAR_LOG_ERROR(g_logger) << "listen error errno=" << errno
            << " errstr=" << strerror(errno);
        return false;
    }
    return true;
}

Socket::ptr Socket::accept()
{
    int newsock = ::accept(m_sock, nullptr, nullptr);
    if(-1 == newsock)
    {
        SYLAR_LOG_ERROR(g_logger) << "accept(" << m_sock << ") errno="
            << errno << " errstr=" << strerror(errno);
        return nullptr;
    }
    
    return this->createAcceptedSocket(newsock);
}

Socket::ptr Socket::createAcceptedSocket(int accpetedSocket)
{
    Socket::ptr sock;
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(accpetedSocket);
    if(ctx && ctx->isSocket() && !ctx->isClose())
    {
        sock.reset(new Socket(m_domain, m_type, m_protocol));
        sock->m_sock = accpetedSocket;
        sock->init(false);
        sock->m_isConnected = true;
        sock->getLocalAddress();
        sock->getRemoteAddress();
    }
    return sock;
}

bool Socket::connect(const Address::ptr addr, uint64_t timeout_ms)
{
    m_remoteAddress = addr;

    if(SYLAR_UNLIKELY(addr->getFamily() != m_domain))
    {
        SYLAR_LOG_ERROR(g_logger) << "connect sock.family("
            << m_domain << ") addr.family(" << addr->getFamily()
            << ") not equal, addr=" << addr->toString();
        return false;
    }

    if(!isValid())
    {
        this->init(true);
        if(SYLAR_UNLIKELY(!isValid()))
        {
            return false;
        }
    }

    if((uint64_t)-1 == timeout_ms) 
    {
        if(::connect(m_sock, addr->getAddr(), addr->getAddrLen()))
        {
            SYLAR_LOG_ERROR(g_logger) << "sock=" << m_sock 
                << " connect("  << addr->toString()
                << ") error errno=" << errno 
                << " errstr=" << strerror(errno);
            close();
            return false;
        }
    }
    else
    {
        if(::connect_with_timeout(m_sock, addr->getAddr(), addr->getAddrLen(), timeout_ms))
        {
            SYLAR_LOG_ERROR(g_logger) << "sock=" << m_sock 
                << " connect(" << addr->toString()
                << ") timeout=" << timeout_ms << " error errno="
                << errno << " errstr=" << strerror(errno);
            close();
            return false;
        }
    }
    m_isConnected = true;
    getRemoteAddress();
    getLocalAddress();
    return true;
}

bool Socket::close()
{
    if(!m_isConnected && m_sock == -1)
    {
        return true;
    }
    m_isConnected = false;
    if(m_sock != -1)
    {
        ::close(m_sock);
        m_sock = -1;
    }
    return false;
}


int Socket::send(const void* buffer, size_t length, int flags)
{
    if(isConnected())
    {
        return ::send(m_sock, buffer, length, flags);
    }
    return -1;
}

int Socket::send(const iovec* buffers, size_t length, int flags)
{
    if(isConnected())
    {
        msghdr msg;
        memset(&msg, 0, sizeof(msg));
        msg.msg_iov = (iovec*)buffers;
        msg.msg_iovlen = length;
        return ::sendmsg(m_sock, &msg, flags);
    }
    return -1;
}

int Socket::sendTo(const void* buffer, size_t length, const Address::ptr to, int flags)
{
    if(isConnected())
    {
        return ::sendto(m_sock, buffer, length, flags, to->getAddr(), to->getAddrLen());
    }
    return -1;
}

int Socket::sendTo(const iovec* buffers, size_t length, const Address::ptr to, int flags)
{
    if(isConnected())
    {
        msghdr msg;
        memset(&msg, 0, sizeof(msg));
        msg.msg_iov = (iovec*)buffers;
        msg.msg_iovlen = length;
        msg.msg_name = to->getAddr();
        msg.msg_namelen = to->getAddrLen();
        return ::sendmsg(m_sock, &msg, flags);
    }
    return -1;
}

int Socket::recv(void* buffer, size_t length, int flags)
{
    if(isConnected())
    {
        return ::recv(m_sock, buffer, length, flags);
    }
    return -1;
}

int Socket::recv(iovec* buffers, size_t length, int flags)
{
    if(isConnected())
    {
        msghdr msg;
        memset(&msg, 0, sizeof(msg));
        msg.msg_iov = (iovec*)buffers;
        msg.msg_iovlen = length;
        return ::recvmsg(m_sock, &msg, flags);
    }
    return -1;
}

int Socket::recvFrom(void* buffer, size_t length, Address::ptr from, int flags)
{
    if(isConnected())
    {
        socklen_t len = from->getAddrLen();
        return ::recvfrom(m_sock, buffer, length, flags, from->getAddr(), &len);
    }
    return -1;
}

int Socket::recvFrom(iovec* buffers, size_t length, Address::ptr from, int flags)
{
    if(isConnected())
    {
        msghdr msg;
        memset(&msg, 0, sizeof(msg));
        msg.msg_iov = (iovec*)buffers;
        msg.msg_iovlen = length;
        msg.msg_name = from->getAddr();
        msg.msg_namelen = from->getAddrLen();
        return ::recvmsg(m_sock, &msg, flags);
    }
    return -1;
}

bool Socket::cancelRead()
{
    return IOManager::GetThis()->cancelEvent(m_sock, sylar::IOManager::READ);
}

bool Socket::cancelWrite()
{
    return IOManager::GetThis()->cancelEvent(m_sock, sylar::IOManager::WRITE);
}

bool Socket::cancelAccept()
{
    return IOManager::GetThis()->cancelEvent(m_sock, sylar::IOManager::READ);
}

bool Socket::cancelAll()
{
    return IOManager::GetThis()->cancelAll(m_sock);
}

int64_t Socket::getSendTimeout()
{
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_sock);
    if(ctx)
    {
        return ctx->getTimeout(SO_SNDTIMEO);
    }
    return -1;
}

void Socket::setSendTimeout(int64_t v)
{
    struct timeval tv{int(v / 1000), int(v % 1000 * 1000)};
    setOption(SOL_SOCKET, SO_SNDTIMEO, tv);
}

int64_t Socket::getRecvTimeout()
{
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_sock);
    if(ctx)
    {
        return ctx->getTimeout(SO_RCVTIMEO);
    }
    return -1;
}

void Socket::setRecvTimeout(int64_t v)
{
    struct timeval tv{int(v / 1000), int(v % 1000 * 1000)};
    setOption(SOL_SOCKET, SO_RCVTIMEO, tv);
}

bool Socket::getOption(int level, int option, void* result, socklen_t* len)
{
    int rt = getsockopt(m_sock, level, option, result, (socklen_t*)len);
    if(rt)
    {
        SYLAR_LOG_DEBUG(g_logger) << "getOption sock=" << m_sock
            << " level=" << level << " option=" << option
            << " errno=" << errno << " errstr=" << strerror(errno);
        return false;
    }
    return true;
}

bool Socket::setOption(int level, int option, const void* result, socklen_t len)
{
    if(setsockopt(m_sock, level, option, result, (socklen_t)len))
    {
        SYLAR_LOG_DEBUG(g_logger) << "setOption sock=" << m_sock
            << " level=" << level << " option=" << option
            << " errno=" << errno << " errstr=" << strerror(errno);
        return false;
    }
    return true;
}

Address::ptr Socket::getRemoteAddress()
{
    if(m_remoteAddress)
    {
        return m_remoteAddress;
    }

    Address::ptr result;
    switch(m_domain)
    {
        case AF_INET:
            result.reset(new IPv4Address());
            break;
        case AF_INET6:
            result.reset(new IPv6Address());
            break;
        case AF_UNIX:
            result.reset(new UnixAddress());
            break;
        default:
            result.reset(new UnknownAddress(m_domain));
            break;
    }
    socklen_t addrlen = result->getAddrLen();
    if(getpeername(m_sock, result->getAddr(), &addrlen))
    {
        //SYLAR_LOG_ERROR(g_logger) << "getpeername error sock=" << m_sock
        //    << " errno=" << errno << " errstr=" << strerror(errno);
        return Address::ptr(new UnknownAddress(m_domain));
    }
    if(m_domain == AF_UNIX)
    {
        UnixAddress::ptr addr = std::dynamic_pointer_cast<UnixAddress>(result);
        addr->setAddrLen(addrlen);
    }
    m_remoteAddress = result;
    return m_remoteAddress;
}

Address::ptr Socket::getLocalAddress()
{
    if(m_localAddress)
    {
        return m_localAddress;
    }

    Address::ptr result;
    switch(m_domain)
    {
        case AF_INET:
            result.reset(new IPv4Address());
            break;
        case AF_INET6:
            result.reset(new IPv6Address());
            break;
        case AF_UNIX:
            result.reset(new UnixAddress());
            break;
        default:
            result.reset(new UnknownAddress(m_domain));
            break;
    }
    socklen_t addrlen = result->getAddrLen();
    if(getsockname(m_sock, result->getAddr(), &addrlen))
    {
        SYLAR_LOG_ERROR(g_logger) << "getsockname error sock=" << m_sock
            << " errno=" << errno << " errstr=" << strerror(errno);
        return Address::ptr(new UnknownAddress(m_domain));
    }
    if(m_domain == AF_UNIX)
    {
        UnixAddress::ptr addr = std::dynamic_pointer_cast<UnixAddress>(result);
        addr->setAddrLen(addrlen);
    }
    m_localAddress = result;
    return m_localAddress;
}

bool Socket::isValid() const
{
    return m_sock != -1;
}

int Socket::getError()
{
    int error = 0;
    socklen_t len = sizeof(error);
    if(!getOption(SOL_SOCKET, SO_ERROR, &error, &len))
    {
        error = errno;
    }
    return error;
}

std::ostream& Socket::dump(std::ostream& os) const
{
    os << "[Socket sock=" << m_sock
       << " is_connected=" << m_isConnected
       << " family=" << m_domain
       << " type=" << m_type
       << " protocol=" << m_protocol;
    if(m_localAddress)
    {
        os << " local_address=" << m_localAddress->toString();
    }
    if(m_remoteAddress)
    {
        os << " remote_address=" << m_remoteAddress->toString();
    }
    os << "]";
    return os;
}

std::string Socket::toString() const
{
    std::stringstream ss;
    dump(ss);
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Socket& sock)
{
    return sock.dump(os);
}
}
