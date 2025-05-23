# 星闪代码编译

##  插件下载

| 工具                                                         | 下载链接                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| git（如果使用ZIP格式下载，可以不按照git，详情参考星闪代码下载方式二） | [Git下载链接](https://git-scm.com/downloads/win)             |
| CH341SER驱动                                                 | [CH340G驱动下载链接](https://www.wch.cn/downloads/CH341SER_EXE.html) |

## 星闪代码下载

- 下载SDK代码，目前提供两种方式，第一种方式如下:

  - 第一种方式通过git指令下载（在Windows上使用git指令下载代码，需要提前安装git，  [Git下载链接地址：https://git-scm.com/downloads/win](https://git-scm.com/downloads/win)），在xx盘鼠标右键选择“Open Git Bash here”，如下图所所示（**建议在D盘、E盘根目录执行**）

  ![image-20250307160531260](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20250307160531260.png)

  - 在命令框中输入`git clone https://gitee.com/HiSpark/fbb_ws63.git`指令下载代码，等待下载完成。

![image-20240807151920249](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240807151920249.png)

​			![image-20240801103855026](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801103855026.png)

- 第二种方式ZIP下载方式如下：

  -  [SDK包下载地址：https://gitee.com/HiSpark/fbb_ws63](https://gitee.com/HiSpark/fbb_ws63)，在主界面上点击“克隆/下载”，选择“下载ZIP”，等待下载，下载完成如下。

    ![image-20250307161452197](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20250307161452197.png)

  
  * 解压“fbb_ws63_master”，**在解压过程中需要关闭电脑杀毒软件**（**防止有些文件当做病毒被删除掉**），解压方式选择“Extract Here”即可（**建议解压到D盘、E盘等根目录，路径不要太深，且不要有中文路径**）
  
  ![image-20240801103718350](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801103718350.png)
  
## 新建工程

* 代码下载完成后，打开HiSpark studio工具，点击“新建工程”。

  ![image-20240801104223178](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801104223178.png)

* 新建工程界面中芯片：“WS63”，工程名：“xxx”（用户自定义，但是不能带中文，特殊符号），软件包：“xxx/fbb_ws63/src”（**SDK软件包存放路径，这个地方一定要选到src层级，否则编译会失败**），配置选择完成后，点击“完成按钮”。

  ![image-20240801112357075](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801112357075.png)

* 点击“build”或者“rebuild”编译（**编译建议关闭杀毒软件，不关可能会导致编译时间长**）

  ![image-20240801112427220](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801112427220.png)

* 编译完成如下图所示。

  ![image-20250307164622717](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20250307164622717.png)

  ## 镜像烧录

- 硬件搭建：Typec线将板端与PC端连接

  ![image-20240801173105245](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801173105245.png)

- 安装驱动“CH341SER驱动”（[CH341SER驱动下载地址](https://www.wch.cn/downloads/CH341SER_EXE.html)，**如果该链接失效或者无法下载，用户自行百度下载即可**），安装CH341SER驱动，安装前单板需要与PC端相连，点击安装即可，显示**驱动安装成功代表成功**，如果出现**驱动预安装成功代表安装失败**

    ![image-20240801173439645](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801173439645.png)

    ![image-20240801173618611](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801173618611.png)

- 安装成功后，在HiSpark Studio工具中点击“工程配置”按钮，选择“程序加载”，传输方式选择“serial”，端口选择“comxxx”，com口在设备管理器中查看。

    ![image-20250317173145978](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20250317173145978.png)

- 配置完成后，点击工具“程序加载”按钮烧录。

    ![image-20240801174117545](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801174117545.png)

- 出现“Connecting, please reset device...”字样时，复位开发板，等待烧录结束。

    ![image-20240801174230202](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20240801174230202.png)

-  在HiSpark Studio底端选择“监视器”，选择端口（**开发板需要与电脑通过typec连接**），如果没有端口显示可以刷新一下，点击“开始监视”，复位开发板，出现“flashboot version”字样代表编译烧录成功

  ![image-20250307171224611](../vendor/HiHope_NearLink_DK_WS63E_V03/doc/media/tools/image-20250307171224611.png)

## FAQ
-  如果根据文档没有编译成功，请参考https://developers.hisilicon.com/postDetail?tid=02110170392979486020
-  如果根据文档编译成功，但是在编写其他代码后，导致编译失败，可以在论坛提问，论坛链接：https://developers.hisilicon.com/forum/0133146886267870001


​    

  
