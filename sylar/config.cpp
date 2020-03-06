#include "config.h"
#include <iostream>

namespace sylar
{
static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");


ConfigVarBase::ptr Config::LookupBase(const std::string& name) {

    auto it = GetDatas().find(name);
    return it == GetDatas().end() ? nullptr : it->second;
}


static void ListAllMember(const std::string& prefix,
                          const YAML::Node& node,
                          std::list<std::pair<std::string, const YAML::Node> >& output) 
{
    if(prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678")
            != std::string::npos) 
    {
        SYLAR_LOG_ERROR(g_logger) << "Config invalid name: " << prefix << " : " << node;
        return;
    }
    output.push_back(std::make_pair(prefix, node));
    if(node.IsMap()) 
    {
        for(auto it = node.begin();
                it != node.end(); ++it) 
        {
            ListAllMember(prefix.empty() ? it->first.Scalar()
                    : prefix + "." + it->first.Scalar(), it->second, output);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node& root) 
{
    std::list<std::pair<std::string, const YAML::Node> > all_nodes;
    ListAllMember("", root, all_nodes);

    for(auto& i : all_nodes) 
    {
        std::string key = i.first;
        // std::cout << i.first << std::endl;
        // SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << i.first;
        if(key.empty()) 
        {
            continue;
        }

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        ConfigVarBase::ptr var = LookupBase(key);

        if(var) 
        {
            if(i.second.IsScalar()) 
            {
                var->fromString(i.second.Scalar());

                // std::cout << i.first << " " << i.second.Scalar() << std::endl;
            }
            else 
            {
                std::stringstream ss;
                ss << i.second;
                // std::cout << ss.str() << std::endl;
                var->fromString(ss.str());
            }
        }
    }
}

// static std::map<std::string, uint64_t> s_file2modifytime;
// static sylar::Mutex s_mutex;

void Config::LoadFromConfDir(const std::string& path, bool force) 
{
    // std::string absoulte_path = sylar::EnvMgr::GetInstance()->getAbsolutePath(path);
    // std::vector<std::string> files;
    // FSUtil::ListAllFile(files, absoulte_path, ".yml");

    // for(auto& i : files) {
    //     {
    //         struct stat st;
    //         lstat(i.c_str(), &st);
    //         // sylar::Mutex::Lock lock(s_mutex);
    //         if(!force && s_file2modifytime[i] == (uint64_t)st.st_mtime) {
    //             continue;
    //         }
    //         s_file2modifytime[i] = st.st_mtime;
    //     }
    //     try {
    //         YAML::Node root = YAML::LoadFile(i);
    //         LoadFromYaml(root);
    //         SYLAR_LOG_INFO(g_logger) << "LoadConfFile file="
    //             << i << " ok";
    //     } catch (...) {
    //         SYLAR_LOG_ERROR(g_logger) << "LoadConfFile file="
    //             << i << " failed";
    //     }
    // }
}

void Config::Visit(std::function<void(ConfigVarBase::ptr)> cb) 
{

    ConfigVarMap& m = GetDatas();
    for(auto it = m.begin(); it != m.end(); ++it) 
    {
        cb(it->second);
    }

}

Config::ConfigVarMap& Config::GetDatas() 
{
    static ConfigVarMap s_datas;
    return s_datas;
}
}