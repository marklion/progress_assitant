FROM ubuntu

RUN  apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y nginx git cmake make g++ wget libpcre3 libpcre3-dev zlib1g-dev openssl libssl-dev libxml2 libxml2-dev libxslt-dev libgd-dev uuid-dev valgrind sqlite3 libsqlite3-dev python3 python3-dev python3-pip
RUN pip3 install pandas
RUN pip3 install openpyxl
RUN pip3 install alibabacloud_dysmsapi20170525==2.0.2
RUN  apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y imagemagick
RUN sed -i '/domain="resource" name="height"/d' /etc/ImageMagick-6/policy.xml
RUN sed -i '/domain="resource" name="width"/d' /etc/ImageMagick-6/policy.xml
COPY mutool /bin/
RUN pip3 install werobot
RUN pip3 install thrift 
RUN  apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y nodejs