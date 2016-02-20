//-----------------------------------------------------------------------------
// �ļ��� : LuaInterface.h
// ģ��	:	Script
// ����	 :	�ű�ϵͳ�ṩ�����ʹ�õ�Lua�Ľӿ�
// �޸���ʷ:
//-----------------------------------------------------------------------------
#ifndef __LUAINTERFACE_H_
#define __LUAINTERFACE_H_
#include "Type.h"
#include "FoxLuaScript.h"
#include "LuaFnRegister.h"
#include "IDTable.h"
#include "Log.h"

#define FILENAMEKEY  "LOADEDFILENAME"

#define MAX_SCRIPT_HOLD 4096

#ifdef __LINUX__
	#define LUA_ENTER_FUNCTION {try{
	#define LUA_LEAVE_FUNCTION }catch(...){g_pLog->FastSaveLog(LOG_FILE_2,"Lua Error %s %s %s",__FILE__,__LINE__,__PRETTY_FUNCTION__);}}
#elif __WINDOWS__
	#define LUA_ENTER_FUNCTION {try{
	#define LUA_LEAVE_FUNCTION }catch(...){g_pLog->FastSaveLog(LOG_FILE_2,"Lua Error %s %s %s",__FILE__,__LINE__,__FUNCTION__);}}
#endif

class Scene;
class LuaInterface
{
public:
	//�ű�����
	FoxLuaScript		mLua ;
	//ע����
	LuaCFuncRegister	mFuncRegister;
	//��������
	Scene*				mOwner;
	//�Ѿ���ȡ�Ľű���
	IDTable				m_ScriptTable ;

public:
	LuaInterface();
	~LuaInterface();

	VOID	Init(Scene* pScene);//���Lua�ű������ĳ�ʼ����C����������ע��
	VOID	Destroy();

	Scene* GetOwner();

	INT		ExeScript( ScriptID_t scriptid, CHAR* funcname ) ;
	INT		ExeScript_D( ScriptID_t scriptid, CHAR* funcname, INT Param0 ) ;
	INT		ExeScript_DD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1 ) ;
	INT		ExeScript_DDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2 ) ;
	INT		ExeScript_DDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3 ) ;
	INT		ExeScript_DDDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4 ) ;
	INT		ExeScript_DDDDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5 ) ;
	INT		ExeScript_DDDDDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, INT Param6 ) ;
	INT		ExeScript_DDDDDDDD( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, INT Param6, INT Param7 ) ;
	INT		ExeScript_DDNN( ScriptID_t scriptid, CHAR* funcname, INT Param0, INT Param1, FLOAT fParam2, FLOAT fParam3 ) ;

	VOID	OnEventRequest( ScriptID_t misId,ObjID_t idObj, ObjID_t targetObjId, ScriptID_t scriptId ,INT exIndex) ;
	
	BOOL	FindSymbol(Lua_State* L,char* funcname);
	BOOL	PrefixCall(Lua_State* L,char** funcname); 

protected :
	//�ײ㹦�ܽӿ�
	INT		ExeFile( CHAR* filename, CHAR* funcname, BOOL bLoad ) ;
	INT		ExeFile_D( CHAR* filename, CHAR* funcname, INT Param0, BOOL bLoad ) ;
	INT		ExeFile_DD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, BOOL bLoad ) ;
	INT		ExeFile_DDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, BOOL bLoad ) ;
	INT		ExeFile_DDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, BOOL bLoad ) ;
	INT		ExeFile_DDDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, BOOL bLoad ) ;
	INT		ExeFile_DDDDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, BOOL bLoad ) ;
	INT		ExeFile_DDDDDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, INT Param6, BOOL bLoad ) ;
	INT		ExeFile_DDDDDDDD( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, INT Param2, INT Param3, INT Param4, INT Param5, INT Param6, INT Param7, BOOL bLoad ) ;
	INT		ExeFile_DDNN( CHAR* filename, CHAR* funcname, INT Param0, INT Param1, FLOAT fParam2, FLOAT fParam3, BOOL bLoad=TRUE ) ;
	
private:
	VOID SetOwner(Scene* pScene);

};



#endif
