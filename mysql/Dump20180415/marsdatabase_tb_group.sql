-- MySQL dump 10.13  Distrib 5.7.20, for Linux (x86_64)
--
-- Host: 47.75.160.215    Database: marsdatabase
-- ------------------------------------------------------
-- Server version	5.7.21-0ubuntu0.16.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tb_group`
--

DROP TABLE IF EXISTS `tb_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_group` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(256) NOT NULL,
  `headImg` varchar(256) NOT NULL,
  `peopleNum` int(11) NOT NULL DEFAULT '1',
  `createTs` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_group`
--

LOCK TABLES `tb_group` WRITE;
/*!40000 ALTER TABLE `tb_group` DISABLE KEYS */;
INSERT INTO `tb_group` VALUES (1,'我佛祖','http://static.test.belltrip.cn/upload/GroupImage/2018/04/15//50b51f91251c4993808831a209ef86d5.png',2,'2018-04-15 10:43:50'),(2,'offer592','http://static.test.belltrip.cn/upload/GroupImage/2018/04/15//339eb48fd9fa4e9582c24edb69f40118.png',2,'2018-04-15 11:14:53'),(3,'boydy','http://static.test.belltrip.cn/upload/GroupImage/2018/04/15//2e99eab3d7c349a796dce237009db3b8.png',2,'2018-04-15 11:39:16'),(4,'群啊','http://static.test.belltrip.cn/upload/GroupImage/2018/04/15//4f0b8c2c39014cdc9386df891634ebb7.png',3,'2018-04-15 12:20:15'),(5,'群1','http://static.test.belltrip.cn/upload/GroupImage/2018/04/15//9d4bc13c3bbe47e8b5a045600def0da5.png',2,'2018-04-15 12:22:47'),(6,'群2','http://static.test.belltrip.cn/upload/GroupImage/2018/04/15//54ceced4cfad40d2950a2cfa60ea9949.png',2,'2018-04-15 12:22:53'),(7,'在一起的时候','http://static.test.belltrip.cn/upload/GroupImage/2018/04/15//7e315cf9daec4b32b4f6fca0d7310440.png',4,'2018-04-15 14:56:51');
/*!40000 ALTER TABLE `tb_group` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-15 17:26:59
