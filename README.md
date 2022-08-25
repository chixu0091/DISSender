# DISSender

## Introduction
This project provide a basic C++ implementation for sending DIS(Distributed Interactive Simulation) packets
using Open-dis, SDL2 and SDL2_net. 

这个项目是一个DIS分布式交互仿真的C++实现，用到了Open-dis以及SDL2相关库

## Architecture
Connection.h : some-SDL-udp-API Wrapper  
DISData.h : basic container for DIS information, each DISData holds an DIS::EntityStatePdu ptr  
DataManager.h : parser file and initial DISData for each single file under path  
Logging.h : as simple as its name  
Sender_main.cpp : entry of the program  

Connection.h : 用SDL udp相关的API的封装  
DISData.h : 一个DIS信息的基础容器， 每个实例都存有一个DIS::EntityStatePdu指针  
DataManager.h : 读取文件并且给每个单独文件创建一个DISData类  
Logging.h : 日志相关辅助函数  
Sender_main.cpp : 程序入口  

## TODO
* Use SDL Timer ADD to set the interval (i.e 2s) of packet sending  
  设置时间间隔去发送包裹  
* Update each file's DISData based on saved raw data  
  根据已经读取保存的信息更新每个单独文件的DISData  
