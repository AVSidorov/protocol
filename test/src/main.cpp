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
//#include "berchtold.pb.h"
#include "pb2xml/xml_format.h"

using namespace google::protobuf;

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
        return std::stof(value);
    }

    const EnumValueDescriptor* XmlString2enum(std::string value,const EnumDescriptor* enumDescriptor){
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
              val = XmlString2bool(value);
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
                val = XmlString2float(value);
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
            auto val = XmlString2enum(value, field->enum_type());
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

    Status XMLtoField(Message& message, const rapidxml::xml_node<> * node)
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
                //return XMLtoMessage();
            }
            else{
                return {StatusCode::kNotFound, "node must be element"};
            }
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
      }
      else{
        return {StatusCode::kInvalidArgument, "node must be element"};
      }

      auto msgName = message.GetTypeName();

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
            XMLtoField(message, child);
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

void enumFieldsFromDescriptor(const EnumDescriptor * enumDescriptor){
    std::cout << enumDescriptor->DebugString() << std::endl;
    for(auto i=0; i<  enumDescriptor->value_count(); i++){
        auto val = enumDescriptor->value(i);
        std::cout<< "name: " << val->name() << std::endl;
        std::cout<< "full_name: " << val->full_name() << std::endl;
        std::cout<< "number: " << val->number() << std::endl;
        std::cout<< "index: " << val->index() << std::endl;
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

void testRapidXmlProbe(){
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

    testRapidXmlProbe();

    ops::sap::AllInOne testMessage;

    messageFieldsFromReflection(testMessage);
    messageFieldsFromDescriptor(testMessage);
    enumFieldsFromDescriptor(testMessage.GetDescriptor()->FindEnumTypeByName("Enm"));

    std::string xmlString;
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
    outstring.clear();
    google::protobuf::util::MessageToJsonString(testMessage, &outstring);
    std::cout << outstring << std::endl;

    xmlString =\
        "<TEST>\n"
        "    <rpt>Df dlksl a</rpt>\n"
        "    <rpt> Hallo World!!!</rpt>\n"
        "    <FLOAT> 1.34e-12 </FLOAT>\n"
        "    <RESULT_INT> 99 </RESULT_INT>\n"
        "</TEST>\n";
    testXMLtoMessage(testMessage, xmlString);
    std::cout << testMessage.enm() << std::endl;

    outstring.clear();
    google::protobuf::util::MessageToJsonString(testMessage, &outstring);
    std::cout << outstring << std::endl;
    google::protobuf::ShutdownProtobufLibrary();
    return 0;

}
