//
// Created by Developer on 01.12.2023.
//
#include "xml_util.h"

#include <xmlOptions.pb.h>
#include <format>

namespace google::protobuf::util{
    const FieldDescriptor* FindFieldByXmlName(Message& message, const std::string& nodeName){
        auto descriptor = message.GetDescriptor();
        for(auto i = 0; i < descriptor->field_count(); ++i){
            auto field = descriptor->field(i);
            auto xmlOptions = field->options().GetExtension(xml_field_options);
            std::string fieldName = field->name();
            if(nodeName == fieldName){ return field;}
            fieldName = field->json_name();
            if(nodeName == fieldName){ return field;}
            fieldName = field->full_name();
            if(nodeName == fieldName){ return field;}
            if (xmlOptions.has_name()){
                fieldName = xmlOptions.name();
            }
            if(nodeName == fieldName){ return field;}
        }
        return nullptr;
    }

    std::string XmlPrepareValue(std::string value){
        value.erase(remove_if(value.begin(), value.end(), isspace), value.end());
        std::transform(value.begin(), value.end(), value.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return value;
    }

    bool XmlStringToBool(std::string value){
        value = XmlPrepareValue(value) ;
        if(value == "true" || value == "1" || value == "on"){
            return true;
        }
        else if(value == "false" || value == "0" || value == "off")
        {
            return false;
        }
        else{
            throw std::runtime_error("Bad Conversion to bool");
        }
    }

    float XmlStringToFloat(std::string value){
        return std::stof(value);
    }

    const EnumValueDescriptor* XmlStringToEnum(std::string value, const EnumDescriptor* enumDescriptor){
        //auto xmlOptions = enumDescriptor->options().GetExtension(xml_enum_options);
        value.erase(remove_if(value.begin(), value.end(), isspace), value.end());
        const EnumValueDescriptor* val;
        val = enumDescriptor->FindValueByName(value);
        if (!val){
            try{
                val = enumDescriptor->FindValueByNumber(std::stoi(value));
            }
            catch (std::exception& e){
                return nullptr;
            }
        }
        return val;
    }

    Status StrValueToField(Message& message, const FieldDescriptor* field, const std::string& value){
        const Reflection* reflection = message.GetReflection();
        switch (field->cpp_type()) {
            case FieldDescriptor::CPPTYPE_STRING: {
                if(field->is_optional()){
                    reflection->SetString(&message,field,value);
                    return {StatusCode::kOk,"optional string is written"};
                }
                else if(field->is_repeated()){
                    reflection->AddString(&message,field,value);
                    return {StatusCode::kOk,"repeated string is written"};
                }
                else{
                    return {StatusCode::kUnknown,"nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_BOOL: {
                bool val;
                try
                {
                    val = XmlStringToBool(value);
                }
                catch (std::exception &e) {
                    return {StatusCode::kOutOfRange, e.what()};
                }
                if(field->is_optional()){
                    reflection->SetBool(&message,field,val);
                    return {StatusCode::kOk,"optional bool is written"};
                }
                else if(field->is_repeated()){
                    reflection->AddBool(&message,field,val);
                    return {StatusCode::kOk,"repeated bool is written"};
                }
                else{
                    return {StatusCode::kUnknown,"nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_DOUBLE: {
                double val;
                try {
                    val = std::stod(value);
                }
                catch (std::exception &e) {
                    return {StatusCode::kOutOfRange, e.what()};
                }
                if (field->is_optional()) {
                    reflection->SetDouble(&message, field, val);
                    return {StatusCode::kOk, "optional double is written"};
                } else if (field->is_repeated()) {
                    reflection->AddDouble(&message, field, val);
                    return {StatusCode::kOk, "repeated double is written"};
                }
                else{
                    return {StatusCode::kUnknown,"nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_FLOAT:{
                float val;
                try
                {
                    val = XmlStringToFloat(value);
                }
                catch (std::exception &e) {
                    return {StatusCode::kOutOfRange, e.what()};
                }
                if(field->is_optional()){
                    reflection->SetFloat(&message,field,val);
                    return {StatusCode::kOk,"optional float is written"};
                }
                else if(field->is_repeated()){
                    reflection->AddFloat(&message,field,val);
                    return {StatusCode::kOk,"repeated float is written"};
                }
                else{
                    return {StatusCode::kUnknown,"nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_INT32:{
                int32 val;
                try
                {
                    val = std::stoi(value);
                }
                catch (std::exception &e) {
                    return {StatusCode::kOutOfRange, e.what()};
                }
                if(field->is_optional()){
                    reflection->SetInt32(&message,field,val);
                    return {StatusCode::kOk,"optional int32 is written"};
                }
                else if(field->is_repeated()){
                    reflection->AddInt32(&message,field,val);
                    return {StatusCode::kOk,"repeated int32 is written"};
                }
                else{
                    return {StatusCode::kUnknown,"nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_INT64:{
                int64 val;
                try
                {
                    val = std::stoll(value);
                }
                catch (std::exception &e) {
                    return {StatusCode::kOutOfRange, e.what()};
                }
                if(field->is_optional()){
                    reflection->SetInt64(&message,field,val);
                    return {StatusCode::kOk,"optional int64 is written"};
                }
                else if(field->is_repeated()){
                    reflection->AddInt64(&message,field,val);
                    return {StatusCode::kOk,"repeated int64 is written"};
                }
                else{
                    return {StatusCode::kUnknown,"nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_UINT32:{
                uint32 val;
                try
                {
                    val = std::stoul(value);
                }
                catch (std::exception &e) {
                    return {StatusCode::kOutOfRange, e.what()};
                }
                if(field->is_optional()){
                    reflection->SetUInt32(&message,field,val);
                    return {StatusCode::kOk,"optional uint32 is written"};
                }
                else if(field->is_repeated()){
                    reflection->AddUInt32(&message,field,val);
                    return {StatusCode::kOk,"repeated uint32 is written"};
                }
                else{
                    return {StatusCode::kUnknown,"nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_UINT64:{
                uint64 val;
                try
                {
                    val = std::stoull(value);
                }
                catch (std::exception &e) {
                    return {StatusCode::kOutOfRange, e.what()};
                }
                if(field->is_optional()){
                    reflection->SetUInt64(&message,field,val);
                    return {StatusCode::kOk,"optional uint64 is written"};
                }
                else if(field->is_repeated()){
                    reflection->AddUInt64(&message,field,val);
                    return {StatusCode::kOk,"repeated uint64 is written"};
                }
                else{
                    return {StatusCode::kUnknown,"nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_ENUM: {
                auto val = XmlStringToEnum(value, field->enum_type());
                if (field->is_optional()) {
                    reflection->SetEnum(&message, field, val);
                    return {StatusCode::kOk, "optional enum value is written"};
                } else if (field->is_repeated()) {
                    reflection->AddEnum(&message, field, val);
                    return {StatusCode::kOk, "repeated enum value is written"};
                } else {
                    return {StatusCode::kUnknown, "nothing is written"};
                }
            }
            case FieldDescriptor::CPPTYPE_MESSAGE:{
                return {StatusCode::kInvalidArgument, "message should be written not here"};
            }
        }
    }

    Status XmlToField(Message& message, const rapidxml::xml_node<>* node)
    {
        std::string nodeName;
        std::string nodeValue;

        if (node->type() == rapidxml::node_element) {
            nodeName = node->name();
            auto field = FindFieldByXmlName(message, nodeName);
            if (field){
                if(field->type() != FieldDescriptor::TYPE_MESSAGE){
                    return StrValueToField(message, field, node->value());
                }
                else if(field->type() == FieldDescriptor::TYPE_MESSAGE){
                    if (field->is_optional()) {
                        auto msg = message.GetReflection()->MutableMessage(&message,field);
                        return XmlToMessage(*msg, node);
                    } else if (field->is_repeated()) {
                        message.GetReflection()->AddMessage(&message,field);
                        auto fieldSize = message.GetReflection()->FieldSize(message, field);
                        auto msg = message.GetReflection()->MutableRepeatedMessage(&message,field, fieldSize-1);
                        return XmlToMessage(*msg, node);
                    } else {
                        return {StatusCode::kUnknown, "nothing is written"};
                    }
                }
                else{
                    return {StatusCode::kNotFound, "unknown field type"};
                }
            }
        }
        else{
            return {StatusCode::kInvalidArgument, "node must be element"};
        }
    }

    Status XmlToMessage(Message& message, const rapidxml::xml_node<>* node){
        std::string nodeName;
        if (node->type() == rapidxml::node_element) {
            nodeName = node->name();
        }
        else{
            return {StatusCode::kInvalidArgument, "node must be element"};
        }

        auto msgName = message.GetTypeName();

        msgName = message.GetDescriptor()->name();

        auto xmlOptions = message.GetDescriptor()->options().GetExtension(xml_message_options);

        if(xmlOptions.has_name())
        {
            msgName = xmlOptions.name();
        }

        if (nodeName != msgName){
            return {StatusCode::kNotFound, "message " + msgName + " in xml NOT-found"};
        }
        else
        {
            for(auto child = node->first_node(); child; child = child->next_sibling()){
                XmlToField(message, child);
            }
        }
        return {StatusCode::kOk,""};
    }

    Status MessageToXmlDoc(const Message& message, rapidxml::xml_document<>* doc){
        return MessageToXmlNode(message, static_cast<rapidxml::xml_node<>*>(doc));
    }

    Status MessageToXmlNode(const Message& message, rapidxml::xml_node<>* node, const char* fieldName){
        const char* msgName;
        auto doc = node->document();

        if (!fieldName) {
            auto xmlOptions = message.GetDescriptor()->options().GetExtension(xml_message_options);

            if (xmlOptions.has_name()) {
                msgName = doc->allocate_string(xmlOptions.name().c_str());
            }
            else{
                msgName = doc->allocate_string(message.GetDescriptor()->name().c_str());
            }
        }
        else{
            msgName = fieldName;
        }


        auto msgNode = doc->allocate_node(rapidxml::node_element, msgName);

        node->append_node(msgNode);

        const Reflection *reflection = message.GetReflection();
        std::vector<const FieldDescriptor*> fields;
        reflection->ListFields(message, &fields);

        for(auto field : fields){
            FieldToXmlNode(message, field, msgNode);
        }
        return {StatusCode::kOk, ""};
    }

    Status FieldToXmlNode(const Message& message, const FieldDescriptor* field, rapidxml::xml_node<>* node){
        const char* fieldName;
        auto doc = node->document();
        auto xmlOptions = field->options().GetExtension(xml_field_options);

        if(xmlOptions.has_name()){
            fieldName = doc->allocate_string(xmlOptions.name().c_str());
        }
        else{
            fieldName = doc->allocate_string(field->name().c_str());
        }

        auto maxInd = field->is_repeated() ? message.GetReflection()->FieldSize(message, field) : 0;

        const Reflection* reflection = message.GetReflection();

        switch (field->cpp_type()) {
            case FieldDescriptor::CPPTYPE_STRING: {
                //TODO check content of string and use CDATA if string contains xml tags and so on
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(reflection->GetRepeatedString(message, field, i).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(reflection->GetString(message, field).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;
            }
            case FieldDescriptor::CPPTYPE_BOOL: {
                //TODO add to xml_options format
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(std::to_string(reflection->GetRepeatedBool(message, field, i)).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(std::to_string(reflection->GetBool(message, field)).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;
            }
            case FieldDescriptor::CPPTYPE_DOUBLE: {
                //TODO add to xml_options format
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(std::to_string(reflection->GetRepeatedDouble(message, field, i)).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(std::to_string(reflection->GetDouble(message, field)).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;
            }
            case FieldDescriptor::CPPTYPE_FLOAT: {
                //TODO add to xml_options format
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(std::to_string(reflection->GetRepeatedFloat(message, field, i)).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(std::format("{:3.5e}",reflection->GetFloat(message, field)).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;
            }
            case FieldDescriptor::CPPTYPE_INT32: {
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(std::to_string(reflection->GetRepeatedInt32(message, field, i)).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(std::to_string(reflection->GetInt32(message, field)).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;
            }
            case FieldDescriptor::CPPTYPE_INT64: {
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(std::to_string(reflection->GetRepeatedInt64(message, field, i)).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(std::to_string(reflection->GetInt64(message, field)).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;
            }
            case FieldDescriptor::CPPTYPE_UINT32: {
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(std::to_string(reflection->GetRepeatedUInt32(message, field, i)).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(std::to_string(reflection->GetUInt32(message, field)).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;
            }
            case FieldDescriptor::CPPTYPE_UINT64: {
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(std::to_string(reflection->GetRepeatedUInt64(message, field, i)).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(std::to_string(reflection->GetUInt64(message, field)).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;

            }
            case FieldDescriptor::CPPTYPE_ENUM: {
                //TODO add to xml_options option value/enum name
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        auto value = doc->allocate_string(std::to_string(reflection->GetRepeatedEnumValue(message, field, i)).c_str());
                        auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                        node->append_node(newNode);
                    }
                }
                else{
                    auto value = doc->allocate_string(std::to_string(reflection->GetEnumValue(message, field)).c_str());
                    auto newNode = doc->allocate_node(rapidxml::node_element,fieldName, value);
                    node->append_node(newNode);
                }
                break;

            }
            case FieldDescriptor::CPPTYPE_MESSAGE: {
                if(field->is_repeated()) {
                    for (auto i = 0; i < maxInd; ++i) {
                        MessageToXmlNode(reflection->GetRepeatedMessage(message, field, i), node, fieldName);
                    }
                }
                else{
                    MessageToXmlNode(reflection->GetMessage(message, field), node, fieldName);
                }
                break;
            }
        }
        return {StatusCode::kOk, ""};
    }
}