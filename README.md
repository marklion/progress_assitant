# progress_assitant
[![Build Status](https://www.travis-ci.com/marklion/progress_assitant.svg?branch=main)](https://www.travis-ci.com/marklion/progress_assitant)

> 需要完善

## 编码环境搭建
```
# 进入源码目录
$ ls
LICENSE             build               depend_docker       gen_thrift.makefile pa_back             pa_front            pa_tool
README.md           build.sh            deploy.sh           idl.thrift          pa_conf             pa_script

# 进入容器目录
$ cd .devcontainer/
$ ls
Dockerfile           devcontainer.json    thrift-0.14.1.tar.gz

# 构建镜像
$ docker build -t pa_deploy:v1.0 .

# 挂载源码目录到容器中并启动shell
$ docker run -ti -v /Users/liuyang/work/progress_assitant:/work pa_deploy:v1.0 bash

# 在源码目录执行build,
# 参数:-b 只构建后端 -f 只构建前端
root@838f41a52599:/# cd /work/
root@838f41a52599:/work# ./build.sh -bf
```

或者直接用vscode打开源码目录，然后安装docker插件并执行在docker容器中打开源码目录，然后即可编码并使用shell构建

项目管理页面：https://d8sis.atlassian.net/wiki/spaces/ZYZLDOC/overview
