本程序使用codeblocks-16.01mingw编译器进行编译运行，codeblocks-16.01mingw的下载地址：https://sourceforge.net/projects/codeblocks/files/Binaries/16.01/Windows/codeblocks-16.01mingw-setup.exe/download
源代码全部存于yuanma文件夹中，输入数据位于case_example文件夹中的data*.txt文件，输出结果位于case_example文件夹中的result*.txt文件
对于yuanma文件中的文件描述如下：|——lib_io.h         读写文件的头文件   |     |——lib_time.h       打印时间的头文件    |   
|——io.cpp                 提供读写文件等功能的源文件  |    |── cdn.cpp                main函数源文件，若要运行程序，需把其中的数据读入文件和结果数据文件的路径改为你自己的路径  |    |── deploy.cpp             设计模型算法的源文件
    │    |── deploy.h       设计模型算法的头文件
   ——|  
程序使用：建立一个空项目，把yuanma文件中的文件全部导入空项目，把cdn.cpp中的数据读入文件和结果数据文件的路径改为data*.txt和result*.txt所在路径，然后编译运行程序即可

.exe文件位于\ZHONGXIN\ZHONGXIN\bin\Debug