#!/bin/bash

SRC_DIR=`dirname $(realpath $0)`
BACK_END_PATH="${SRC_DIR}/pa_rpc/gen_code"
FRONT_END_PATH="${SRC_DIR}/pa_front/public/gen_code"

rm -rf "${BACK_END_PATH}"
mkdir "${BACK_END_PATH}"
rm -rf "${FRONT_END_PATH}"
mkdir "${FRONT_END_PATH}"

thrift -out "${BACK_END_PATH}" --gen cpp:no_skeleton "${SRC_DIR}/idl.thrift"
thrift -out "${FRONT_END_PATH}" --gen js:es6 "${SRC_DIR}/idl.thrift"