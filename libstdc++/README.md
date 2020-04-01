# 在Linux系统下添加动态库

在运行`server`或者`client`时需要``GLIBCXX_3.4.21``和``GLIBCXX_3.4.22``动态运行库



可能有些环境不匹配，先按以下教程添加运行库文件(`libstdc++.so.6.0.22`)



首先你的报错可能是这样的：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200401164120319.png)

我们需要记录下这个目录，因为每个人可能是不一样的(比如我这里是`/urs/lib/x86_64-linux-gnu`)



我们需要做的就是把原来的`libstdc++.so.6`的内容替换成我们的`libstdc++.so.6.0.22`



所以步骤如下：

1、将原来的`libstdc++.so.6`进行备份

2、将`libstdc++.so.6.0.22`复制到对应的目录下

3、将原来的`libstdc++.so.6`删除

4、使用链接使得`libstdc++.so.6`指向`libstdc++.so.6.0.22`



下面是一个自动部署脚本(`deploy.sh`)，仅供参考

```bash
#! /bin/bash
sudo cp $1/libstdc++.so.6 $1/libstdc++.so.6_backup 
sudo cp ./libstdc++.so.6.0.22 $1
cd $1
sudo rm libstdc++.so.6 
sudo ln -s libstdc++.so.6.0.22 libstdc++.so.6
```

运行时需要`sudo`权限，并且需要一个参数，就是刚才动态库所在目录，使用示例：

```bash
sudo bash deploy.sh /urs/lib/x86_64-linux-gnu
```

`.sh`脚本和`libstdc++.so.6.0.22`需要在同一目录下