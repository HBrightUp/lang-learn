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
-- Table structure for table `tb_usrinfo`
--

DROP TABLE IF EXISTS `tb_usrinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_usrinfo` (
  `uid` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL,
  `password` varchar(64) NOT NULL,
  `sex` tinyint(3) unsigned NOT NULL,
  `phone` varchar(16) DEFAULT NULL,
  `mail` varchar(32) DEFAULT NULL,
  `birthday` date DEFAULT NULL,
  `head` varchar(256) DEFAULT NULL,
  `token` varchar(64) NOT NULL,
  `createTime` datetime DEFAULT CURRENT_TIMESTAMP,
  `realName` varchar(45) NOT NULL,
  `nickName` varchar(45) NOT NULL,
  PRIMARY KEY (`uid`),
  UNIQUE KEY `idtb_usrInfo_UNIQUE` (`name`),
  UNIQUE KEY `ID_UNIQUE` (`uid`),
  UNIQUE KEY `tokens_UNIQUE` (`token`)
) ENGINE=InnoDB AUTO_INCREMENT=10124 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_usrinfo`
--

LOCK TABLES `tb_usrinfo` WRITE;
/*!40000 ALTER TABLE `tb_usrinfo` DISABLE KEYS */;
INSERT INTO `tb_usrinfo` VALUES (1,'wsz','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','300889ac404711e8b76c00163e00b46a','2018-04-15 08:51:52','',''),(2,'hml','E10ADC3949BA59ABBE56E057F20F883E',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','35bbf45e404711e8b76c00163e00b46a','2018-04-15 08:52:02','',''),(3,'wsz2','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','dfa73acc404911e8b76c00163e00b46a','2018-04-15 09:11:06','',''),(4,'wsz3','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','e9980012404a11e8b76c00163e00b46a','2018-04-15 09:18:32','',''),(5,'wsz4','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','f1643808404b11e8b76c00163e00b46a','2018-04-15 09:25:55','',''),(6,'wsz5','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','1bee4c7b404c11e8b76c00163e00b46a','2018-04-15 09:27:06','',''),(7,'wsz6','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','0820a0eb404d11e8b76c00163e00b46a','2018-04-15 09:33:42','',''),(8,'wsz7','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','799f920b404f11e8b76c00163e00b46a','2018-04-15 09:51:12','',''),(9,'xiong','78AF172A046CC53069960B3CB7DC19AC',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','200bb3c4405011e8b76c00163e00b46a','2018-04-15 09:55:51','',''),(10,'wsz8','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','2203df2a405111e8b76c00163e00b46a','2018-04-15 10:03:04','',''),(11,'Royal','3E336734C7406D745C4CD7598E108F9C',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','b0a9adb8405211e8b76c00163e00b46a','2018-04-15 10:14:13','',''),(12,'wsz9','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','aa1c1ae8405311e8b76c00163e00b46a','2018-04-15 10:21:11','',''),(13,'qwer11','E10ADC3949BA59ABBE56E057F20F883E',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','da1fc756405311e8b76c00163e00b46a','2018-04-15 10:22:32','',''),(14,'wsz10','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','ed87108f405411e8b76c00163e00b46a','2018-04-15 10:30:14','',''),(15,'wsz11','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','ac9be880405511e8b76c00163e00b46a','2018-04-15 10:35:34','',''),(16,'luoyou','3E336734C7406D745C4CD7598E108F9C',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','e4d412a6405511e8b76c00163e00b46a','2018-04-15 10:37:09','',''),(17,'wsz13','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','c1bd55d6405611e8b76c00163e00b46a','2018-04-15 10:43:19','',''),(18,'wsz14','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','6b2c9920405811e8b76c00163e00b46a','2018-04-15 10:55:13','','wszyui'),(19,'test11','78AF172A046CC53069960B3CB7DC19AC',0,'13495466830','123@qq.com','1999-01-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','de93d569405811e8b76c00163e00b46a','2018-04-15 10:58:27','','这里是昵称'),(10101,'test111','78AF172A046CC53069960B3CB7DC19AC',0,'13495466830','123@qq.com','1999-01-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','a892fe9a405911e8b76c00163e00b46a','2018-04-15 11:04:05','','这里是昵称'),(10102,'cch','3E336734C7406D745C4CD7598E108F9C',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','5e3ff25a405a11e8b76c00163e00b46a','2018-04-15 11:09:10','','aa'),(10103,'yyq','3E336734C7406D745C4CD7598E108F9C',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','9d53e1c8405a11e8b76c00163e00b46a','2018-04-15 11:10:56','','aa'),(10104,'luo','3E336734C7406D745C4CD7598E108F9C',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','f5bf95be405d11e8b76c00163e00b46a','2018-04-15 11:34:53','','abc'),(10105,'you','3E336734C7406D745C4CD7598E108F9C',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','112ff151405e11e8b76c00163e00b46a','2018-04-15 11:35:39','','ABC'),(10106,'wsz0','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','92083af9405f11e8b76c00163e00b46a','2018-04-15 11:46:25','','wsz1240'),(10107,'zj1','66EEE9ACE7508C154D02022000E1CF86',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','19313b1c406111e8b76c00163e00b46a','2018-04-15 11:57:21','',''),(10108,'zj2','66EEE9ACE7508C154D02022000E1CF86',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','681d220b406111e8b76c00163e00b46a','2018-04-15 11:59:33','',''),(10109,'wsz21','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','f35274e2406111e8b76c00163e00b46a','2018-04-15 12:03:27','','wsz21'),(10110,'wsz31','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','09fc93e0406711e8b76c00163e00b46a','2018-04-15 12:39:52','','31'),(10111,'wsz40','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','2228e858406a11e8b76c00163e00b46a','2018-04-15 13:02:01','','40'),(10112,'zj3','7694F4A66316E53C8CDD9D9954BD611D',0,'13495466830','123@qq.com','1999-01-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','eb8019af406a11e8b76c00163e00b46a','2018-04-15 13:07:39','','这里是昵称'),(10113,'lyb','3E336734C7406D745C4CD7598E108F9C',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','3dd675d0406b11e8b76c00163e00b46a','2018-04-15 13:09:57','','abcs'),(10114,'bian','3E336734C7406D745C4CD7598E108F9C',0,'15992406441','120@qq.com','1991-02-01','http://static.test.belltrip.cn/upload/mars/touxiang.png','5a45b226406b11e8b76c00163e00b46a','2018-04-15 13:10:45','','vhgg'),(10115,'wsz41','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','423c3d8f406e11e8b76c00163e00b46a','2018-04-15 13:31:33','','wsz41'),(10116,'vin126','D2A03D7F6791775225884ED8013E2D59',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','3ccd1597406f11e8b76c00163e00b46a','2018-04-15 13:38:34','',''),(10117,'Jason368','0F2A48E6D5254EF45C1406EB233B0209',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','59bba915406f11e8b76c00163e00b46a','2018-04-15 13:39:22','',''),(10118,'wsz43','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','b9864b94407011e8b76c00163e00b46a','2018-04-15 13:49:12','','43'),(10119,'wsz42','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','71cefdb6407111e8b76c00163e00b46a','2018-04-15 13:54:21','','42'),(10120,'wsz56','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','71784dbc407211e8b76c00163e00b46a','2018-04-15 14:01:30','','56'),(10121,'lv88888888','8068C76C7376BC08E2836AB26359D4A4',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','14b8c348407311e8b76c00163e00b46a','2018-04-15 14:06:04','',''),(10122,'wsz50','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','29028c1f407411e8b76c00163e00b46a','2018-04-15 14:13:48','','50'),(10123,'wsz90','202CB962AC59075B964B07152D234B70',0,'13182676026','33333@qq.com','1996-10-05','http://static.test.belltrip.cn/upload/mars/touxiang.png','ba215000407911e8b76c00163e00b46a','2018-04-15 14:53:39','','90');
/*!40000 ALTER TABLE `tb_usrinfo` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-15 17:26:58
