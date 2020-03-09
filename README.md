# C++ 简单的socket通信 面向对象

## Server 类
   
   服务器端
   
   定义了Socket(),Bind(),Listen(),Handle(),start(),stop()6个类方法，分别用来进行Socket初始化，绑定，监听，启动和停止。
   
   其中`Handel`是Server进行的动作，我这里设计的是显示客户端发来的数据，需要更改的可以在此方法中进行更改可以扩充

## Client 类
   
   客户端
   
   定义了Socket(),Connet(),handle(),start(),stop()5个类方法，分别用来进行Socket初始化以及连接，启动和停止
   
   同样的`handel`是Clien的动作，这里是向服务器发送一段话，需要更改的可以在此方法中进行更改可以扩充

## 使用
   
   - 1、在主目录下使用 `make`,来进行编译
   
   - 2、先使用`./server` 来开始服务器程序
   
   - 3、再使用 `./client <ip address>`来启动客户端程序，其中`<ip address>`是ip地址，如果是本地，请填写`127.0.0.1`
   
