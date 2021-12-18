-- MySQL dump 10.13  Distrib 8.0.23, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: ds_sys_db
-- ------------------------------------------------------
-- Server version	8.0.23

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `ds_sys_const`
--

DROP TABLE IF EXISTS `ds_sys_const`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_const` (
  `ConstID` int NOT NULL,
  `ConstType` int DEFAULT '0',
  `ConstValue` int DEFAULT NULL,
  `ConstName` varchar(256) DEFAULT NULL,
  `Comment` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`ConstID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_const`
--

LOCK TABLES `ds_sys_const` WRITE;
/*!40000 ALTER TABLE `ds_sys_const` DISABLE KEYS */;
INSERT INTO `ds_sys_const` VALUES (1,0,0,'DS_DATATYPE_CHAR','字符'),(2,0,1,'DS_DATATYPE_BYTE','字节'),(3,0,2,'DS_DATATYPE_SWORD','有符号字'),(4,0,3,'DS_DATATYPE_WORD','无符号字'),(5,0,4,'DS_DATATYPE_INT','有符号整数'),(6,0,5,'DS_DATATYPE_UINT','无符号整数'),(7,0,9,'DS_DATATYPE_LONGLONG','有符号长整数'),(8,0,9,'DS_DATATYPE_ULONGLONG','无符号长整数'),(9,0,6,'DS_DATATYPE_STR','字符串'),(10,0,7,'DS_DATATYPE_BINARY','二进制数据'),(11,0,10,'DS_DATATYPE_FLOAT','float'),(12,0,15,'DS_DATATYPE_DOUBLE','double'),(13,0,31,'DS_DATATYPE_SUFFIX','后缀数据，两个整形');
/*!40000 ALTER TABLE `ds_sys_const` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_dbsvrinfo`
--

DROP TABLE IF EXISTS `ds_sys_dbsvrinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_dbsvrinfo` (
  `DSDBSID` int NOT NULL,
  `DBType` int DEFAULT NULL,
  `DBName` varchar(128) DEFAULT NULL,
  `DSNName` varchar(128) DEFAULT NULL,
  `IP` varchar(128) DEFAULT NULL,
  `Port` int DEFAULT NULL,
  `ConnectCount` int DEFAULT NULL,
  `DBUserName` varchar(128) DEFAULT NULL,
  `DBPassword` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`DSDBSID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_dbsvrinfo`
--

LOCK TABLES `ds_sys_dbsvrinfo` WRITE;
/*!40000 ALTER TABLE `ds_sys_dbsvrinfo` DISABLE KEYS */;
INSERT INTO `ds_sys_dbsvrinfo` VALUES (1,10001,'dssysdb','dssysdb','127.0.0.1',3306,10,'dsuser','dspass123');
/*!40000 ALTER TABLE `ds_sys_dbsvrinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_msg`
--

DROP TABLE IF EXISTS `ds_sys_msg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_msg` (
  `DSMsgID` int NOT NULL,
  `MsgHeadID` int DEFAULT NULL,
  `MsgHeadProto` int DEFAULT NULL,
  `MsgBodyProto` int DEFAULT NULL,
  `HeadLenInMsg` int DEFAULT NULL,
  `SuffixStartPos` int DEFAULT NULL,
  `IsLoginMsg` int DEFAULT NULL,
  `MsgName` varchar(256) DEFAULT NULL,
  `SuffixCount` varchar(128) DEFAULT NULL,
  `SuffixSumLen` varchar(128) DEFAULT NULL,
  `SuffixStructID` int DEFAULT NULL,
  `DSSVRID` int DEFAULT NULL,
  `Comment` varchar(512) DEFAULT '0',
  PRIMARY KEY (`DSMsgID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_msg`
--

LOCK TABLES `ds_sys_msg` WRITE;
/*!40000 ALTER TABLE `ds_sys_msg` DISABLE KEYS */;
INSERT INTO `ds_sys_msg` VALUES (3,0,20001,20001,0,0,0,'Head','','',-1,0,NULL),(10001,3,20001,20001,0,0,0,'ExpInsertReq',NULL,NULL,NULL,0,NULL),(10002,3,20001,20001,0,0,0,'ExpSelectReq',NULL,NULL,NULL,0,NULL),(10003,3,20001,20001,0,0,0,'ExpUpdateReq',NULL,NULL,NULL,0,NULL),(10004,3,20001,20001,0,0,0,'ExpDeleteReq',NULL,NULL,NULL,0,NULL),(20001,3,20001,20001,0,0,0,'ExpInsertAck',NULL,NULL,NULL,0,NULL),(20002,3,20001,20001,0,0,0,'ExpSelectAck','1.-1.0',NULL,50002,0,NULL),(20003,3,20001,20001,0,0,0,'ExpUpdateAck',NULL,NULL,NULL,0,NULL),(20004,3,20001,20001,0,0,0,'ExpDeleteAck',NULL,NULL,NULL,0,NULL),(51001,3,20001,20008,0,NULL,NULL,'InterfaceSelectReq',NULL,NULL,NULL,0,'0'),(51002,3,20001,20008,0,NULL,NULL,'InterfaceSelectAck',NULL,NULL,5101,0,'0'),(51003,3,20001,20008,0,NULL,NULL,'InterfaceInsertReq',NULL,NULL,NULL,0,'0'),(51004,3,20001,20008,0,NULL,NULL,'InterfaceInsertAck',NULL,NULL,NULL,0,'0'),(51005,3,20001,20008,0,NULL,NULL,'InterfaceUpdateReq',NULL,NULL,NULL,0,'0'),(51006,3,20001,20008,0,NULL,NULL,'InterfaceUpdateAck',NULL,NULL,NULL,0,'0'),(51007,3,20001,20008,0,NULL,NULL,'InterfaceDeleteReq',NULL,NULL,NULL,0,'0'),(51008,3,20001,20008,0,NULL,NULL,'InterfaceDeleteAck',NULL,NULL,NULL,0,'0');
/*!40000 ALTER TABLE `ds_sys_msg` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_msgcol`
--

DROP TABLE IF EXISTS `ds_sys_msgcol`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_msgcol` (
  `DSColID` int NOT NULL,
  `DSMsgPlayID` int DEFAULT NULL,
  `ColName` varchar(256) DEFAULT NULL,
  `ColType` int DEFAULT NULL,
  `ColLen` int DEFAULT NULL,
  `AckMember` varchar(256) DEFAULT NULL,
  `ReqMember` varchar(256) DEFAULT NULL,
  `SetAckFlag` int DEFAULT NULL,
  `ColIdx` int DEFAULT NULL,
  `ReqID` int DEFAULT NULL,
  `FmtStr` varchar(256) DEFAULT NULL,
  `DSSVRID` int DEFAULT '0',
  PRIMARY KEY (`DSColID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_msgcol`
--

LOCK TABLES `ds_sys_msgcol` WRITE;
/*!40000 ALTER TABLE `ds_sys_msgcol` DISABLE KEYS */;
INSERT INTO `ds_sys_msgcol` VALUES (101,2,'TestName',6,128,NULL,'1.0.0',NULL,1,10001,'\'%s\'',0),(102,2,'ds_TINYINT',1,NULL,NULL,'2.0.0',NULL,2,10001,NULL,0),(103,2,'ds_SMALLINT',2,NULL,NULL,'2.0.1',NULL,3,10001,NULL,0),(104,2,'ds_INT',4,NULL,NULL,'2.0.2',NULL,4,10001,NULL,0),(105,2,'ds_BIGINT',9,NULL,NULL,'2.0.3',NULL,5,10001,NULL,0),(106,2,'ds_FLOAT',10,NULL,NULL,'2.0.4',NULL,6,10001,NULL,0),(107,2,'ds_DOUBLE',15,NULL,NULL,'2.0.5',NULL,7,10001,NULL,0),(108,2,'ds_name',6,128,NULL,'2.0.6',NULL,8,10001,'\'%s\'',0),(109,2,'ds_SUFFIX',6,128,NULL,'2.0.7',NULL,9,10001,'\'%s\'',0),(201,3,'ds_TINYINT',1,NULL,'-1.0.0','',NULL,2,10002,NULL,0),(202,3,'ds_SMALLINT',2,NULL,'-1.0.1','',NULL,3,10002,NULL,0),(203,3,'ds_BIGINT',9,NULL,'-1.0.3','',NULL,4,10002,NULL,0),(204,3,'ds_FLOAT',10,NULL,'-1.0.4','',NULL,5,10002,NULL,0),(205,3,'ds_DOUBLE',15,NULL,'-1.0.5','',NULL,6,10002,NULL,0),(206,3,'ds_name',6,128,'-1.0.6','',NULL,7,10002,'',0),(207,3,'TestName',6,128,'-1.0.7','',NULL,8,10002,'',0),(306,4,'ds_DOUBLE',15,NULL,NULL,'2.0.4',NULL,6,10003,NULL,0),(561,16,'InterfaceID',4,NULL,'2.0.1',NULL,NULL,1,50001,NULL,0),(562,16,'DSSVRID',4,NULL,'2.0.2',NULL,NULL,2,50001,NULL,0),(563,16,'IFName',6,128,'2.0.3',NULL,NULL,3,50001,NULL,0),(564,16,'IFPath',6,128,'2.0.4',NULL,NULL,4,50001,NULL,0),(565,16,'IsEnable',4,NULL,'2.0.5',NULL,NULL,5,50001,NULL,0),(571,17,'InterfaceID',4,NULL,NULL,'2.0.1',NULL,1,50003,NULL,0),(572,17,'DSSVRID',4,NULL,NULL,'2.0.2',NULL,2,50003,NULL,0),(573,17,'IFName',6,128,NULL,'2.0.3',NULL,3,50003,'\'%s\'',0),(574,17,'IFPath',6,128,NULL,'2.0.4',NULL,4,50003,'\'%s\'',0),(575,17,'IsEnable',4,NULL,NULL,'2.0.5',NULL,5,50003,NULL,0),(581,18,'DSSVRID',4,NULL,NULL,'2.0.2',NULL,1,50005,NULL,0);
/*!40000 ALTER TABLE `ds_sys_msgcol` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_msgmember`
--

DROP TABLE IF EXISTS `ds_sys_msgmember`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_msgmember` (
  `DSMsgMemberID` int NOT NULL,
  `DSMsgID` int DEFAULT NULL,
  `MemberAttribute` varchar(128) DEFAULT NULL,
  `MemberName` varchar(128) DEFAULT NULL,
  `MemberClass` int DEFAULT NULL,
  `MemberType` int DEFAULT NULL,
  `MemberLen` int DEFAULT NULL,
  `MemberOrder` int DEFAULT NULL,
  `DefaultStr` varchar(512) DEFAULT NULL,
  `HeadMemberType` int DEFAULT NULL,
  `Comment` varchar(512) DEFAULT NULL,
  `DSSVRID` int DEFAULT '0',
  PRIMARY KEY (`DSMsgMemberID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_msgmember`
--

LOCK TABLES `ds_sys_msgmember` WRITE;
/*!40000 ALTER TABLE `ds_sys_msgmember` DISABLE KEYS */;
INSERT INTO `ds_sys_msgmember` VALUES (1,5001,NULL,'MsgFlag',1,4,1,0,NULL,NULL,NULL,0),(2,5001,'','MsgID',1,4,1,1,'',1,NULL,0),(3,5001,'','MsgLen',1,4,1,2,'',2,NULL,0),(4,5001,'','MsgSerier',1,4,1,3,'',3,NULL,0),(10,3,'','HeadStruct',0,5001,1,1,'',0,NULL,0),(101,50002,NULL,'CharTypeExp',1,1,1,1,NULL,1,NULL,0),(102,50002,NULL,'ShortTypeExp',1,2,1,2,NULL,1,NULL,0),(103,50002,NULL,'IntTypeExp',1,4,1,3,NULL,1,NULL,0),(104,50002,NULL,'Int64TypeExp',1,9,1,4,NULL,1,NULL,0),(105,50002,NULL,'FloatTypeExp',1,10,1,5,NULL,1,NULL,0),(106,50002,NULL,'DoubleTypeExp',1,15,1,6,NULL,1,NULL,0),(107,50002,NULL,'StringTypeExp',1,6,128,7,NULL,1,NULL,0),(108,50002,NULL,'SuffixTypeExp',1,31,1,8,NULL,1,NULL,0),(1001,10001,NULL,'szTestNameExp',0,6,128,1,NULL,1,NULL,0),(1002,10001,NULL,'StructDataExp',0,50002,1,2,NULL,1,NULL,0),(1006,20001,NULL,'nRetExp',0,4,1,1,NULL,1,NULL,0),(2001,10002,NULL,'szTestNameExp',0,6,128,1,NULL,1,NULL,0),(2002,10002,NULL,'IntTypeExp',0,4,1,2,NULL,1,NULL,0),(2006,20002,NULL,'nRetCountExp',0,4,1,1,NULL,1,NULL,0),(3001,10003,NULL,'szTestNameExp',0,6,128,1,NULL,1,NULL,0),(3002,10003,NULL,'StructDataExp',0,50002,1,2,NULL,1,NULL,0),(3006,20003,NULL,'nRetCount',0,4,1,1,NULL,1,NULL,0),(4001,10004,NULL,'szTestNameExp',0,6,128,1,NULL,1,NULL,0),(4002,10004,NULL,'StructDataExp',0,50002,1,2,NULL,1,NULL,0),(4006,20004,NULL,'nRetCount',0,4,1,1,NULL,1,NULL,0),(5101,5101,'optional','InterfaceID',1,4,1,1,NULL,NULL,NULL,0),(5102,5101,'optional','DSSVRID',1,4,1,2,NULL,NULL,NULL,0),(5103,5101,'optional','IFName',1,6,1,3,NULL,NULL,NULL,0),(5104,5101,'optional','IFPath',1,31,1,4,NULL,NULL,NULL,0),(5105,5101,'optional','IsEnable',1,4,1,5,NULL,NULL,NULL,0),(5106,5101,'optional','DescComment',1,6,1,6,NULL,NULL,NULL,0),(5201,51001,'optional','DSSVRID',0,4,1,2,NULL,NULL,NULL,0),(5202,51002,'repeated','CfgItems',0,5101,1,2,NULL,NULL,NULL,0),(5203,51003,'repeated','CfgItems',0,5101,1,2,NULL,NULL,NULL,0),(5204,51004,'optional','Ret',0,4,1,2,NULL,NULL,NULL,0),(5205,51005,'repeated','CfgItems',0,5101,1,2,NULL,NULL,NULL,0),(5206,51006,'optional','Ret',0,4,1,2,NULL,NULL,NULL,0),(5207,51007,'repeated','CfgItems',0,5101,1,2,NULL,NULL,NULL,0),(5208,51008,'optional','Ret',0,4,1,2,NULL,NULL,NULL,0);
/*!40000 ALTER TABLE `ds_sys_msgmember` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_msgplay`
--

DROP TABLE IF EXISTS `ds_sys_msgplay`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_msgplay` (
  `DSMsgPlayID` int NOT NULL,
  `MsgPlayName` varchar(128) DEFAULT NULL,
  `AckID` int DEFAULT NULL,
  `ReqID` int DEFAULT NULL,
  `DSDBSID` int DEFAULT NULL,
  `TableName` varchar(512) DEFAULT NULL,
  `AddTableName` varchar(256) DEFAULT NULL,
  `AddTableNameMode` int DEFAULT NULL,
  `TableNameSufID` varchar(256) DEFAULT NULL,
  `TableNameSufType` int DEFAULT NULL,
  `SqlOperType` int DEFAULT NULL,
  `WhereStr` varchar(512) DEFAULT NULL,
  `GroupStr` varchar(256) DEFAULT NULL,
  `OrderStr` varchar(256) DEFAULT NULL,
  `RetMember` varchar(256) DEFAULT NULL,
  `DSSVRID` int DEFAULT '0',
  PRIMARY KEY (`DSMsgPlayID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_msgplay`
--

LOCK TABLES `ds_sys_msgplay` WRITE;
/*!40000 ALTER TABLE `ds_sys_msgplay` DISABLE KEYS */;
INSERT INTO `ds_sys_msgplay` VALUES (2,'InsertExp',20001,10001,1,'ds_sys_db.DS_test_type',NULL,NULL,NULL,NULL,2,NULL,NULL,NULL,NULL,0),(3,'SelectExp',20002,10002,1,'ds_sys_db.DS_test_type',NULL,NULL,NULL,NULL,1,'ds_TINYINT=1',NULL,'ds_SMALLINT',NULL,0),(4,'UpdateExp',20003,10003,1,'ds_sys_db.DS_test_type',NULL,NULL,NULL,NULL,3,'ds_TINYINT=1',NULL,NULL,NULL,0),(5,'DeleteExp',20004,10004,1,'ds_sys_db.DS_test_type',NULL,NULL,NULL,NULL,4,'ds_SMALLINT=1007 ',NULL,NULL,NULL,0),(16,'SelectPB',51002,51001,1,'ds_sys_db.ds_sys_sysinterfacecfg',NULL,NULL,NULL,NULL,1,'IsEnable=0',NULL,'InterfaceID',NULL,0),(17,'InsertPB',51004,51003,1,'ds_sys_db.ds_sys_sysinterfacecfg',NULL,NULL,NULL,NULL,2,'',NULL,'',NULL,0),(18,'UpdatePB',51006,51005,1,'ds_sys_db.ds_sys_sysinterfacecfg',NULL,NULL,NULL,NULL,3,'IsEnable=0',NULL,'',NULL,0),(19,'DeletePB',51008,51007,1,'ds_sys_db.ds_sys_sysinterfacecfg',NULL,NULL,NULL,NULL,4,'IsEnable=0',NULL,'',NULL,0);
/*!40000 ALTER TABLE `ds_sys_msgplay` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_msgwhere`
--

DROP TABLE IF EXISTS `ds_sys_msgwhere`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_msgwhere` (
  `DSWhereID` int NOT NULL,
  `DSMsgPlayID` int DEFAULT NULL,
  `WhereStr` varchar(512) DEFAULT NULL,
  `AckMember` varchar(256) DEFAULT NULL,
  `ReqMember` varchar(256) DEFAULT NULL,
  `ColIdx` int DEFAULT NULL,
  `ReqID` int DEFAULT NULL,
  `DSSVRID` int DEFAULT '0',
  PRIMARY KEY (`DSWhereID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_msgwhere`
--

LOCK TABLES `ds_sys_msgwhere` WRITE;
/*!40000 ALTER TABLE `ds_sys_msgwhere` DISABLE KEYS */;
INSERT INTO `ds_sys_msgwhere` VALUES (1,5,'and dsid<%d',NULL,'1.0.0',1,10005,0),(2,16,'and dssvrid= %d',NULL,'2.-1.0',1,50001,0);
/*!40000 ALTER TABLE `ds_sys_msgwhere` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_struct`
--

DROP TABLE IF EXISTS `ds_sys_struct`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_struct` (
  `DSSTID` int NOT NULL,
  `StructName` varchar(128) DEFAULT NULL,
  `DSSVRID` int DEFAULT '0',
  `Comment` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`DSSTID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_struct`
--

LOCK TABLES `ds_sys_struct` WRITE;
/*!40000 ALTER TABLE `ds_sys_struct` DISABLE KEYS */;
INSERT INTO `ds_sys_struct` VALUES (5001,'HeadStruct',0,NULL),(5101,'SysInterfaceStruct',0,NULL),(50002,'Byte1ExpDataStruct',0,NULL);
/*!40000 ALTER TABLE `ds_sys_struct` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_svrinfo`
--

DROP TABLE IF EXISTS `ds_sys_svrinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_svrinfo` (
  `DSSVRID` int NOT NULL,
  `SvrName` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`DSSVRID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_svrinfo`
--

LOCK TABLES `ds_sys_svrinfo` WRITE;
/*!40000 ALTER TABLE `ds_sys_svrinfo` DISABLE KEYS */;
INSERT INTO `ds_sys_svrinfo` VALUES (0,'配置服务'),(234,'TestSvr234');
/*!40000 ALTER TABLE `ds_sys_svrinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_sysinfo`
--

DROP TABLE IF EXISTS `ds_sys_sysinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_sysinfo` (
  `DSSIID` int NOT NULL,
  `DSVersionNum` int DEFAULT NULL,
  PRIMARY KEY (`DSSIID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_sysinfo`
--

LOCK TABLES `ds_sys_sysinfo` WRITE;
/*!40000 ALTER TABLE `ds_sys_sysinfo` DISABLE KEYS */;
/*!40000 ALTER TABLE `ds_sys_sysinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_sysinterfacecfg`
--

DROP TABLE IF EXISTS `ds_sys_sysinterfacecfg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_sysinterfacecfg` (
  `DSICID` int NOT NULL AUTO_INCREMENT,
  `InterfaceID` int DEFAULT NULL,
  `DSSVRID` int DEFAULT NULL,
  `IFName` varchar(128) DEFAULT NULL,
  `IFPath` varchar(256) DEFAULT NULL,
  `IsEnable` int DEFAULT NULL,
  `Comment` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`DSICID`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_sysinterfacecfg`
--

LOCK TABLES `ds_sys_sysinterfacecfg` WRITE;
/*!40000 ALTER TABLE `ds_sys_sysinterfacecfg` DISABLE KEYS */;
INSERT INTO `ds_sys_sysinterfacecfg` VALUES (1,20008,0,'dsPB.dll','..\\Debug\\',1,NULL);
/*!40000 ALTER TABLE `ds_sys_sysinterfacecfg` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_sys_testtable`
--

DROP TABLE IF EXISTS `ds_sys_testtable`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_sys_testtable` (
  `DSICID` int NOT NULL AUTO_INCREMENT,
  `InterfaceID` int DEFAULT NULL,
  `DSSVRID` int DEFAULT NULL,
  `IFName` varchar(128) DEFAULT NULL,
  `IFPath` varchar(256) DEFAULT NULL,
  `IsEnable` int DEFAULT NULL,
  `Comment` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`DSICID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_sys_testtable`
--

LOCK TABLES `ds_sys_testtable` WRITE;
/*!40000 ALTER TABLE `ds_sys_testtable` DISABLE KEYS */;
INSERT INTO `ds_sys_testtable` VALUES (1,1,1,'1','1',1,NULL);
/*!40000 ALTER TABLE `ds_sys_testtable` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ds_test_type`
--

DROP TABLE IF EXISTS `ds_test_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `ds_test_type` (
  `dsid` int NOT NULL AUTO_INCREMENT,
  `TestName` varchar(128) DEFAULT NULL,
  `ds_TINYINT` tinyint DEFAULT NULL,
  `ds_SMALLINT` smallint DEFAULT NULL,
  `ds_INT` int DEFAULT NULL,
  `ds_BIGINT` bigint DEFAULT NULL,
  `ds_FLOAT` float DEFAULT NULL,
  `ds_DOUBLE` double DEFAULT NULL,
  `ds_name` varchar(256) DEFAULT NULL,
  `ds_SUFFIX` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`dsid`)
) ENGINE=InnoDB AUTO_INCREMENT=98 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ds_test_type`
--

LOCK TABLES `ds_test_type` WRITE;
/*!40000 ALTER TABLE `ds_test_type` DISABLE KEYS */;
/*!40000 ALTER TABLE `ds_test_type` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-11-30 22:45:40
