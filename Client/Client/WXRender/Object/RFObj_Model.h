/****************************************\
*										*
*	   CRFObj_Model						*
*										*
\****************************************/

#pragma once
#include "RFObj.h"

class CRFObj_Model : public CWXObj_Object
{
public:
	//�õ�����
	virtual EntityType	GetType(VOID) const	{ return ETYPE_MODEL; }
	//�����Ƿ�ɼ�
	virtual VOID		SetVisible(BOOL bVisible);

	//����MeshFile
	virtual VOID		StaObj_SetMeshFile(LPCTSTR szMeshFile);

public:
	CRFObj_Model();
	virtual ~CRFObj_Model();
};
