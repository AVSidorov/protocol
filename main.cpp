#include <iostream>
#include <string>
#include "proto/test.pb.h"
#include <google/protobuf/util/json_util.h>
#include "ert/json2xml.hpp"
#include "xml2json.hpp"
#include <stdio.h>

int main() {
    std::string outstring;
    ops::sap::Result message;
    message.set_result_int(ops::sap::ResultInt::RESULT_INT_OK);

    std::cout << "message.result_int: "<< message.result_int() << std::endl;
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
    message.set_result_int(ops::sap::ResultInt::RESULT_INT_OK);
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



    ert::JsonSaxConsumer consumer(2);
    bool success = nlohmann::json::sax_parse(outstring, &consumer);

    if (!success)
        std::cerr << "Conversion error !" << std::endl;

    // output xml
    std::string xmlString {consumer.getXmlString()};
    std::cout << xmlString;
    std::cout << "\n\n";
    //Doesn't work. Bad xml
    //xml2json(xmlString.data());
    xmlString = "<RESULT>\n"
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
//                "     Petya byl golodniy.\n"
                "  </ERROR_TEXT>\n"
                "</RESULT>";

    std::cout << xmlString << "\n\n";
    char* buf = new char[xmlString.size()];
    xmlString.copy(buf,xmlString.size(), 0);
    outstring.clear();
    outstring = xml2json(buf);
    std::cout << outstring << std::endl;

    xmlString.copy(buf,xmlString.size(), 0);
    rapidxml::xml_document<> packet;
    packet.parse<0>(xmlString.data());
    std::cout << packet << "\n\n";
//    rapidxml::print(std::cout,packet);

    rapidxml::xml_node<> *node;
    std::vector<rapidxml::xml_node<>*> nodeStack;
    std::string ident {""};
    ident.reserve(16);

    node = static_cast<rapidxml::xml_node<>*>(&packet);
    nodeStack.push_back(node);

    while (node){
        nodeStack.pop_back();
        if(node->type() == rapidxml::node_element)
        {
            if (node->prefix()) { std::cout << node->prefix() << "."; }
            if (node->name()) { std::cout << node->name() << ": "; }
            if (node->value()) { std::cout << node->value() << std::endl; }
        }

        if (node->first_node()){
        for (auto child =  node->last_node() ; child; child = child->previous_sibling())
        {
            nodeStack.push_back(child);
        }}
            node = nodeStack.empty() ? nullptr : nodeStack.back();
    }


    delete[] buf;
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
