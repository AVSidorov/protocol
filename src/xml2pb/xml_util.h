//
// Created by Developer on 01.12.2023.
//

#ifndef PB2XML_XML_UTIL_H
#define PB2XML_XML_UTIL_H

#include <rapidxml/rapidxml.hpp>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <xmlOptions.pb.h>

namespace google::protobuf::util{

const FieldDescriptor* FindFieldByXmlName(Message& message, const std::string& nodeName);

std::string XmlPrepareValue(std::string value);

bool XmlStringToBool(std::string value);

float XmlStringToFloat(std::string value);

const EnumValueDescriptor* XmlStringToEnum(std::string value, const EnumDescriptor* enumDescriptor);

Status StrValueToField(Message& message, const FieldDescriptor* field, const std::string& value);

Status XmlToField(Message & message, const rapidxml::xml_node<> * node);

Status XmlToMessage(Message & message, const rapidxml::xml_node<>* node);

Status MessageToXmlDoc(const Message& message, rapidxml::xml_document<>* doc);

//@param fieldName is necessary due to name can be determined not by message name, but by fieldName (field of message type)
Status MessageToXmlNode(const Message& message, rapidxml::xml_node<>* node, const char* fieldName = nullptr);

Status FieldToXmlNode(const Message& message, const FieldDescriptor* field, rapidxml::xml_node<>* node);

}
#endif //PB2XML_XML_UTIL_H
