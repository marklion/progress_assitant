# progress_assitant
[![Build Status](https://www.travis-ci.com/marklion/progress_assitant.svg?branch=main)](https://www.travis-ci.com/marklion/progress_assitant)

> 需要完善

## 编码环境搭建
```
# 拉取开发环境和运行环境
$ docker pull marklion/pa_dev:v1.0
$ docker pull marklion/pa_deploy:v1.0

# 重新tag镜像
$ docker tag marklion/pa_dev:v1.0 pa_dev:v1.0
$ docker tag marklion/pa_deploy:v1.0 pa_deploy:v1.0


# 挂载源码目录到容器中并启动shell
$ docker run -ti -v /Users/liuyang/work/progress_assitant:/work pa_dev:v1.0 bash

# 在源码目录执行build,
# 参数:-b 只构建后端 -f 只构建前端
root@838f41a52599:/# cd /work/
root@838f41a52599:/work# ./build.sh -bf

# 在容器外执行install.sh启动项目
$ ./install.sh -p xxx
```

或者直接用vscode打开源码目录，然后安装docker插件并执行在docker容器中打开源码目录，然后即可编码并使用shell构建

项目管理页面：https://d8sis.atlassian.net/wiki/spaces/ZYZLDOC/overview
