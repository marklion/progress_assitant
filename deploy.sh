#!/bin/bash

PA_DELIVER="pa_deliver.tar.gz"
WECHAT_SECRET_INPUT="none"
WECHAT_MP_SECRET_INPUT="none"
ALI_KEY_ID_INPUT="none"
ALI_KEY_SEC_INPUT="none"
MAIL_PWD_INPUT="none"
PORT=80
DATA_BASE="pa.db"
IMG_BED_INPUT="logo_res"
CONF_FILE_INPUT="data_config.json"

DOCKER_IMG_NAME="pa_deploy:v1.0"
SRC_DIR=`dirname $(realpath $0)`/../
is_in_container() {
    cat /proc/1/cgroup | grep pids | grep docker 2>&1>/dev/null
}

make_docker_img_from_dockerfile() {
    docker build -t ${DOCKER_IMG_NAME} ${SRC_DIR}/depend_docker
}

get_docker_image() {
    docker images ${DOCKER_IMG_NAME} | grep pa_deploy > /dev/null
    if [ $? != 0 ]
    then
        make_docker_img_from_dockerfile
    fi
}

start_all_server() {
    line=`wc -l $0|awk '{print $1}'`
    line=`expr $line - 106` 
    tail -n $line $0 | tar zx  --skip-old-files -C /
    mv /data_config.json /conf/
    nginx -c /conf/nginx.conf
    /script/auto_reply.py &
    pa_rpc &
    node /pa_rest_node/index.js &
    bash
}

start_docker_con() {
    local DATA_BASE_PATH=`realpath $DATA_BASE`
    local DATA_BASE_PATH=`dirname ${DATA_BASE_PATH}`
    local IMG_BED=`realpath $IMG_BED_INPUT`
    local CON_ID=`docker create -ti --rm -p ${PORT}:80 -e WECHAT_SECRET="${WECHAT_SECRET_INPUT}" -e WECHAT_MP_SECRET="${WECHAT_MP_SECRET_INPUT}" -e ALI_KEY_ID="${ALI_KEY_ID_INPUT}" -e ALI_KEY_SEC="${ALI_KEY_SEC_INPUT}" -e MAIL_PWD="${MAIL_PWD_INPUT}" -v ${DATA_BASE_PATH}:/database -v ${IMG_BED}:/dist/logo_res ${DOCKER_IMG_NAME} /root/install.sh`
    docker cp $0 ${CON_ID}:/root/
    docker cp /etc/localtime ${CON_ID}:/etc/localtime
    docker cp /etc/timezone ${CON_ID}:/etc/timezone
    docker cp ${CONF_FILE_INPUT} ${CON_ID}:/data_config.json
    docker start -ai ${CON_ID}
}

while getopts "D:p:w:d:i:m:a:k:M:c:" arg
do
    case $arg in
        D)
            PA_DELIVER=${OPTARG}
            ;;
        p)
            PORT=${OPTARG}
            ;;
        w)
            WECHAT_SECRET_INPUT=${OPTARG}
            ;;
        m)
            WECHAT_MP_SECRET_INPUT=${OPTARG}
            ;;
        d)
            DATA_BASE=${OPTARG}
            ;;
        i)
            IMG_BED_INPUT=${OPTARG}
            ;;
        a)
            ALI_KEY_ID_INPUT=${OPTARG}
            ;;
        k)
            ALI_KEY_SEC_INPUT=${OPTARG}
            ;;
        M)
            MAIL_PWD_INPUT=${OPTARG}
            ;;
        c)
            CONF_FILE_INPUT=${OPTARG}
            ;;
        *)
            echo "invalid args"
            ;;
    esac
done

if is_in_container 
then
    start_all_server
else
    get_docker_image
    start_docker_con
fi

#
exit



