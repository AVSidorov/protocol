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

bool XmlString2bool(std::string value);

float XmlString2float(std::string value);

const EnumValueDescriptor* XmlString2enum(std::string value,const EnumDescriptor* enumDescriptor);

Status StrValueToField(Message& message, const FieldDescriptor* field, const std::string& value);

Status XMLtoField(Message& message, const rapidxml::xml_node<> * node);

Status XMLtoMessage(Message& message, const rapidxml::xml_node<>* node);

}
#endif //PB2XML_XML_UTIL_H
