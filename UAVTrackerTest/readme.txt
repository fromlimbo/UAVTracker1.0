****************************************************************
UAVTraccker1.0
特性：基于TLD框架，跟踪器使用KCF算法，加入Kalman滤波提高实时性

开发工具：VS2013，Opencv3.1

使用方法：对于没有安装vs的pc，需要安装vc++x_32
使用方法：将database.txt,parameter.yml放入exe所在的文件夹中，运行即可

数据集修改方法：
将视频放入exe所在的文件夹中，在database文本文件中添加，格式为：视频名称，第一帧目标框的左上角横坐标，第一帧目标框的左上角纵坐标，第一帧目标框的宽度，第一帧目标框的高度。

编译方法：
1:安装了VS2013的用户，可以直接打开sln文件
2：打开不了，可以自己编译，所需的OPENCV3.1头文件放在include文件夹中，所需的Lib文件放在lib文件夹中，其中UAVTrackerd.lib为debug模式所需的lib文件，UAVTracker.lib为release模式所需的lib文件，其余的为Opencv的lib文件，debug、release所需的dll文件分别放在Debug,Release文件夹中

*******************************************************************