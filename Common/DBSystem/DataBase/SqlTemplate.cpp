#include "stdafx.h"
#include "SqlTemplate.h"

CHAR*		LoadCharList		= 
"select \
charguid,sex,charname,level,haircolor,facecolor,hairmodel,facemodel,scene,menpai, \
headid, camp \
from %s where accname =\'%s\' and isvalid =1";
/*
UINT		ItemType;
SHORT		Pos
*/
CHAR*		LoadCharEquipList	=
"select \
itemtype,pos \
from %s where charguid =%d and pos>=%d and pos<%d and isvalid =1";


/*
 *	��ɫ������Ϣ
 */
CHAR*		LoadCharFullData	=
"select \
charguid,charname,title,sex,createtime,level,enegry,exp,vmoney,pw,\
haircolor,facecolor,hairmodel,facemodel,scene,xpos,zpos,logintime,logouttime,dbversion,\
camp,menpai,hp,mp,strikepoint,str,spr,con,ipr,dex,points,settings,\
shopinfo,carrypet,guldid,teamid,headid,erecover,vigor,maxvigor,vrecover,energymax,\
pwdeltime,dietime,bankmoney,bankend,bkscene,bkxpos,bkzpos,rage \
from %s where charguid = %d and isvalid = 1";



CHAR*		UpdateCharFullData	=	
"update \
%s  set title=\'%s\',sex=%d,level=%d,enegry=%d,exp=%d,vmoney=%d,pw=\'%s\',\
haircolor=%d,facecolor=%d,hairmodel=%d,facemodel=%d,scene=%d,xpos=%d,zpos=%d,logintime=%d,logouttime=%d, \
camp=\'%s\',menpai=%d,hp=%d,mp=%d,strikepoint=%d,str=%d,spr=%d,con=%d,ipr=%d,dex=%d,points=%d,settings=\'%s\', \
shopinfo = \'%s\',carrypet = \'%s\' ,guldid= %d,teamid =%d,headid=%d,erecover=%d,vigor=%d,maxvigor=%d,vrecover=%d,energymax=%d, \
pwdeltime=%d,dietime=%d,bankmoney=%d,bankend=%d,bkscene=%d,bkxpos=%d,bkzpos=%d,rage=%d,dbversion=%d  \
where charguid = %d and isvalid = 1";

/*
 *	��ɫ��Ʒ�����
 */

CHAR*		LoadCharItemList	=
"select \
charguid,world,server,guid,itemtype,pos,fixattr,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,dbversion,creator,var \
from %s where charguid = %d and isvalid = 1";

/*
 *	��ɫ��Ʒ��ɾ��
 */
CHAR*	   DeleteCharItemList  =
"delete from %s where charguid = %d";


/*
 *	��ɫ�����б�
 */

CHAR*	   LoadCharSkillList	=	
"select \
skid,sktime \
from %s where charguid = %d and dbversion=%d and isvalid = 1";

/*
 *	 ɾ����ɫ����
 */
CHAR*	   DeleteCharSkillList = 
"delete \
from %s where charguid = %d and dbversion<=%d";


/*
 *	��ɫ�ķ��б�
 */

CHAR*	LoadCharXinFaList	=
"select \
charguid,xinfaid,xinfalvl \
from %s where charguid = %d and dbversion=%d";

/*
 *	ɾ����ɫ�ķ�
 */
CHAR*  DeleteCharXinFaList = 
"delete from %s where charguid = %d and dbversion<=%d";

/*
 *	������б�
 */

CHAR*	LoadCharAbilityList = 
"select \
charguid,abid,ablvl,abexp \
from %s where charguid = %d and dbversion=%d";

CHAR*	LoadCharPresList =
"select \
charguid,pres from %s where charguid = %d and dbversion=%d";



/*
*	ɾ��������б�
*/
CHAR* DeleteCharAbilityList =
"delete from %s where charguid = %d and dbversion<=%d";


/*
 *	�����б�
 */
CHAR*	LoadCharTaskList = 
"select \
charguid,missionid,scriptid,flag,p1,p2,p3,p4,p5,p6,p7,p8 \
from %s where charguid = %d and dbversion=%d";

/*
*	������������
*/
CHAR*	LoadCharTaskDataAndFlag = 
"select \
charguid,mdata,mflag \
from %s where charguid = %d and dbversion=%d";

/*
 *	����˽����Ϣ
 */
CHAR*	LoadCharPrivateInfo =
"select \
charguid,pinfo from %s where charguid=%d and dbversion=%d";

/*
*	���سƺ���Ϣ
*/
CHAR*	LoadCharTitleInfo =
"select \
charguid,titleinfo from %s where charguid=%d and dbversion=%d";


/*
*	������ȴ��Ϣ
*/
CHAR*	LoadCharCoolDownInfo =
"select \
charguid,cooldown from %s where charguid=%d and dbversion=%d";


/*
 *	ɾ�������б�
 */
CHAR* DeleteCharTaskList =
"delete from %s where charguid = %d and dbversion<=%d";

/*
 *	��ϵ�б�
 */

CHAR*	LoadCharRelationList	=	
"select \
charguid,fguid,fname,fpoint,reflag,groupid,extdata \
from %s where charguid = %d and dbversion=%d";

CHAR*   LoadCharRelationData  =
"select \
charguid,relflag from %s where charguid = %d and isvalid =1 and dbversion=%d";

/*
 *	��ϵ�б�ɾ��
 */

CHAR* DeleteCharRelationList = 
"delete from %s where charguid = %d and dbversion<=%d";

/*
 *	���½�ɫ��ϵ��
 */
CHAR* UpdateCharRelationData	=
"update %s set relflag = \'%s\' where charguid = %d and isvalid =1 and dbversion=%d";


/*
 *	�����䷽��
 */
CHAR* UpdateCharPresList	=
"update %s set pres = \'%s\' where charguid = %d and  isvalid = 1 and dbversion = %d";

/*
 *	Impact�б�
 */

CHAR*  LoadCharImpactList	=
"select \
charguid,imdata \
from %s where charguid = %d and dbversion = %d";
/*
 *	Impact�б�ɾ��
 */
CHAR* DeleteCharImpactList =
"delete from %s where charguid = %d and dbversion<=%d";


/*
 *	�����б�
 */

CHAR*	LoadCharPetList	=
"select \
charguid ,hpetguid,lpetguid,dataid,petname,petnick,level,needlevel,atttype,aitype,camp,hp,mp,\
life,pettype,genera,enjoy,strper,conper,dexper,sprper,iprper,gengu,growrate,\
repoint,exp,str,con,dex,spr,ipr,skill \
from %s where charguid =%d and dbversion=%d";
/*
 *	�����б�ɾ��
 */

CHAR*  DeleteCharPetList =
"delete \
from %s where charguid = %d and dbversion<=%d";


/*
 *	�����½�ɫ
 */
CHAR* CreateNewChar	=
"insert into %s(accname,charguid,charname,title,pw,sex,level,enegry,outlook,scene,xpos,zpos,menpai,\
hp,mp,strikepoint,camp,str,con,dex,spr,ipr,points,logouttime,logintime,createtime,dbversion,haircolor,\
hairmodel,facecolor,facemodel,vmoney,settings,isvalid,exp,pres,\
shopinfo,carrypet,guldid,teamid,headid,erecover,vigor,maxvigor,vrecover,energymax,pwdeltime,\
pinfo,bkscene,bkxpos,bkzpos,titleinfo,dietime,bankmoney,bankend,cooldown) \
 values(\'%s\',%d,\'%s\',\'%s\',\'%s\',\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,\'%s\',\
\'%s\',\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,\
\'%s\',%d,%d,%d,\'%s\',%d,%d,%d,\'%s\')";

/*
 *	��ȡ��guid
 */
CHAR*	FetchNewGuid =
"declare @guid int \r\n\
set nocount on \r\n\
exec @guid = fetch_guid\r\n\
set nocount off \r\n\
select @guid B";

/*
 *	ɾ����ɫ
 */
CHAR*	DeleteBaseChar =
"update %s set isvalid = 0,charname = charname+'@DELETE_'+CAST(%d AS CHAR) \
where accname= \'%s\' and charguid = %d";

 /*
  *	������Ʒ
  */
CHAR* NewCharItem	=
"save_iteminfo @pcharguid=%d,@pguid=%d,@pworld=%d,@pserver=%d,@pitemtype=%u,@ppos=%d,@pfixattr=\'%s\',\
@pp1=%d,@pp2=%d,@pp3=%d,@pp4=%d,@pp5=%d,@pp6=%d,@pp7=%d,@pp8=%d,@pp9=%d,@pp10=%d,@pp11=%d,@pp12=%d,@pp13=%d,@pp14=%d,\
@pisvalid=%d,@pdbversion=%d,@pcreator=\'%s\',@pvar=\'%s\'";

//CHAR* NewCharItem	=
//"insert into %s(charguid,world,server,guid,itemtype,pos,fixattr,\
//p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,isvalid,dbversion) \
// values(%d,%d,%d,%d,%d,%d,\'%s\',%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)\n";

/*
 *	��������
 */
CHAR*	NewCharSkill =
"insert into %s(charguid,skid,sktime,dbversion,isvalid) \
	values(%d,%d,%d,%d,1) \n";


/*
*	�����ķ�
*/
CHAR*	NewCharXinFa =
"insert into %s(charguid,xinfaid,xinfalvl,dbversion) \
values(%d,%d,%d,%d)";


/*
*	���������
*/
CHAR*	NewCharAbility =
"insert into %s(charguid,abid,ablvl,abexp,dbversion) \
values(%d,%d,%d,%d,%d)";

/*
*	��������
*/
CHAR*	NewCharTask =
"insert into %s(charguid,missionid,scriptid,flag,p1,p2,p3,p4,p5,p6,p7,p8,dbversion) \
values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)";

/*
 *	������������
 */
CHAR*	UpdateCharTaskData =
"update \
%s  set mdata=\'%s\' \
where charguid = %d and isvalid = 1 and dbversion=%d";

CHAR*	UpdateCharTaskFlag =
"update \
%s set mflag=\'%s\' \
where charguid = %d and isvalid = 1 and dbversion=%d";
/*
 *	����˽����Ϣ
 */
CHAR*	UpdateCharPrivateInfo = 
"update \
%s set pinfo=\'%s\' where charguid=%d and isvalid=1 and dbversion=%d";

/*
*	���³ƺ���Ϣ
*/
CHAR*	UpdateCharTitleInfo = 
"update \
%s set titleinfo=\'%s\'\
where charguid=%d and isvalid=1 and dbversion=%d";


/*
*	������ȴ��Ϣ
*/
CHAR*	UpdateCharCoolDownInfo = 
"update \
%s set cooldown=\'%s\'\
where charguid=%d and isvalid=1 and dbversion=%d";


/*
 *	������ϵ��Ϣ
 */

CHAR*	NewCharRelation	 =
"insert into %s(charguid,fguid,fname,fpoint,reflag,groupid,extdata,dbversion) \
values(%d,%d,\'%s\',%d,%d,%d,\'%s\',%d)";

/*
 *	����Impact��Ϣ
 */
CHAR*	NewCharImpact	=
"insert into %s(charguid,imdata,dbversion) \
values(%d,\'%s\',%d)";

/*
 *	����������Ϣ
 */
CHAR* NewCharPet		=
"insert into %s(charguid ,hpetguid,lpetguid,dataid,petname,petnick,level,needlevel,atttype,aitype,camp,hp,mp,\
life,pettype,genera,enjoy,strper,conper,dexper,sprper,iprper,gengu,growrate,\
repoint,exp,str,con,dex,spr,ipr,skill,dbversion) \
values(%d,%d,%d,%d,\'%s\',\'%s\',%d,%d,%d,%d,\'%s\',%d,%d,\
%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,\'%s\',%d)";

/*
 *	�����������Ʒ���к� 
 */

CHAR* LoadServerItemSerial =
"select sid,serial from %s where sid = %u";

/*
 *	���·�������Ʒ���к�
 */

CHAR* UpdateServerItemSerial = 
"update %s set serial = %u where sid = %u";

/*
 *	��������Email��Ϣ
 */
CHAR* LoadWorldMailInfo	=
"select pindex, sender,recer,mailinfo,mailcont from %s";

/*
*	�½�����Email��Ϣ
*/
CHAR*	SaveWorldMailInfo	=
"save_mailinfo @p1=\'%s\',@p2=\'%s\',@p3=\'%s\',@p4=\'%s\',@p5=%d,@p6=%d";

/*
*	ɾ������Email��Ϣ
*/
CHAR*	DeleteWorldMailInfo	=
"update t_mail set isvalid = 0 where pindex=%d";

/*
*	�������繤����Ϣ
*/
CHAR*	LoadWorldGuildInfo	=
"select  guildid,guildname,guildstat,chiefguid,pcount,ucount,mucount,gpoint,guildmoney,cityid,\
time,logevity,contribu,honor,indlvl,agrlvl,comlvl,deflvl,techlvl,ambilvl,admin,guilddesc, \
chiefname,cname,glvl,guilduser \
from %s where isvalid=1";

/*
*	�½����繤����Ϣ
*/
CHAR*	SaveWorldGuildInfo	=
"save_guildinfo @pguildid=%d,@pguildname=\'%s\',@pguildstat=%d,@pchiefguid=%d,@ppcount=%d,\
@pucount=%d,@pmucount=%d,@pgpoint=%d,@pguildmoney=%d,@pcityid=%d,@ptime=%d,@plogevity=%d,\
@pcontribu=%d,@phonor=%d,@pindlvl=%d,@pagrlvl=%d,@pcomlvl=%d,@pdeflvl=%d,@ptechlvl=%d,@pambilvl=%d,\
@padmin=\'%s\',@pguilddesc=\'%s\',@pchiefname=\'%s\',@pcname=\'%s\',@pglvl=%d,@pguilduser=\'%s\',@pisvalid=%d";

/*
*	ɾ�����繤����Ϣ
*/
CHAR*	DeleteWorldGuildInfo	=
"update t_guild set isvalid = 0 where pindex=%d";

/*
*	���س�ֵ��Ϣ
*/
CHAR*	LoadWebShopInfo = 
"select top 1 \
Id,Money,Item_1,ItemCount_1,Item_2,ItemCount_2,Item_3,ItemCount_3,Item_4,ItemCount_4,\
Item_5,ItemCount_5 from %s where UserId=%d and IsSucceed=%d order by CreateTime";

/*
*	�����ֵ��Ϣ
*/
CHAR*	SaveWebShopInfo = 
"update %s set IsSucceed=%d where Id=%d";
