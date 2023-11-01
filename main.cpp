#include <iostream>
#include <string>
#include "src/proto/test.pb.h"
#include <google/protobuf/util/json_util.h>


int main() {
    std::cout << "Hello, World!" << std::endl;
    ops::sap::Result message;
    ops::sap::DataType dataType;
    dataType.set_type("ResultInt");
    std::string outstring;
    google::protobuf::util::MessageToJsonString(message, &outstring);
    std::cout << outstring << std::endl;
    outstring.clear();
    google::protobuf::util::MessageToJsonString(dataType, &outstring);
    std::cout << outstring << std::endl;
    return 0;
}
