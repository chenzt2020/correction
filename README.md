需要OpenCV环境

1.Windows平台编译
    
新建vs2019工程，导入全部源文件，然后配置好依赖，用Release x64选项编译运行

也可用vs2019+cmake编译

2.Linux平台编译
```sh
    cmake .
    cd build
    make
```