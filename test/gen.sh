#!/bin/bash
# assumes .deb package requirements:
#  protobuf-compiler python-protobuf protobuf-c-compiler libprotoc-dev
SRC_DIR=./src/proto
DST_DIR=../gen

if [[ ! -d $DST_DIR ]];then
  mkdir $DST_DIR
fi

if [[ ! -d $DST_DIR/cpp ]];then
  mkdir $DST_DIR/cpp
fi

if [[ ! -d $DST_DIR/py ]];then
  mkdir $DST_DIR/py
fi

for protofile in $SRC_DIR/*.proto; do
    protoc -I=$SRC_DIR --cpp_out=$DST_DIR/cpp --python_out=$DST_DIR/py $protofile
done
