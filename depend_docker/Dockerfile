FROM ubuntu

RUN  apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y nginx git cmake make g++ wget libpcre3 libpcre3-dev zlib1g-dev openssl libssl-dev libxml2 libxml2-dev libxslt-dev libgd-dev uuid-dev valgrind sqlite3 libsqlite3-dev python3 python3-dev python3-pip
RUN pip3 install pandas
RUN pip3 install openpyxl