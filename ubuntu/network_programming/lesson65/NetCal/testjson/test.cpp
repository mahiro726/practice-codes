#include <iostream>
#include <sstream>
#include <jsoncpp/json/json.h>

int main()
{
    std::string json_string = "{\"name\":\"张三\", \"age\":30, \"city\":\"北京\"}";
    std::cout << json_string << std::endl;

    Json::Reader reader;
    Json::Value root;
    bool parseok = reader.parse(json_string, root);

    if(!parseok)
    {}

    std::string name = root["name"].asString();
    int age = root["age"].asInt();
    std::string city = root["city"].asString();

    std::cout << name << std::endl;
    std::cout << age << std::endl;
    std::cout << city << std::endl;


    // Json::Value sub_root1;
    // sub_root1["name"] = "张三"; // key:value
    // sub_root1["sex"] = "男";
    // sub_root1["age"] = 18;

    // Json::Value sub_root2;
    // sub_root2["math"] = 135;
    // sub_root2["chinese"] = 120;

    // Json::Value one;
    // one["who"] = sub_root1;
    // one["score"] = sub_root2;

    // Json::Value root;
    // for (int i = 0; i < 10; i++)
    // {
    //     root.append(one);
    // }
    

    // Json::StreamWriterBuilder wbuilder;
    // std::unique_ptr<Json::StreamWriter> swriter(wbuilder.newStreamWriter());

    // std::stringstream ss;
    // swriter->write(root, &ss);

    // std::cout << ss.str() << std::endl;

    // // version2
    // Json::FastWriter writer;
    // std::string s = writer.write(root);
    // std::cout << s << std::endl;

    // std::string s = root.toStyledString();
    
    // std::cout << s << std::endl;
}