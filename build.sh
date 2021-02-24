#!/bin/bash

COMPS=("tcp_framework" "sqlite_orm" "pa_lib" "pa_rest" "pa_daemon")
SRC_DIR=`dirname $(realpath $0)`

BUILD_DIR=${1:-build}
BUILD_DIR=$(realpath $BUILD_DIR)

[ -d ${BUILD_DIR} ] || mkdir -p "${BUILD_DIR}"

for SUB_FOLDER in ${COMPS[*]}
do
    COMP_NAME=${SUB_FOLDER}
    SUB_FOLDER="$SRC_DIR/${SUB_FOLDER}"
    [ -d ${BUILD_DIR}/${COMP_NAME} ] || mkdir $BUILD_DIR/$COMP_NAME
    pushd $BUILD_DIR/$COMP_NAME
    [ -f Makefile ] || cmake $SUB_FOLDER -D PRJ_INTERNAL_BUILD=${BUILD_DIR}

    [ $? == "0" ] && make || exit -1
    popd
done


[ -d ${BUILD_DIR}/conf ] || mkdir ${BUILD_DIR}/conf
cp ${SRC_DIR}/nginx_conf/nginx.conf ${BUILD_DIR}/conf

pushd ${SRC_DIR}/pa_front
[ -d node_modules ] || npm install 
npm run build
mkdir ./dist/logo_res
cp ./dist -a ${BUILD_DIR}/
popd

tar zcf pa_deliver.tar.gz -C ${BUILD_DIR} bin lib conf dist 
cat deploy.sh pa_deliver.tar.gz > install.sh
chmod +x install.sh
rm pa_deliver.tar.gz

