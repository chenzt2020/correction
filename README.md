需要OpenCV环境

### Windows平台编译

1. 运行build.bat (需要安装CMake3.6以上+VS2019)

2. 打开VS2019工程，配置好OpenCV依赖，用Release x64选项编译运行

### Linux平台编译
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```