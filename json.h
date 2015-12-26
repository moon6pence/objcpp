#ifndef OBJCPP_JSON_H_
#define OBJCPP_JSON_H_

#include "picojson.h"

#include <fstream>

struct json_writer : public property_visitor
{
    mutable picojson::object json;

    json_writer()
    {
    }

    void visit(property<bool> &property) const
    {
        json.insert(picojson::object::value_type(property.name(), picojson::value(property.get())));
    }

    void visit(property<int> &property) const
    {
        json.insert(picojson::object::value_type(property.name(), picojson::value(static_cast<double>(property.get()))));
    }

    void visit(property<float> &property) const
    {
        json.insert(picojson::object::value_type(property.name(), picojson::value(static_cast<double>(property.get()))));
    }

    void visit(property<std::string> &property) const
    {
        json.insert(picojson::object::value_type(property.name(), picojson::value(property.get())));
    }

    void visit(property<Object> &property) const
    {
    }

    void visit(operation &operation) const
    {
    }
};

template <class Object>
picojson::value writeJSON(Object *object)
{
    json_writer writer;
    object->accept(writer);
    return picojson::value(writer.json);
}

struct write_json_visitor
{
    picojson::object &json;

    write_json_visitor(picojson::object &json) : json(json)
    {
    }

    template <class Object>
    void operator() (const std::string &name, Object *object) const
    {
        json.insert(picojson::object::value_type(name, writeJSON(object)));
    }

    void write_file(const std::string &jsonfile)
    {
        std::fstream file(jsonfile, std::ios::out);
        file << picojson::value(json).serialize(true);
        file.close();
    }
};

struct json_reader : public property_visitor
{
    const picojson::value &json;

    json_reader(const picojson::value &json) : json(json)
    {
    }

    void visit(property<bool> &property) const
    {
        if (json.contains(property.name()))
            property.set(json.get(property.name()).get<bool>());
        else
            std::cout << "WARNING: Cannot find the property " << property.name() << " in JSON.";
    }

    void visit(property<int> &property) const
    {
        if (json.contains(property.name()))
            property.set(static_cast<int>(json.get(property.name()).get<double>()));
        else
            std::cout << "WARNING: Cannot find the property " << property.name() << " in JSON.";
    }

    void visit(property<float> &property) const
    {
        if (json.contains(property.name()))
            property.set(static_cast<int>(json.get(property.name()).get<double>()));
        else
            std::cout << "WARNING: Cannot find the property " << property.name() << " in JSON.";
    }

    void visit(property<std::string> &property) const
    {
        if (json.contains(property.name()))
            property.set(json.get(property.name()).get<std::string>());
        else
            std::cout << "WARNING: Cannot find the property " << property.name() << " in JSON.";
    }

    void visit(property<Object> &property) const
    {
    }

    void visit(operation &operation) const
    {
    }
};

template <class Object>
void readJSON(Object *object, const picojson::value &json)
{
    if (!json.is<picojson::object>())
    {
        std::cout << "WARNING: Cannot read the json object" << std::endl;
        return;
    }

    json_reader reader(json);
    object->accept(reader);
}

struct read_json_visitor
{
    picojson::value json;

    read_json_visitor(picojson::value &json) : json(json)
    {
    }

    read_json_visitor(const std::string &jsonfile)
    {
        std::fstream file(jsonfile, std::ios::in);
        if (!file.is_open())
        {
            std::cerr << "ERROR: Failed to read " << jsonfile << std::endl;
            return;
        }

        file >> json;
        file.close();

        std::string err = picojson::get_last_error();
        if (!err.empty())
        {
            std::cerr << "Failed to parse " << jsonfile << ": " << err << std::endl;
            return;
        }
    }

    bool is_open()
    {
        return !json.is<picojson::null>();
    }

    template <class Object>
    void operator() (const std::string &name, Object *object) const
    {
        const picojson::value &json_object = json.get(name);

        if (json_object.is<picojson::object>())
            readJSON(object, json.get(name));
        else
            std::cout << "WARNING: Cannot find the object " << name << " in JSON." << std::endl;
    }
};

#endif // OBJCPP_JSON_H_