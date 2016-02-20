/****************************************\
*										*
*	   CRFObj_Static					*
*										*
\****************************************/

#pragma once
#include "RFObj.h"

class CRFObj_Static : public CWXObj_Object
{
public:
	//�õ�����
	virtual EntityType	GetType(VOID) const	{ return ETYPE_MESH; }
	//�����Ƿ�ɼ�
	virtual VOID		SetVisible(BOOL bVisible);

	//����MeshFile
	virtual VOID		StaObj_SetMeshFile(LPCTSTR szMeshFile);

public:
	CRFObj_Static();
	virtual ~CRFObj_Static();
};
