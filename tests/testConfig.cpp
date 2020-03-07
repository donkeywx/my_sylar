#include "../sylar/config.h"
#include "../sylar/log.h"

static sylar::ConfigVar<int>::ptr g_int = 
    sylar::Config::Lookup("test.g_int", (int)8080, "test.g_int");
static sylar::ConfigVar<float>::ptr g_float = 
    sylar::Config::Lookup("test.g_float", (float)3.4, "test.g_float");
static sylar::ConfigVar<std::vector<int> >::ptr g_vec =
    sylar::Config::Lookup("test.g_vec", std::vector<int>{1,2}, "test.g_vec");
static sylar::ConfigVar<std::list<int> >::ptr g_list =
    sylar::Config::Lookup("test.g_list", std::list<int>{1,2}, "test.g_list");
static sylar::ConfigVar<std::set<int> >::ptr g_set =
    sylar::Config::Lookup("test.g_set", std::set<int>{3, 1}, "test.g_set");
static sylar::ConfigVar<std::unordered_set<int> >::ptr g_uset = 
    sylar::Config::Lookup("test.g_uset", std::unordered_set<int>{1,3,1}, "test.g_uset");
static sylar::ConfigVar<std::map<std::string, int> >::ptr g_map =
    sylar::Config::Lookup("test.g_map", std::map<std::string, int>{{"name",1},{"sex",2}}, "test.g_map");
static sylar::ConfigVar<std::unordered_map<std::string, int> >::ptr g_umap = 
    sylar::Config::Lookup("test.g_umap", std::unordered_map<std::string, int>{{"name", 1}, {"sex", 2}}, "test.g_umap");


void print_yaml(const YAML::Node& node, int level) 
{
    if(node.IsScalar()) { // 纯量，单个的、不可再分的值
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4, ' ')
            << node.Scalar() << " - " << node.Type() << " - " << level;
    } else if(node.IsNull()) {
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4, ' ')
            << "NULL - " << node.Type() << " - " << level;
    } else if(node.IsMap()) {
        for(auto it = node.begin();
                it != node.end(); ++it) {
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4, ' ')
                    << it->first << " - " << it->second.Type() << " - " << level;
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << it->first.Scalar() << " "<< it->second.Scalar() << std::endl;
            print_yaml(it->second, level + 1);
        }
    } else if(node.IsSequence()) {
        for(size_t i = 0; i < node.size(); ++i) {
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4, ' ')
                << i << " - " << node[i].Type() << " - " << level;
            print_yaml(node[i], level + 1);
        }
    }
}

void test_yaml() 
{
    YAML::Node root = YAML::LoadFile("/home/wangk/code/cpp/sylar/code/my_sylar/tests/log.yaml");
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "map: " << YAML::NodeType::Map << " "
        << "null: " << YAML::NodeType::Null << " "
        << "scalar: " << YAML::NodeType::Scalar << " "
        << "sequence" << YAML::NodeType::Sequence << " "
        << "undefined" << YAML::NodeType::Undefined << std::endl;
    print_yaml(root, 0);
}

void test_stl()
{
    // SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "before: " << vec->getVal();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_int->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_float->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_vec->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_list->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_set->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_uset->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_map->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_umap->toString();
    YAML::Node root = YAML::LoadFile("/home/wangk/code/cpp/sylar/code/my_sylar/tests/log.yaml");
    sylar::Config::LoadFromYaml(root);

    std::map<std::string, int> map = g_map->getVal();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << map["name"] << " " << map["sex"];
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_int->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_float->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_vec->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_list->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_set->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_uset->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_map->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_umap->toString();
}

class Person
{
public:
    Person(){}
    std::string m_name;
    bool m_sex;
    int m_age;

    std::string toString()const
    {
        std::stringstream ss;
        ss << "name: " << m_name << " "
            << "sex: " << m_sex << " "
            << "age: " << m_age;
        
        return ss.str();
    }

    bool operator==(const Person& other) const
    {
        return m_name == other.m_name
            && m_sex == other.m_sex
            && m_age == other.m_age;
    }
};

namespace sylar
{
template<>
class LexicalCast<std::string, Person>
{
public:
    Person operator()(const std::string& v)
    {

        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_name = node["name"].as<std::string>();
        p.m_sex = node["sex"].as<bool>();
        p.m_age = node["age"].as<int>();
        return p;
    }
};

template<>
class LexicalCast<Person, std::string>
{
public:
    std::string operator()(const Person& p)
    {
        YAML::Node node;
        node["name"] = p.m_name;
        node["age"] = p.m_age;
        node["sex"] = p.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};
}

sylar::ConfigVar<Person>::ptr g_person =
    sylar::Config::Lookup("class.person", Person(), "system person");

sylar::ConfigVar<std::map<std::string, Person> >::ptr g_person_map =
    sylar::Config::Lookup("class.map", std::map<std::string, Person>(), "system person");

sylar::ConfigVar<std::map<std::string, std::vector<Person> > >::ptr g_person_vec_map =
    sylar::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person> >(), "system person");


void test_class()
{
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_person->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_person_map->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "before: " << g_person_vec_map->toString();

    YAML::Node root = YAML::LoadFile("/home/wangk/code/cpp/sylar/code/my_sylar/tests/log.yaml");
    sylar::Config::LoadFromYaml(root);

    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_person->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_person_map->toString();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << "after: " << g_person_vec_map->toString();

}

void test_log()
{
    static sylar::Logger::ptr system_log = SYLAR_LOG_NAME("system");
    SYLAR_LOG_INFO(system_log) << "hello system" << std::endl;

    // std::cout << sylar::LoggerMgr::getInstance()->toYamlString() << std::endl;
    YAML::Node root = YAML::LoadFile("/home/wangk/code/cpp/sylar/code/my_sylar/tests/log.yaml");

    sylar::Config::LoadFromYaml(root);

    // std::cout << "=============" << std::endl;
    // std::cout << sylar::LoggerMgr::getInstance()->toYamlString() << std::endl;
    // std::cout << "=============" << std::endl;
    // std::cout << root << std::endl;

    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "hello root" << std::endl;
    SYLAR_LOG_INFO(system_log) << "hello system" << std::endl;

    // system_log->setFormatter("%d{%Y-%m-%d %H:%M:%S}- %m%n");
    SYLAR_LOG_INFO(system_log) << "hello system" << std::endl;
}
int main()
{
    // test_stl();
    // test_class();
    test_log();
    return 0;
}


// #include <iostream>
// #include <string>
// #include <vector>
// #include <yaml-cpp/yaml.h>
// #include <boost/lexical_cast.hpp>

// using namespace std;

// template<class F, class T>
// class LexicalCast {
// public:
//     /**
//      * @brief 类型转换
//      * @param[in] v 源类型值
//      * @return 返回v转换后的目标类型
//      * @exception 当类型不可转换时抛出异常
//      */
//     T operator()(const F& v) {
//         return boost::lexical_cast<T>(v);
//     }
// };

// template<class T>
// class LexicalCast<std::string, std::vector<T> > {
// public:
//     std::vector<T> operator()(const std::string& v) {
//         YAML::Node node = YAML::Load(v);
//         typename std::vector<T> vec;
//         std::stringstream ss;
//         for(size_t i = 0; i < node.size(); ++i) {
//             ss.str("");
//             ss << node[i];
//             vec.push_back(LexicalCast<std::string, T>()(ss.str()));
//         }
//         return vec;
//     }
// };


// template<class T>
// class LexicalCast<std::vector<T>, std::string> {
// public:
//     std::string operator()(const std::vector<T>& v) {
//         YAML::Node node(YAML::NodeType::Sequence);
//         for(auto& i : v) {
//             node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
//         }
//         std::stringstream ss;
//         ss << node;
//         return ss.str();
//     }
// };

// int  main()
// {
//     vector<float> src {1.1, 2, 2, 3.4};
//     cout << LexicalCast<vector<float>, std::string>()(src) << endl;
//     // vector<float> vec =  LexicalCast<std::string, vector<float>>()("- 1.1 - 2 - 2 - 3.4");
//     // cout << vec[0] << endl;
//     cout << LexicalCast<std::string, int>()("123") << endl;
// }



// #include <iostream>
// #include <yaml-cpp/yaml.h>
// #include <fstream>

// using namespace std;

// int main(int argc,char** argv)
// {
//     YAML::Node config = YAML::LoadFile("../config.yaml");

//     cout << "Node type " << config.Type() << endl;
//     cout << "skills type " << config["skills"].Type() << endl;

//     cout << "name:" << config["name"].as<string>() << endl;
//     cout << "sex:" << config["sex"].as<string>() << endl;
//     cout << "age:" << config["age"].as<int>() << endl;

//     cout << "skills c++:" << config["skills"]["c++"].as<int>() << endl;
//     cout << "skills java:" << config["skills"]["java"].as<int>() << endl;
//     cout << "skills android:" << config["skills"]["android"].as<int>() << endl;
//     cout << "skills python:" << config["skills"]["python"].as<int>() << endl;

//     for(YAML::const_iterator it= config["skills"].begin(); it != config["skills"].end();++it)
//     {
//         cout << it->first.as<string>() << ":" << it->second.as<int>() << endl;
//     }

//     YAML::Node test1 = YAML::Load("[1,2,3,4]");
//     cout << " Type: " << test1.Type() << endl;

//     YAML::Node test2 = YAML::Load("1");
//     cout << " Type: " << test2.Type() << endl;

//     YAML::Node test3 = YAML::Load("{'id':1,'degree':'senior'}");
//     cout << " Type: " << test3.Type() << endl;

//     ofstream fout("testconfig.xml");

//     config["score"] = 99;

//     fout << config;

//     fout.close();


//     return 0;
// }