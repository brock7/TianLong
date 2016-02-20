/*
|==========================================
|	DBC���ݿ��ļ���
|		(������/�ͻ���ͨ��)
|==========================================
|
|		--------------------
|		|  ���ݿ��ļ���ʽ  |
|		--------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  CHAR[4]    DBC File Identity�� always "TDBC"
|		0X004	  UINT      Number of records in the file 
|		0X008     UINT      Number of 4-byte fields per record
|		0X010     UINT      String block size 
|   FieldType
|		0X014     UINT[FieldNum]   
|							  The type fo fields(0-INT, 1-FLOAT, 2-string)
|   FieldBlock
|				  UINT[FieldNum*RecordNum]
|							  DataBlock
|	StringBlock
|				  CHAR[StringSize]
|							  StringBlock
|
*/
#pragma once

#include <vector>
#include <hash_map>

namespace DBC
{
	class DBCFile
	{
	public:
		//�ļ�ͷ
		struct FILE_HEAD
		{
			CHAR		m_Identify[4];			//��ʾ	'TDBC'
			INT			m_nFieldsNum;			//����
			INT			m_nRecordsNum;			//����
			INT			m_nStringBlockSize;		//�ַ�������С
		};

		//�ֶ���������
		enum FIELD_TYPE
		{
			T_INT		= 0,	//����
			T_FLOAT		= 1,	//������
			T_STRING	= 2,	//�ַ���
		};

		//���ݿ��ʽ����
		typedef std::vector< FIELD_TYPE >	FILEDS_TYPE;

		//���ݶ�
		union FIELD
		{
			FLOAT	fValue;
			INT		iValue;
			CHAR*	pString;	// Just for runtime!
		};
		//������
		typedef std::vector< FIELD >		DATA_BUF;

	public:
		//�򿪶������ļ�������һ�����ݿ�
		BOOL					OpenFromDBC(const CHAR* szFileName);
		//���ı��ļ�������һ�����ݿ�
		BOOL					OpenFromTXT(const CHAR* szFileName);

	public:
		//����
		virtual const FIELD*	Search_Index_EQU(INT nIndex) const;

		virtual const FIELD*	Search_Posistion(INT nRecordLine, INT nColumNum) const;

	public:
		//ȡ��ID
		virtual					GetID(VOID) const				{ return m_ID; }
		//ȡ������
		virtual INT				GetFieldsNum(VOID) const	    { return m_nFieldsNum; }
		//ȡ�ü�¼������
		virtual INT				GetRecordsNum(VOID) const		{ return m_nRecordsNum; }
		//����������
		virtual VOID			CreateIndex(LPCTSTR szFileName, INT nColum = 0);

	protected:
		//���ݿ��ʽ�ļ���
		UINT			m_ID;
		//���ݿ��ʽ����
		FILEDS_TYPE				m_theType;
		//����
		INT						m_nRecordsNum;
		//����
		INT						m_nFieldsNum;
		//������
		DATA_BUF				m_vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
		//�ַ�����
		CHAR*					m_pStringBuf;
		//�ַ�������С
		INT						m_nStringBufSize;
		//������
		stdext::hash_map< INT, FIELD* >	m_hashIndex;
		//������
		INT						m_nIndexColum;

	public:
		static INT				_ConvertStringToVector(const CHAR* strStrINTgSource, std::vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty);

	public:
		DBCFile(UINT id);
		virtual ~DBCFile();
	};
}