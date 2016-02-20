/****************************************\
*										*
* 			   ���ݳ����ݽṹ			*
*					-�ƺ�				*
*										*
\****************************************/

#pragma once

struct _TITLE_
{
	enum
	{
		INVALID_TITLE = 0,
		ID_TITLE,
		STRING_TITLE,
	};
	BOOL		bFlag;							//������title�Ƿ���id
	BYTE		bType;							//������title������
	union
	{
		INT		ID;								//�����id,������Ǳ���id
		CHAR	szTitleData[MAX_CHARACTER_TITLE];	//�������id���������title����
	};
	_TITLE_()
	{
		bFlag		=	INVALID_TITLE;
		bType		=	_TITLE::NO_TITLE;	
		memset(szTitleData, 0, MAX_CHARACTER_TITLE);
	}
};