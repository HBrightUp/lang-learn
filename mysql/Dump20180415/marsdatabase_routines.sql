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
-- Dumping events for database 'marsdatabase'
--

--
-- Dumping routines for database 'marsdatabase'
--
/*!50003 DROP PROCEDURE IF EXISTS `sp_conversation_getlist` */;
ALTER DATABASE `marsdatabase` CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_conversation_getlist`(in _uid int,in _conversationId varchar(32))
BEGIN
	
    CREATE TEMPORARY TABLE temp_conversations
	select sendId,receiveId,t3.conversationId,lastMsg,lastTime,msgType,sendType 
	from tb_conversation t3
	join (
		select conversationId from tb_grouprelate where userId=_uid
		union
		select conversationId from tb_contacts where uid=_uid
	)tt on t3.conversationId=tt.conversationId;
    
    #复制会话表数据
	create temporary table temp_conversations2 select * from temp_conversations;
   
    #单聊
    select cast(_uid as unsigned int4) as uid, case when t2.nickName='' then t2.name else t2.nickName end as nickName
		,t1.lastMsg,t1.lastTime,t1.msgType,t1.conversationId,t1.sendType,t2.head as headImg
		,case when _uid=t1.sendId then t1.receiveId else t1.sendId end as cuid
    from temp_conversations t1
    join tb_usrinfo t2 on t1.sendType=2 and  t2.uid=case when _uid=t1.sendId then t1.receiveId else t1.sendId end
    
    union
    
    #群聊
    
    select cast(_uid as unsigned int4) as uid, t5.name as nickName
		,t4.lastMsg,t4.lastTime,t4.msgType,t4.conversationId,t4.sendType,t5.headImg
		,t5.id as cuid
    from temp_conversations2 t4
    join tb_group t5 on t5.id=t4.receiveId
    where t4.sendType=1;
 
    
    DROP TEMPORARY TABLE if exists temp_conversations;
    DROP TEMPORARY TABLE if exists temp_conversations2;
       
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
ALTER DATABASE `marsdatabase` CHARACTER SET utf8 COLLATE utf8_general_ci ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_conversation_save` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`hml`@`%` PROCEDURE `sp_conversation_save`(in _sendId int,in _receiveId int,in _conversationId varchar(32),in _lastMsg varchar(2000),in _msgType int,in _sendType int,out _result int)
BEGIN
	set _result=0;
	if exists(select id from tb_conversation where conversationId=_conversationId) then
		set _result=1;
        update tb_conversation set lastMsg=_lastMsg,lastTime=now() where conversationId=_conversationId;
	else
		set _result=1;
		INSERT INTO tb_conversation(sendid, receiveid, conversationid, lastMsg,msgType, sendType)
        values(_sendId,_receiveId,_conversationId,_lastMsg,_msgType,_sendType);
	end if;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_dosplit` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_dosplit`(_sourceStr nvarchar(1000),_split char(10))
BEGIN
	CREATE TEMPORARY TABLE if not exists tmplst(val nvarchar(100));
	delete from tmplst;
    while(instr(_sourceStr,_split)>0)   
        do 
			insert into tmplst(val)values(substring_index(_sourceStr,_split,1));
			set _sourceStr=substring(_sourceStr,instr(_sourceStr,_split)+1);
		end while;
        insert into tmplst(val)values(_sourceStr);      
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_group_create` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_group_create`(in _name varchar(256) charset utf8 ,in _uid int,in _cuids varchar(1000),in _imgurl varchar(256),out result int)
BEGIN
	
	#参数定义为nvarchar()会出错
	if exists(select id from tb_group where name=_name) then
		set result=2;
		
	else
    
		insert into tb_group(name,headImg)values(_name,_imgurl);
		set @groupId=LAST_INSERT_ID();
		set @conversationId=replace(UUID(),'-','');
		call sp_dosplit(_cuids,',');
		
		#群主
		insert into tb_grouprelate(groupId,userId,userRole,conversationId)
		values(@groupId,_uid,1,@conversationId);
		
		#普通用户
		insert into tb_grouprelate(groupId,userId,userRole,conversationId)
		select @groupId,cast(val as signed),0,@conversationId	
		from tmplst;
        
        set @peopleNum =(select count(id) from tb_grouprelate where groupId=@groupId);
        
        update tb_group set peopleNum =@peopleNum where id=@groupId;
        
		
		select cast(_uid as unsigned int) as uid, _name as nickName,cast(@groupId as unsigned int) as cuid
			,'加群成功' as lastMsg,NOW() as lastTime,cast(1 as unsigned int) as msgType,@conversationId as conversationId,cast(1 as unsigned int) as  sendType
            ,cast(@peopleNum as unsigned int) as peopleNum ,_imgurl as headImg;
			
		
		set result=1;
    
    end if ;
    
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_safe_updates` */;
ALTER DATABASE `marsdatabase` CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `sp_safe_updates`()
BEGIN
SET SQL_SAFE_UPDATES = 0;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
ALTER DATABASE `marsdatabase` CHARACTER SET utf8 COLLATE utf8_general_ci ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_test` */;
ALTER DATABASE `marsdatabase` CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_test`(in c int,in a varchar(256))
BEGIN
	#in _name nvarchar(256),in _uid int,in _cuids nvarchar(1000),out result int
	#set b=9999;
	select * from tb_usrinfo where uid=1000017;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
ALTER DATABASE `marsdatabase` CHARACTER SET utf8 COLLATE utf8_general_ci ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_user_addcontact` */;
ALTER DATABASE `marsdatabase` CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_user_addcontact`(in _uid int(10),in _cuid int(10),out result int(10))
BEGIN
	set result=0;
    if exists(select 1 from tb_contacts where uid=_uid and cuid=_cuid) then
		set result=2;
	else
		set result=1;
        set @conversationId=replace(UUID(),'-','');
		INSERT INTO tb_contacts(uid,cuid,conversationId)
		VALUE(_uid,_cuid,@conversationId);
        INSERT INTO tb_contacts(uid,cuid,conversationId)
		VALUE(_cuid,_uid,@conversationId);
	end if;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
ALTER DATABASE `marsdatabase` CHARACTER SET utf8 COLLATE utf8_general_ci ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_user_getcontacts` */;
ALTER DATABASE `marsdatabase` CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_user_getcontacts`(in _uid int(10))
BEGIN
	SELECT t2.name,t2.sex,t2.phone,t2.mail,t2.birthday,t2.head,t2.createTime,t1.uid,t1.cuid,t1.conversationId
	FROM tb_contacts t1
	inner join tb_usrinfo t2 on t1.cuid =t2.uid and t1.status=1
	where t1.uid=_uid;
    
    /*
    select t3.id,t3.name,t3.headImg,t3.peopleNum,t3.createTs
		,t4.conversationId,t4.userId
    from tb_group t3
	join tb_grouprelate t4 on t4.userId=_uid and t3.id=t4.groupId;
    */
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
ALTER DATABASE `marsdatabase` CHARACTER SET utf8 COLLATE utf8_general_ci ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_user_getgroups` */;
ALTER DATABASE `marsdatabase` CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_user_getgroups`(in _uid int(10))
BEGIN
	/*
	SELECT t2.name,t2.sex,t2.phone,t2.mail,t2.birthday,t2.head,t2.createTime,t1.uid,t1.cuid,t1.conversationId
	FROM tb_contacts t1
	inner join tb_usrinfo t2 on t1.cuid =t2.uid
	where t1.uid=_uid;
    */
    
    select t3.id,t3.name,t3.headImg,t3.peopleNum,t3.createTs
		,t4.conversationId,t4.userId
    from tb_group t3
	join tb_grouprelate t4 on t4.userId=_uid and t3.id=t4.groupId;
    
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
ALTER DATABASE `marsdatabase` CHARACTER SET utf8 COLLATE utf8_general_ci ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_user_getlist` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_user_getlist`(in uids varchar(1000))
BEGIN
	call sp_dosplit(uids,',');
    
    select t1.head
    from tb_usrinfo t1
    join tmplst t2 on t1.uid=t2.val;
    
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_user_login` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_user_login`(in uName varchar(32),in uPwd varchar(64))
BEGIN
	#ddd
	select uid,name,sex,phone,mail,birthday,head,token,createTime,nickName from tb_usrinfo where Name=uName and PassWord=uPwd;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_user_reg` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_user_reg`(IN uName varchar(32), IN uPwd varchar(64),in _nickName varchar(256),IN sex int(10),IN phone varchar(16),IN mail varchar(32),IN birthday DATE, IN head varchar(256),out result int(10))
BEGIN
	set result=0;
    if exists(select 1 from tb_usrinfo where name=uName) then
		set result=2;
	else
		set result=1;
		INSERT INTO tb_usrinfo(name,password,sex,phone,mail,birthday,head,token,nickName,realName)
		VALUE(uName,uPwd,sex,phone,mail,birthday,head,replace(UUID(),'-',''),_nickName,'');
	end if;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `sp_user_search` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`zyj`@`%` PROCEDURE `sp_user_search`(in _uid int,in _suid int,in _loginname varchar(256))
BEGIN
	select t1.uid,t1.name,t1.sex,t1.head,t1.createTime
		,cast(case when t2.id is null then -1 else t2.status end as unsigned) as contactType
    from tb_usrinfo t1
    left join tb_contacts t2 on t2.uid=_uid and t1.uid=t2.cuid
    where length(_loginname)>0 and t1.uid<>_uid and (t1.name like  concat('%',_loginname,'%') or t1.nickname like  concat('%',_loginname,'%'));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-04-15 17:27:01
