#!/bin/bash
export NODE_OPTIONS=--openssl-legacy-provider
COMPS=("tcp_framework" "sqlite_orm" "external_src" "pa_util" "pa_rpc" "pa_automation")
SRC_DIR=`dirname $(realpath $0)`
BACK_SRC_DIR="${SRC_DIR}/pa_back"

BUILD_DIR="${SRC_DIR}/build"

BUILD_MODE='test-build'
BUILD_BACK_END='n'
BUILD_FRONT_END='n'


while getopts "fbo:m:" arg
do
    case $arg in
        f)
            BUILD_FRONT_END='y'
            ;;
        b)
            BUILD_BACK_END='y'
            ;;
        o)
            BUILD_DIR=${OPTARG}
            ;;
        m)
            BUILD_MODE=${OPTARG}
            ;;
        *)
            echo "invalid args"
            ;;
    esac
done
BUILD_DIR=$(realpath $BUILD_DIR)

[ -d ${BUILD_DIR} ] || mkdir -p "${BUILD_DIR}"

make -f ${SRC_DIR}/gen_thrift.makefile

if [ ${BUILD_BACK_END} == 'y' ]
then
    for SUB_FOLDER in ${COMPS[*]}
    do
        COMP_NAME=${SUB_FOLDER}
        SUB_FOLDER="$BACK_SRC_DIR/${SUB_FOLDER}"
        [ -d ${BUILD_DIR}/${COMP_NAME} ] || mkdir $BUILD_DIR/$COMP_NAME
        pushd $BUILD_DIR/$COMP_NAME
        [ -f Makefile ] || cmake -D PRJ_INTERNAL_BUILD=${BUILD_DIR} $SUB_FOLDER

        [ $? == "0" ] && make -j 8 || exit -1
        popd
    done
    cp ${BACK_SRC_DIR}/pa_rest_node -a ${BUILD_DIR}/
    pushd ${BUILD_DIR}/pa_rest_node
    npm install
    popd
fi

[ -d ${BUILD_DIR}/conf ] || mkdir ${BUILD_DIR}/conf
cp ${SRC_DIR}/pa_conf/nginx.conf ${BUILD_DIR}/conf

[ -d ${BUILD_DIR}/script ] || mkdir ${BUILD_DIR}/script
cp -a ${SRC_DIR}/pa_script/* ${BUILD_DIR}/script/
chmod +x ${BUILD_DIR}/script/*

if [ ${BUILD_FRONT_END} == 'y' ]
then
    pushd ${SRC_DIR}/pa_front
    [ -d node_modules ] || npm install
    npm run ${BUILD_MODE}
    mkdir ./dist/logo_res
    mkdir ./dist/company_logo
    cp ${SRC_DIR}/pa_conf/company_logo/* ./dist/company_logo/
    cp ./dist -a ${BUILD_DIR}/
    popd
fi

pushd ${SRC_DIR}/pa_tool/pa_api_cli
npm install
mkdir ${BUILD_DIR}/dist/tools
pkg --out-path ${BUILD_DIR}/dist/tools pa_api_cli.js
popd

tar zcf pa_deliver.tar.gz -C ${BUILD_DIR} bin lib conf dist script pa_rest_node
cat ${SRC_DIR}/deploy.sh pa_deliver.tar.gz > ${BUILD_DIR}/install.sh
chmod +x ${BUILD_DIR}/install.sh
rm pa_deliver.tar.gz

