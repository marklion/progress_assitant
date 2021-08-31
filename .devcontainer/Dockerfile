# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.191.1/containers/ubuntu/.devcontainer/base.Dockerfile

# [Choice] Ubuntu version: bionic, focal
ARG VARIANT="focal"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# [Optional] Uncomment this section to install additional OS packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y cmake wget libpython3-dev npm sqlite3 libsqlite3-dev uuid uuid-dev libcurl4-openssl-dev automake bison flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make pkg-config
RUN npm install n -g
RUN n stable
RUN npm install pkg -g
WORKDIR /root/
RUN mkdir thrift_dir
COPY thrift-0.14.1.tar.gz ./thrift_dir/
SHELL ["/bin/bash", "-c"]
RUN pushd thrift_dir;tar xf thrift-0.14.1.tar.gz;pushd thrift-0.14.1 ;./configure --prefix=/usr --without-ruby --without-java --without-go --without-erlang --without-perl --without-qt5 --without-lua --without-php --without-nodejs --without-php_extension && make -j 8 && make install;popd;popd

