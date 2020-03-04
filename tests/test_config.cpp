#include "../sylar/config.h"
#include "../sylar/log.h"

static sylar::ConfigVar<int>::ptr global = 
    sylar::Config::Lookup("port", (int)8080, "port");

static sylar::ConfigVar<int>::ptr c = 
    sylar::Config::Lookup("port", (int)8080, "port");

int main()
{
    SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << global->getVal();
    SYLAR_LOG_DEBUG(SYLAR_LOG_ROOT()) << global->toString();
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