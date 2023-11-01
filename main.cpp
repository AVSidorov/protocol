#include <iostream>
#include <string>
#include "src/proto/test.pb.h"
#include <google/protobuf/util/json_util.h>


int main() {
    std::string outstring;
    ops::sap::Result message;
    message.set_result_int(ops::sap::ResultInt::OK);

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
    ops::sap::Result_Results_Value value;
    value.set_name("NewValue");
    value.set_value("AX32.32");
    std::cout << value.DebugString() << std::endl;
    message.mutable_results()->add_values()->set_name("NewValue1");
    message.mutable_results()->mutable_values(message.results().values_size()-1)->set_value("asdf");
    message.mutable_results()->add_values()->CopyFrom(value);
    message.set_result_int(ops::sap::ResultInt::OK);
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
    std::cout << message.DebugString() << std::endl;

    google::protobuf::ShutdownProtobufLibrary();
    return 0;

}
