#pragma once

#include "TDBasicType.h"
/****************************************\
*										*
*			  �ڵ��ඨ����				*
*										*
\****************************************/
class tNode;
struct /*TDENGDA_API*/ tClass
{
	LPCTSTR							szClassName;					//!< Node������
	INT								nObjectSize;					//!< Node��Ĵ�С
	tNode* (__stdcall*				pfnCreateObject)();				//!< ����ָ�룬��������һ��INode��ʵ��
	tClass*							pBaseClass;						//!< ���̳еĻ���
	tClass*							pNextClass;						//!< ��һ����
	tNode*							CreateObject(LPCTSTR szName);	//!< ��������һ��ʵ���ĺ���
};

//��������ȡ�ö�����
#define GETCLASS(className)  (&className::m_class##className)

//�ඨ��������
#define WX_DECLARE_DYNAMIC(className) \
public: \
	static tClass				m_class##className; \
	virtual const tClass*		GetClass(VOID) const; \
	static tNode* __stdcall	CreateObject();

//�ඨ��ʵ�ֺ�
#define WX_IMPLEMENT_DYNAMIC(className, baseClass) \
	static TCHAR sz##className[] = _T(#className); \
	tClass className::m_class##className = \
	{ sz##className, sizeof(className), className::CreateObject, baseClass, NULL }; \
	const tClass* className::GetClass() const \
		{ return &className::m_class##className; } \
	tNode* className::CreateObject() \
		{ return new className; } 


//�������ඨ��ʵ�ֺ�
#define WX_IMPLEMENT_DYNAMIC_VIRTUAL(className, baseClass) \
	static TCHAR sz##className[] = _T(#className); \
	tClass className::m_class##className = \
	{ sz##className, sizeof(className), className::CreateObject, baseClass, NULL }; \
	const tClass* className::GetClass() const \
		{ return &className::m_class##className; } \
	tNode* className::CreateObject() \
		{ TDThrow(_T("Pure virtual class")); return NULL; } 
