# Sever  
**<font size=4>概述</font>**   
<font size=4>Server 是一个由C++实现的简单http服务器，支持GET/POST请求、http 长短连接，能从浏览器请求服务器上的图片和视频文件。</font>  

****************************************************************

**<font size=4>技术点</font>**   
- <font size=4>使用线程池+非阻塞IO+epoll多路复用+Reactor事件处理的并发模型</font>
- <font size=4>使用小根堆管理定时器处理非活动连接</font>
- <font size=4>使用对象池减少内存申请释放开销</font>
- <font size=4>支持GET/POST请求</font>  
- <font size=4>实现同步日志记录状态</font>  

*****************************************************************

**<font size=4> 环境 </font>**  
- <font size=4>Ubuntu 22.04 </font>  
- <font size=4>处理器核数：8 </font>  
- <font size=4> gcc版本:11.04 </font>  
******************************************************************
**<font size=4>webbench 测试</font>**
![压力测试](https://github.com/xyyang0/server/blob/main/res/webbench.jpg)

*****************************************************************

**<font size=4>运行</font>**   
- <font size=4>mkdir build</font>  
- <font size=4>make build</font>  
- <font size=4>./build/sever [-l [0|1]]</font>
- <font size=4>端口号为12345</font>

*****************************************************************

**<font size=4>ToDo</font>**
- [ ] <font size=4>实现客户端登陆和注册</font>
- [x] <font size=4>实现日志记录服务器运行状态</font>
- [ ] <font size=4>重构代码，优化运行效率</font>

  
