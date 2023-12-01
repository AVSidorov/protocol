#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <stdexcept>

#include <google/protobuf/util/json_util.h>
#include <rapidxml/rapidxml.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "test.pb.h"
#include "pb2xml/xml_format.h"

using namespace google::protobuf;

namespace google::protobuf::util{
//    void FieldToXML(const Message& message, rapidxml::xml_node<>* doc,const FieldDescriptor* field){
//      const Reflection *reflection = message.GetReflection();
//      if(field->is_repeated()){
//        for(auto i=0; i<reflection->FieldSize(message, field); ++i){
//
//        }
//      }
//    }
//
//
//    void MessageToXML(const Message& message, rapidxml::xml_node<>* doc){
//      const Reflection *reflection = message.GetReflection();
//      std::vector<const FieldDescriptor*> fields;
//      reflection->ListFields(message, &fields);
//      for(auto field : fields){
//        FieldToXML(message, doc, field);
//      }
//    }

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

    bool XmlString2bool(std::string value){
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

    float XmlString2float(std::string value){
        boost::algorithm::trim(value);
        return boost::lexical_cast<float>(value);
    }

    Status XMLtoRepeatedField(Message& message, const FieldDescriptor* field, const std::string& value){
      const Reflection* reflection = message.GetReflection();
      switch (field->cpp_type()) {
        case FieldDescriptor::CPPTYPE_BOOL: {
          try {
            reflection->AddBool(&message, field, XmlString2bool(value));
            return {StatusCode::kOk,""};
          }
          catch (std::exception &e) {
            return {StatusCode::kOutOfRange, e.what()};
          }
        }
        case FieldDescriptor::CPPTYPE_DOUBLE:{
          return {StatusCode::kOk, "double"};
        }
        case FieldDescriptor::CPPTYPE_FLOAT:{
          return {StatusCode::kOk, "float"};
        }
        case FieldDescriptor::CPPTYPE_INT32:{
          return {StatusCode::kOk, "int32"};
        }
        case FieldDescriptor::CPPTYPE_INT64:{
          return {StatusCode::kOk, "int64"};
        }
        case FieldDescriptor::CPPTYPE_ENUM:{
          return {StatusCode::kOk, "enum"};
        }
        case FieldDescriptor::CPPTYPE_UINT32:{
          return {StatusCode::kOk, "uint32"};
        }
        case FieldDescriptor::CPPTYPE_UINT64:{
          return {StatusCode::kOk, "uint64"};
        }
        case FieldDescriptor::CPPTYPE_MESSAGE:{
          return {StatusCode::kOk, "message"};
        }
        case FieldDescriptor::CPPTYPE_STRING: {
          reflection->AddString(&message, field, value);
          return {StatusCode::kOk,""};
        }
      }
    }

    Status XMLtoOptionalField(Message& message, const FieldDescriptor* field, const std::string& value){
      const Reflection* reflection = message.GetReflection();
      switch (field->cpp_type()) {
        case FieldDescriptor::CPPTYPE_BOOL: {
          try {
            reflection->SetBool(&message, field, XmlString2bool(value));
            return {StatusCode::kOk,"optional bool is written"};
          }
          catch (std::exception &e) {
            return {StatusCode::kOutOfRange, e.what()};
          }
        }
        case FieldDescriptor::CPPTYPE_DOUBLE: {
          try {
              //reflection->SetBool(&message, field, XmlString2fdouble(value));
            return {StatusCode::kOk, "optional double is written"};
          }
          catch (boost::bad_lexical_cast &e) {
            return {StatusCode::kOutOfRange, e.what()};
          }
        }
        case FieldDescriptor::CPPTYPE_FLOAT:{
          try {
            reflection->SetFloat(&message, field, XmlString2float(value));
            return {StatusCode::kOk,"optional double is written"};
          }
          catch (std::exception &e) {
            return {StatusCode::kOutOfRange, e.what()};
          }
        }
        case FieldDescriptor::CPPTYPE_INT32:{
          return {StatusCode::kOk, "int32"};
        }
        case FieldDescriptor::CPPTYPE_INT64:{
          return {StatusCode::kOk, "int64"};
        }
        case FieldDescriptor::CPPTYPE_ENUM:{
          return {StatusCode::kOk, "enum"};
        }
        case FieldDescriptor::CPPTYPE_UINT32:{
          return {StatusCode::kOk, "uint32"};
        }
        case FieldDescriptor::CPPTYPE_UINT64:{
          return {StatusCode::kOk, "uint64"};
        }
        case FieldDescriptor::CPPTYPE_MESSAGE:{
          return {StatusCode::kOk, "message"};
        }
        case FieldDescriptor::CPPTYPE_STRING: {
          reflection->AddString(&message, field, value);
          return {StatusCode::kOk,""};
        }
      }
    }

    Status XMLtoField(Message& message, const rapidxml::xml_node<> * node)
    {
      std::string nodeName;
      std::string nodeValue;

      if (node->type() == rapidxml::node_element) {
        nodeName = node->name();
        nodeValue = node->value();
        auto field = FindFieldByXmlName(message, nodeName);
        if (field->is_repeated()){
          return XMLtoRepeatedField(message, field, nodeValue);
        }
        else if(field->is_optional()){
            return XMLtoOptionalField(message, field, nodeValue);
        }
      }
      else{
        return {StatusCode::kInvalidArgument, "node must be element"};
      }
    }

    Status XMLtoMessage(Message& message, const rapidxml::xml_node<>* node){
      std::string nodeName;
      if (node->type() == rapidxml::node_element) {
        nodeName = node->name();
        //std::cout << "xml node name " << node->name() << std::endl;
      }
      else{
        //std::cout << "node must be element" << std::endl;
        return {StatusCode::kInvalidArgument, "node must be element"};
      }

      auto msgName = message.GetTypeName();

      auto xmlOptions = message.GetDescriptor()->options().GetExtension(xml_message_options);

      if(xmlOptions.has_name())
      {
        msgName = xmlOptions.name();
      }

      if (nodeName != msgName){
        //std::cout << "message " + msgName + " in xml NOT-found" << std::endl;
        return {StatusCode::kNotFound, "message " + msgName + " in xml NOT-found"};
      }
      else
      {
        for(auto child = node->first_node(); child; child = child->next_sibling()){
          auto field = FindFieldByXmlName(message, child->name());
          if (field){
            XMLtoField(message, child);
          }
        }
      }
      return {StatusCode::kOk,""};
  }

}

void messageFieldsFromReflection(const Message& message){
  std::cout << "\n\n Message from Reflection " << message.GetTypeName() << "\n\n";
  const Reflection *reflection = message.GetReflection();
  std::vector<const FieldDescriptor*> fields;
  reflection->ListFields(message, &fields);

  for(auto field : fields){
    std::cout << "name " << field->name() << "\n";
    std::cout << "full_name " << field->full_name() << "\n";
    std::cout << "json_name " << field->json_name() << "\n";
    std::cout << "number " << field->number() << "\n";
    std::cout << "type " << field->type() << "\n";
    std::cout << "type_name " << field->type_name() << "\n";
    std::cout << "cpp_type " << field->cpp_type() << "\n";
    std::cout << "cpp_type_name " << field->cpp_type_name() << "\n";
    std::cout << "label " << field->label() << "\n";
    std::cout << "is_optional " << field->is_optional() << "\n";
    std::cout << "is_repeated " << field->is_repeated() << "\n";
    std::cout << "is_required " << field->is_required() << "\n";
    std::cout << "is_packable " << field->is_packable() << "\n";
    std::cout << "is_packed " << field->is_packed() << "\n";
    std::cout << "is_map " << field->is_map() << "\n";
    std::cout << "index " << field->index() << "\n";
    std::cout << "has_default_value " << field->has_default_value() << "\n";

    auto fieldsOptions {field->options()};
    const Reflection *reflection1 = fieldsOptions.GetReflection();
    std::vector<const FieldDescriptor*> fields1;
    reflection1->ListFields(fieldsOptions, &fields1);
    std::cout << "\n";
  }
}

void messageFieldsFromDescriptor(const Message& message){
  std::cout << "\n\n Message from descriptor " << message.GetTypeName() << "\n\n";
  auto descriptor = message.GetDescriptor();
  for(auto i = 0; i < descriptor->field_count(); ++i){
    auto field = descriptor->field(i);
    std::cout << "name: "<< field->name() << std::endl;
    std::cout << "full_name: " << field->full_name() << std::endl;
    std::cout <<"json_name: "<< field->json_name() << std::endl;
    std::cout <<"cpp_type: " << field->cpp_type() << std::endl;
    std::cout <<"is_optional: " << field->is_optional() << std::endl;
    std::cout <<"is_repeated: " << field->is_repeated() << std::endl;
  }
}

void testXMLtoMessage(Message& message, std::string xmlString){
    // string should be copied
    // rapidxml changes string in place during parse
    std::cout << xmlString << std::endl;

    rapidxml::xml_document<> doc;

    doc.parse<0>(xmlString.data());

    message.Clear();
    std::cout << std::string(google::protobuf::util::XMLtoMessage(message, doc.first_node()).message()) << std::endl;
    std::cout << message.DebugString() << std::endl;
}

int main() {
    std::string outstring;
    ops::sap::Result message;
    message.set_return_int(ops::sap::ReturnInt::RETURN_INT_OK);

    std::cout << "message.result_int: "<< message.return_int() << std::endl;
    std::cout << message.DebugString() << std::endl;
    google::protobuf::util::MessageToJsonString(message, &outstring);
    std::cout << outstring << std::endl;


    ops::sap::DataType dataType;
    dataType.set_type("ResultInt");

    outstring.clear();
    google::protobuf::util::MessageToJsonString(dataType, &outstring);
    std::cout << outstring << std::endl;

    outstring.clear();
    outstring = "{\n"
                "  \"results\":{\n"
                "    \"values\": [\n"
                "      { \"name\": \"Abcd\", \"value\": \"1.0\"},\n"
                "      { \"name\": \"foo\", \"value\": \"0xffff\"}\n"
                "    ]\n"
                "  }\n"
                "}";
    std::cout << outstring << std::endl;
    google::protobuf::util::JsonStringToMessage(outstring, &message);
    for (auto value :message.results().values()){
        std::cout << "value.name: " << value.name() << std::endl;
        std::cout << "value.value: " << value.value() << std::endl;
    }
    ops::sap::Result_ResultData_Value value;
    value.set_name("NewValue");
    value.set_value("AX32.32");
    std::cout << value.DebugString() << std::endl;
    message.mutable_results()->add_values()->set_name("NewValue1");
    message.mutable_results()->mutable_values(message.results().values_size()-1)->set_value("asdf");
    message.mutable_results()->add_values()->CopyFrom(value);
    message.set_return_int(ops::sap::ReturnInt::RETURN_INT_OK);
    message.mutable_data_type()->CopyFrom(dataType);


    outstring.clear();
    google::protobuf::util::MessageToJsonString(message,&outstring);
    std::cout << outstring << std::endl;
    std::cout << message.DebugString() << std::endl;

    auto dataType1 = new ops::sap::DataType;
    dataType1->set_type("TypeUnknown");
    message.set_allocated_data_type(dataType1);
    std::cout << message.DebugString() << std::endl;

    auto data = new google::protobuf::Any;
    data->PackFrom(message);
    std::cout << "data message (Any): " << data->DebugString() << std::endl;

    message.set_allocated_data(data);

    outstring.clear();
    google::protobuf::util::MessageToJsonString(message,&outstring);
    std::cout << outstring << std::endl;
    std::cout << message.DebugString() << std::endl;

    message.Clear();
    std::cout << "Message cleared" << std::endl;
    std::cout << message.DebugString() << std::endl;
    std::cout << "End Message" << std::endl;

    std::cout << "Message from JSON" << std::endl;
    google::protobuf::util::JsonStringToMessage(outstring, &message);
    std::cout << message.DebugString() << std::endl << std::endl;

    ops::sap::Wrapper wrapper;
//    auto result = new google::protobuf::Any;
//    result->PackFrom(message);
    wrapper.mutable_content()->PackFrom(message);

    std::cout << "Wrapper Test" << "\n\n";
    outstring.clear();
    google::protobuf::util::MessageToJsonString(wrapper,&outstring);
    std::cout << outstring << std::endl;
    std::cout << wrapper.DebugString() << std::endl;

    outstring.clear();
    google::protobuf::XmlFormat::PrintToXmlString(message, &outstring);
    std::cout << "Message to XML test" << std::endl;
    std::cout << outstring << std::endl << std::endl;



    std::string xmlString =\
        "<RESULT>\n"
        "  <PROCESS_OK> true </PROCESS_OK>\n"
        "  <RESULT_IN> 3 </RESULT_IN>\n"
        "  <RESULT-DATA>\n"
        "    <MESSWERT>\n"
        "      <NAME> adsf </NAME>\n"
        "      <WERT> zzvzx </WERT>\n"
        "    </MESSWERT>\n"
        "    <MESSWERT>\n"
        "      <NAME> ddd </NAME>\n"
        "      <WERT> 1.32E-12 </WERT>\n"
        "    </MESSWERT>\n"
        "  </RESULT-DATA>\n"
        "  <ERROR_TEXT>\n"
        "     sas\n"
        "     Petya byl golodniy.\n"
        "  </ERROR_TEXT>\n"
        "</RESULT>";

    std::cout << xmlString << "\n\n";
    char* buf = new char[xmlString.size()];
//    xmlString.copy(buf,xmlString.size(), 0);
//    outstring.clear();
//    outstring = xml2json(buf);
//    std::cout << outstring << std::endl;

    xmlString.copy(buf,xmlString.size(), 0);
    rapidxml::xml_document<> packet;
    packet.parse<0>(xmlString.data());
//    std::cout << packet. << "\n\n";
//    rapidxml::print(std::cout,packet);

    rapidxml::xml_node<> *node;
    std::string ident;
    ident.reserve(16);
    std::vector<rapidxml::xml_node<>*> nodeStack;


    node = static_cast<rapidxml::xml_node<>*>(&packet);
    nodeStack.push_back(node);

    while (node){
        nodeStack.pop_back();
        if(node->type() == rapidxml::node_element)
        {
            std::cout << ident;
            if (node->name()) { std::cout << node->name() << ": "; }
            if (node->value()) { std::cout << node->value() << std::endl; }
        }

        if (node->first_node())
        {
            for (auto child =  node->last_node() ; child; child = child->previous_sibling())
            {
                nodeStack.push_back(child);
            }
        } //else if(!ident.empty()){        ident.pop_back();        }
        node = nodeStack.empty() ? nullptr : nodeStack.back();
        //ident.append("\t");
    }


    delete[] buf;

    ops::sap::AllInOne testMessage;
    messageFieldsFromReflection(testMessage);
    messageFieldsFromDescriptor(testMessage);

    xmlString ="<TEST></TEST>";
    testXMLtoMessage(message, xmlString);
    testXMLtoMessage(testMessage, xmlString);

    xmlString = xmlString ="<TEST><rpt>ABCD</rpt></TEST>";
    testXMLtoMessage(testMessage, xmlString);

    xmlString =\
        "<TEST>\n"
        "    <rpt>ABCD</rpt>\n"
        "    <FLOAT> 1.34e-12 </FLOAT>"
        "</TEST>\n";
    testXMLtoMessage(testMessage, xmlString);

    xmlString =\
        "<TEST>\n"
        "    <rpt>Df dlksl a</rpt>\n"
        "    <rpt> Hallo World!!!</rpt>\n"
        "    <FLOAT> 1.34e-12 </FLOAT>\n"
        "</TEST>\n";
    testXMLtoMessage(testMessage, xmlString);

    xmlString =\
        "<TEST>\n"
        "    <rpt>Df dlksl a</rpt>\n"
        "    <rpt> Hallo World!!!</rpt>\n"
        "    <FLOAT> 1.34e-12 </FLOAT>\n"
        "    <FLOAT> .034e12 </FLOAT>\n"
        "</TEST>\n";
    testXMLtoMessage(testMessage, xmlString);

    google::protobuf::ShutdownProtobufLibrary();
    return 0;

}
