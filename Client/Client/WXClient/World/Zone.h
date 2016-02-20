/****************************************\
*										*
* 			   ��������Zone				*
*										*
\****************************************/
#pragma once
#include "Region.h"
#include "..\Object\Object.h"
#include "TDObjectSystem.h"
/*
|
|	o ��������Ϸ�����ָ�ɹ̶���С������ÿ������λΪһ��Zone��
|     Zone�Ĵ�С��һ����Ļ�Ĵ�С����
|
|   o �̳���MapObject�����嶼��ע�ᵽ�Լ����ڵ�Zone,ÿ��Zone����
|     һ����������ע�ᵽ�����������
|
|   o ��Zone��ע������ݻ���"������������" Region��������
|     Zone���Կ����ҵ�������Region
|
|
*/

class CObject_Map;
class CZone
{
public:
	CZone();
	virtual ~CZone();

	//Zone��ʼ��������������Map��ʼ��ʱ����
	VOID		Initial(VOID);
	//��ĳһObjectע�ᵽ������Object����̳���Object_Map
	VOID		RegisterObject(CObject_Map* pObject);
	//��ĳһObject��ע��
	VOID		UnRegisterObject(CObject_Map* pObject);

public:
	std::set< INT >* GetRigisterObject(){ return &m_setRegisterObject; };
	std::list< INT >* GetListRegion(){ return &m_listRegion; };
protected:
	//ע�ᵽ��Zone�ϵ�Object
	std::set< INT >		m_setRegisterObject;

	//������Zone��Region
	std::list< INT >	m_listRegion;
	
	VOID AddRegion(INT id);

	friend class CScene;
	friend class CPath;
	friend class CObject;
	friend class CAI_MySelf;
	
};