dsDBS鼎石数据系统
	鼎石数据系统是在取得zsDDSR.dll源码基础上进行的再次开发,且在原有基础上改动较大，
故版本号从1.8开始。
	*版本：1.8
	*作者：郝多年
	*微信：19843355855
编译说明：
    vs2010  + google protobuf 2.6版本

目录说明
dsBase		    鼎石基类
dsDBS		    鼎石核心功能实现	
dsDBSExp	    鼎石数据服务端示例
dsExpClnt	    客户端模拟程序，向dsDBSExp发请求消息（select、insert、update、delete）
Include		    接口头文件
Lib		        库文件	
Debug		    编译输出目录
dsExpPB         Protobuf协议客户端模拟程序，向dsDBSExp发请求消息（select、insert、update、delete）
dsPB            ProtoBuf协议接入示例动态库

dsBase.dll	    鼎石基类,接口虚类
dsDBS.dll	    实现类 odbc,byte1，动态加载dsPB.dll、dsMySql.dll及基它扩展
dsPB.dll	    google protobuf协议例子
dsMySql.dll	    mysql数据库接口例子

