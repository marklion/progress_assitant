FROM ubuntu

RUN apt-get update 
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y nginx git cmake make g++ wget
ADD ngrest_install.sh /root/software/
ADD protobuf-cpp-3.13.0.tar.gz /root/software/
ADD nginx_websocket_install.sh /root/software/
WORKDIR /root/software
WORKDIR /root/software/protobuf-3.13.0
RUN ./configure --prefix=/usr && make -j 6 && make install
WORKDIR /root/software
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y libpcre3 libpcre3-dev zlib1g-dev openssl libssl-dev libxml2 libxml2-dev libxslt-dev libgd-dev uuid-dev
RUN ./nginx_websocket_install.sh
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y valgrind sqlite3 libsqlite3-dev
RUN WITH_THREAD_LOCK=1 ./ngrest_install.sh
ADD libngrestjson.so /root/.ngrest/ngrest-build/deploy/lib/