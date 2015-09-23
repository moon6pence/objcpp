#ifndef OBJCPP_JSON_H_
#define OBJCPP_JSON_H_

#include "picojson.h"

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

struct json_reader : public property_visitor
{
    mutable picojson::value &json;

    json_reader(picojson::value &json) : json(json)
    {
    }

    void visit(property<bool> &property) const
    {
        property.set(json.get(property.name()).get<bool>());
    }

    void visit(property<int> &property) const
    {
        property.set(static_cast<int>(json.get(property.name()).get<double>()));
    }

    void visit(property<float> &property) const
    {
        property.set(static_cast<int>(json.get(property.name()).get<double>()));
    }

    void visit(property<std::string> &property) const
    {
        property.set(json.get(property.name()).get<std::string>());
    }

    void visit(property<Object> &property) const
    {
    }

    void visit(operation &operation) const
    {
    }
};

template <class Object>
void readJSON(Object *object, picojson::value &json)
{
    json_reader reader(json);
    object->accept(reader);
}

#endif // OBJCPP_JSON_H_