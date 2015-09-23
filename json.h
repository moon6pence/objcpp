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
picojson::value writeJson(Object *object)
{
    json_writer writer;
    object->accept(writer);
    return picojson::value(writer.json);
}

#endif // OBJCPP_JSON_H_