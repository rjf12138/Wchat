#include "server_json.h"

Json::Json(const std::string &json_path)
{
    json_file.open(json_path);
    if(json_file.good()){
        json_file >> js;
    }else{
        throw std::runtime_error("Unable to open json path");
    }
}

Json::~Json()
{
    json_file.close();
}

Json&
Json::get_json_instance(const std::string &json_path)
{
    static Json json_instance(json_path);
    json_instance.json_file_path = json_path;
    return json_instance;
}

void 
Json::read_value(const std::string &name , int &value)
{
    value = js[name];
}
void 
Json::read_value(const std::string &name , double &value)
{
    value = js[name];
}
void 
Json::read_value(const std::string &name , std::string &value)
{
    value = js[name];
}

void 
Json::read_value(const std::string &name , bool &value)
{
    value = js[name];
}

void 
Json::read_value(const std::string &name , short &value)
{
    value = js[name];
}
