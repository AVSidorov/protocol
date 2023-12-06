//
// Created by Developer on 01.12.2023.
//

#ifndef XML_UTIL_H
#define XML_UTIL_H

#include "xml_util_constants.h"
#include <rapidxml/rapidxml.hpp>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

namespace google::protobuf::util{

XML_UTIL_EXPORT const FieldDescriptor* FindFieldByXmlName(Message& message, const std::string& nodeName);

std::string XmlPrepareValue(std::string value);

bool XmlStringToBool(std::string value);

float XmlStringToFloat(std::string value);

const EnumValueDescriptor* XmlStringToEnum(std::string value, const EnumDescriptor* enumDescriptor);

XML_UTIL_EXPORT Status StrValueToField(Message& message, const FieldDescriptor* field, const std::string& value);

XML_UTIL_EXPORT Status XmlToField(Message & message, const rapidxml::xml_node<> * node);

XML_UTIL_EXPORT Status XmlToMessage(Message & message, const rapidxml::xml_node<>* node);

XML_UTIL_EXPORT Status MessageToXmlDoc(const Message& message, rapidxml::xml_document<>* doc);

//@param fieldName is necessary due to name can be determined not by message name, but by fieldName (field of message type)
XML_UTIL_EXPORT Status MessageToXmlNode(const Message& message, rapidxml::xml_node<>* node, const char* fieldName = nullptr);

XML_UTIL_EXPORT Status FieldToXmlNode(const Message& message, const FieldDescriptor* field, rapidxml::xml_node<>* node);

}
#endif //XML_UTIL_H
