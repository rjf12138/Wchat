#ifndef __SERVER_JSON__
#define __SERVER_JSON__

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

class Json{
public:
    static Json& get_json_instance(const std::string &json_path);

    void read_value(const std::string &name, int &value);
    void read_value(const std::string &name, double &value);
    void read_value(const std::string &name, std::string &value);
    void read_value(const std::string &name, bool &value);
    void read_value(const std::string &name, short &value);

private:
    Json() = default;
    Json(const std::string &json_path);
    ~Json();
private:
    std::fstream    json_file;
    std::string     json_file_path;
    nlohmann::json  js;
};

#endif