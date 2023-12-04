#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <google/protobuf/util/json_util.h>
#include <rapidxml/rapidxml.hpp>

#include "test.pb.h"
#include "pb2xml/xml_format.h"
#include "xml2pb/xml_util.h"

using namespace google::protobuf;

void messageFieldsFromReflection(const Message& message){
  std::cout << "\n\nMessage from Reflection " << "\n";
  std::cout << "TypeName: " << message.GetTypeName() << "\n";
  std::cout << "Message name: " << message.GetDescriptor()->name() << "\n";
  std::cout << "Message full_name: " << message.GetDescriptor()->full_name() << "\n\n";

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
    std::cout << "\n\nMessage from descriptor " << "\n";
    std::cout << "TypeName: " << message.GetTypeName() << "\n";
    std::cout << "Message name: " << message.GetDescriptor()->name() << "\n";
    std::cout << "Message full_name: " << message.GetDescriptor()->full_name() << "\n\n";

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
    std::cout << "======= START testXMLtoMessage =======" << "\n";
    const Reflection* reflection = message.GetReflection();
    const Descriptor* descriptor = message.GetDescriptor();

    // string should be copied
    // rapidxml changes string in place during parse
    std::cout << "Convert XML String: \n"<< xmlString << std::endl;
    std::cout<< "to message: " << descriptor->full_name() << std::endl;

    rapidxml::xml_document<> doc;

    doc.parse<0>(xmlString.data());

    message.Clear();
    std::cout << std::string(google::protobuf::util::XmlToMessage(message, doc.first_node()).message()) << std::endl;
    std::cout << "message.DebugString: \n"<< message.DebugString() << std::endl;
    std::cout << "message.ShortDebugString: \n"<< message.ShortDebugString() << std::endl;


    std::string outstring;
    google::protobuf::util::MessageToJsonString(message, &outstring);
    std::cout << "Message JSON: \n"<< outstring << std::endl;

    doc.clear();
    google::protobuf::util::MessageToXmlDoc(message, &doc);

    std::cout << doc;
    rapidxml::print(std::cout, doc, rapidxml::print_no_indenting);

    std::cout << "\n======= END testXMLtoMessage =======" << "\n\n";
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
//    message.mutable_data_type()->CopyFrom(dataType);


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
    std::cout << "Message to XML test pb2xml" << std::endl;
    std::cout << outstring << std::endl << std::endl;

    rapidxml::xml_document<> messageXML;

    google::protobuf::util::MessageToXmlDoc(message, &messageXML);

    std::cout << "Message to XML test sid: \n" << messageXML;

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
        "    <FLOAT> -.1 </FLOAT>\n"
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
    std::cout << "AllInOne.enm (value): " <<testMessage.enm() << std::endl;

    xmlString =\
        "<TEST>\n"
        "    <rpt>Df dlksl a</rpt>\n"
        "    <rpt> Hallo World!!!</rpt>\n"
        "    <FLOAT> 1.34e-12 </FLOAT>\n"
        "    <RESULT_INT> 99 </RESULT_INT>\n"
        "</TEST>\n";
    testXMLtoMessage(testMessage, xmlString);
    std::cout << "AllInOne.enm (value): " << testMessage.enm() << std::endl;


    xmlString =\
        "<TEST>\n"
        "    <rpt>Df dlksl a</rpt>\n"
        "    <rpt> Hallo World!!!</rpt>\n"
        "    <FLOAT> 1.34e-12 </FLOAT>\n"
        "    <RESULT_INT> 99 </RESULT_INT>\n"
        "    <RESULT-DATA>\n"
        "    </RESULT-DATA>\n"
        "</TEST>\n";
    testXMLtoMessage(testMessage, xmlString);

    xmlString =\
        "<TEST>\n"
        "    <rpt>Df dlksl a</rpt>\n"
        "    <rpt> Hallo World!!!</rpt>\n"
        "    <FLOAT> 1.34e-12 </FLOAT>\n"
        "    <RESULT_INT> 99 </RESULT_INT>\n"
        "    <RESULT-DATA>\n"
        "        <MESSWERT>\n"
        "            <NAME>Voltage</NAME>\n"
        "            <WERT>1.3</WERT>\n"
        "        </MESSWERT>\n"
        "    </RESULT-DATA>\n"
        "</TEST>\n";
    testXMLtoMessage(testMessage, xmlString);

    xmlString =\
        "<TEST>\n"
        "    <rpt>Df dlksl a</rpt>\n"
        "    <rpt> Hallo World!!!</rpt>\n"
        "    <FLOAT> 1.34e-12 </FLOAT>\n"
        "    <RESULT_INT> 99 </RESULT_INT>\n"
        "    <RESULT-DATA>\n"
        "        <MESSWERT>\n"
        "            <NAME>Voltage</NAME>\n"
        "            <WERT>1.3</WERT>\n"
        "        </MESSWERT>\n"
        "        <MESSWERT>\n"
        "            <NAME>Serial</NAME>\n"
        "            <WERT>AX3.23_999-23</WERT>\n"
        "        </MESSWERT>\n"
        "    </RESULT-DATA>\n"
        "</TEST>\n";
    testXMLtoMessage(testMessage, xmlString);

    testXMLtoMessage(testMessage, xmlString);

    google::protobuf::ShutdownProtobufLibrary();
    return 0;

}
