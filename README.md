# DISSender

## Introduction
This project provide a basic C++ implementation for sending DIS(Distributed Interactive Simulation) packets
using Open-dis, SDL2 and SDL2_net.  

这是一个DIS(分布式交换仿真)发包软件的C++实现，用到了Open-dis以及SDL2和SDL2_net相关库  
Platform : Windows 10  
Visual Studio Version : 2022  

## Download
`git clone https://github.com/chixu0091/DISSender.git`  

## Compile and Run
open vs sln file and build  
打开sln文件编译  

The running format is `./DISSender.exe [filepath] [time_interval] [max_sendings]`  
which filepath is path of raw data to get DIS information, time interval is in ms to send  
between each update iteration and max sendings defined the maximum number of iterations.  
Example: `./DISSender.exe ./testdata/ 1000 10`(This is default parms so `./DISSender.exe` works)  

基础的执行命令格式为 `./DISSender.exe [filepath] [time_interval] [max_sendings]`  
filepath是读取的文件path，time interval是间隔发送的时间毫秒，max sendings是最大发包轮次  
例子: `./DISSender.exe ./testdata/ 1000 10`(这是缺省值所以直接用 `./DISSender.exe`也可以)  


## Architecture
Connection.h : some-SDL-udp-API Wrapper  
DISData.h : basic container for DIS information, each DISData holds an DIS::EntityStatePdu ptr  
DataManager.h : parser file and initial DISData for each single file under path  
Logging.h : as simple as its name  
Sender_main.cpp : entry of the program  
COMMON.h : some useful global variables and functions  

Connection.h : 用SDL udp相关的API的封装  
DISData.h : 一个DIS信息的基础容器， 每个实例都存有一个DIS::EntityStatePdu指针  
DataManager.h : 读取文件并且给每个单独文件创建一个DISData类  
Logging.h : 日志相关辅助函数  
Sender_main.cpp : 程序入口  
COMMON.h : 全局变量和函数  

## TODO
* ~~Use SDL Timer ADD to set the interval (i.e 2s) of packet sending~~  
  ~~设置时间间隔去发送包裹~~  
* ~~Update each file's DISData based on saved raw data~~  
  ~~根据已经读取保存的信息更新每个单独文件的DISData~~  
* Update logging as c-printf style  
  更新LOG系统像c-printf一样  
