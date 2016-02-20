/****************************************\
*										*
* 	  ������ui����ʾ������������ӿ�	*
*										*
\****************************************/

#pragma once
#include "TDNode.h"
/*
	1. ����/������ui����ʾ��obj����
	2. ������Ҫ��ʾ��FakeObj��ÿ�������Ⱦ����Ⱦ��Texture�ϣ������µ�ui��
	3. ��ЩObj������߼����������������װ�������Ȳ�������Ҫ���ϲ��߼�����
*/

#define FAKE_RENDERTEXTURE		("_RenderTexture")

class tObject;
class tFakeObjSystem : public tNode
{
public:
	//����/������ui����ʾ��obj����
	virtual tObject*		NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName)	= 0;
	virtual VOID			DestroyObject(LPCTSTR szObjName) = 0;

	//UI��Ҫ��ʾĳ����
	virtual VOID			OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName)	= 0;

	//UI��ʾ/����
	virtual	VOID			OnUIShown(LPCTSTR szWindowName, float fAspectRatio)	= 0;
	virtual VOID			OnUIHiden(LPCTSTR szWindowName)	= 0;

	//��תģ��
	virtual VOID			RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle) = 0;
	virtual VOID			RotateObj_End(LPCTSTR szObjName) = 0;
	virtual VOID			Obj_ChangeAction(LPCTSTR szObjName, INT SkillID,INT SkillLoop) = 0;
protected:
	WX_DECLARE_DYNAMIC(tFakeObjSystem);
};
