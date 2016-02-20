#include "StdAfx.h"
#include "UIGlobal.h"
#include "UIString.h"
#include "TDKernel.h"
#include "TDObjectSystem.h"
#include "TDException.h"
#include "TDDataBase.h"
#include "TDDBC_Struct.h"
#include "Core/utf.h"

struct AUTO_LOCALE_CONVERT
{
	AUTO_LOCALE_CONVERT()
	{
		std::locale langLocale("");
		old = std::locale::global(langLocale);
	}
	~AUTO_LOCALE_CONVERT()
	{
		std::locale::global(old);
	}

	std::locale old;
};

CUIStringSystem* CUIStringSystem::s_pMe = NULL;
CUIStringSystem::CUIStringSystem()
{
	s_pMe = this;
}

CUIStringSystem::~CUIStringSystem()
{
}

VOID CUIStringSystem::Init(LPCTSTR szStringDBC)
{
	//----------------------------------------------------
	//�ַ����ֵ�
	tDataBaseSystem* pUIDBC = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(pUIDBC);
	const tDataBase* pDBCDic = pUIDBC->GetDataBase(DBC_STRING_DICT);
	for(INT i=0; i<(INT)pDBCDic->GetRecordsNum(); i++)
	{
		const _DBC_STRING_DICT* pLine = (const _DBC_STRING_DICT*)((tDataBase*)pDBCDic)->Search_LineNum_EQU(i);
		m_theDirectionary[pLine->szKey] = pLine->szString;
	}

	//ϵͳ�ؼ���
	//m_theDirectionary[UKS_PLAYERNAME] = 
	
	//---------------------------------
	//�ַ����ֵ�idת��
	DIRECTIONARY::iterator it;
	for(it=m_theDirectionary.begin(); it!=m_theDirectionary.end(); it++)
	{
		STRING strOut;
		ParserString_Prebuild(it->second, strOut);

		it->second = strOut;
	}
	//------------------------------------------------

	//����ģ�崴��
	GenerateTalkTemplateTable();

	// ����ӳ��
	//��������ui
	const tDataBase* pDBC = pUIDBC->GetDataBase(DBC_CODE_CONVERT);
	for(INT i=0; i<(INT)pDBC->GetRecordsNum(); i++)
	{
		const _DBC_CODE_CONVERT* pLine = (const _DBC_CODE_CONVERT*)((tDataBase*)pDBC)->Search_LineNum_EQU(i);
		CEGUI::utf32 dwValue = 0;
		sscanf( pLine->szCode, "%8X", &dwValue );
		m_mapCodeConvertTable[ pLine->nID ] = dwValue ;
	}

	//------------------------------------------------
	// ���˴����鹹��
	const tDataBase* pDBCfl = pUIDBC->GetDataBase(DBC_TALK_FILTER);
	for(INT i=0; i<(INT)pDBCfl->GetRecordsNum(); i++)
	{
		const _DBC_TALK_FILTER* pLine = (const _DBC_TALK_FILTER*)((tDataBase*)pDBCfl)->Search_LineNum_EQU(i);
		m_vecFilterTable.push_back(pLine->szString);
	}

	//------------------------------------------------
	// ��ȫƥ����˱���
	GenerateFullCompareTable();
}

std::pair< BOOL, CUIStringSystem::DIRECTIONARY::iterator > CUIStringSystem::_IsValidDictionKey(const STRING& strKey)
{
	DIRECTIONARY::iterator it = m_theDirectionary.find(strKey);
	if(it == m_theDirectionary.end()) return std::make_pair(false, it);
	else return std::make_pair(true, it);
}

// Prebuildת������Ҫ�Ǵ��ֵ��ϵͳ���ַ����в���
VOID CUIStringSystem::ParserString_Prebuild(const STRING& strSource, STRING& strOut)
{
	const CHAR KeyParserBegin	= '{';
	const CHAR KeyParserMiddle	= '=';
	const CHAR KeyParserEnd		= '}';

	STRING::size_type nValidBegin = 0;
	strOut = "";

	do
	{
		if(nValidBegin >= strSource.size()) break;

		STRING::size_type nValidEnd = strSource.find(KeyParserBegin, nValidBegin);

		//���һ������ֵ
		if(nValidEnd == STRING::npos) 
		{
			strOut += strSource.substr(nValidBegin);

			break;
		}

		//��������ֵ
		if(nValidEnd != nValidBegin) 
		{
			strOut += strSource.substr(nValidBegin, nValidEnd-nValidBegin);
		}

		STRING::size_type nKeyStart = nValidEnd+1;									//"{"
		STRING::size_type nKeyMiddle = strSource.find(KeyParserMiddle, nKeyStart);	//"="
		STRING::size_type nKeyEnd = strSource.find(KeyParserEnd, nKeyStart);		//"}"
		
		//�Ϸ���Key����
		if(nKeyEnd != STRING::npos && nKeyMiddle != STRING::npos && nKeyEnd > nKeyMiddle)
		{
			STRING strKey_Left = strSource.substr(nKeyStart, nKeyMiddle-nKeyStart);
			STRING strKey_Right = strSource.substr(nKeyMiddle+1, nKeyEnd-nKeyMiddle-1);

			if(strKey_Left == "ID")
			{
				std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strKey_Right);
				//�ҵ�key
				if(isValidKey.first)
				{
					//�����keyת��������ֵ
					strOut += isValidKey.second->second;

					nValidBegin = nKeyEnd+1;
					continue;
				}
			}
		}
		strOut += strSource.substr(nValidEnd, nKeyEnd-nValidEnd);
		nValidBegin = nKeyEnd;
	}while(true);
}

//��MBCS���ַ������кϷ���⣬�޳��Ƿ��ַ�������������
/*
|
|	ʹ��GBK��׼
|
|	Windows95�������汾���İ�֧��GBK��
|	GB2312��EUC���뷶Χ�ǵ�һ�ֽ�0xA1~0xFE(ʵ��ֻ�õ�0xF7)����
|	���ֽ�0xA1~0xFE��GBK�Դ˽�����չ����һ�ֽ�Ϊ0x81~0xFE���ڶ�
|	�ֽڷ������֣�һ��0x40~0x7E������0x80~0xFE�����к�GB2312��
|	ͬ����������ȫ��ͬ����չ���ִ���ǰ�����(����)�ͱ�˳(�ʻ�)
|	��GB13000��ȡ����������GBK�С����GBK������GB13000����Ȼ����
|	�ֻ���ͬ����������ϵ��ͬ��һ����ISO2022ϵ�в��ȳ����룬һ��
|	�ǵȳ����룬���ұ�������Ҳ��ͬ��ע�⵽GBKʵ���ϲ��ǹ��ұ�׼��
|	�ڴ�֮ǰ��һ��GB2312������������֮����һ���������Ƚ���GB13000��
|	GBKֻ��һ�ֹ��ɺ���չ�淶��������Unicode����GB2312->Unicode,
|	GB12345->Unicode��ת����񣬶�û��GBK->Unicodeת�����ֻ��
|	Microsoft������Code Page 936(CP936.TXT)��������GBK->Unicode
|	ת����񡣵�Ҫע������һ����ҵ��˾�������ļ��������ǹ��һ�
|	���ʱ�׼��֯�����ģ��п������׼�в�һ�µĵط���
|
|
*/
BOOL CUIStringSystem::_CheckStringCode(const STRING& strIn, STRING& strOut)
{
	const BYTE byANSIBegin		= 0X20;
	const BYTE byANSIEnd		= 0X80;

	const BYTE by1GBKBegin	= 0X81;
	const BYTE by1GBKEnd	= 0XFE;

	const BYTE by2GBKBegin1	= 0X40;
	const BYTE by2GBKEnd1	= 0X7E;
	const BYTE by2GBKBegin2	= 0X80;
	const BYTE by2GBKEnd2	= 0XFE;

	const CHAR * const szERROR = "<ERROR>";

	//Ԥ�ȷ�����ڴ�
	INT nSourSize = (INT)strIn.size();
	strOut.reserve(nSourSize+1);
	strOut = "";

	for(INT i=0; i<nSourSize; )
	{
		BYTE byChar = strIn[i];

		//��׼ANSIӢ���ַ�
		if(byChar >= byANSIBegin && byChar <= byANSIEnd || byChar == '\r' || byChar == '\n' || byChar == '\t')
		{
			strOut += byChar;
			i++; 
			continue;
		}
		//���ֵ�һ���ֽ�
		else if(byChar >= by1GBKBegin && byChar <= by1GBKEnd)
		{
			//�������
			if(i+1 == nSourSize) goto CHECKSTRING_ERROR;

			//ȡ���ڶ����ֽ�
			BYTE byNext = strIn[++i];
			if(	!( byNext >= by2GBKBegin1 && byNext <= by2GBKEnd1 || byNext >= by2GBKBegin2 && byNext <= by2GBKEnd2) )
			{
				//�Ƿ��ĺ���
				goto CHECKSTRING_ERROR;
			}

			//���������ĺ���
			CHAR szTemp[8];
			szTemp[0] = byChar;
			szTemp[1] = byNext;
			szTemp[2] = 0;
			strOut += szTemp;

			i++;
			continue;
		}
		else goto CHECKSTRING_ERROR;
	}
	return TRUE;

CHECKSTRING_ERROR:
	strOut += szERROR;
	return FALSE;
}

//�����ִ��г��ֵķǷ��ַ����й���
BOOL CUIStringSystem::_CheckStringFilter(const STRING& strIn)
{
	size_t allsize = m_vecFilterTable.size();
	for(INT i = 0; i < (INT)allsize; ++i)
	{
		if(STRING::npos != strIn.find(m_vecFilterTable[i]))	return FALSE; //���ַǷ��ַ���
	}

	return TRUE;
}

//��ȫƥ�����
BOOL CUIStringSystem::_CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType, BOOL bUseAllTable)
{
	BOOL bFind = FALSE;

	FULLCMP_MAP::iterator it = m_fullCompareTable.find("all");
	if(bUseAllTable && it != m_fullCompareTable.end())
	{
		UI_FULLCMP_TEMPLATE& strVec = it->second;
		if(strVec.find(strIn) != strVec.end()) bFind = TRUE;
	}

	it = m_fullCompareTable.find(strFilterType);
	if(!bFind && it != m_fullCompareTable.end())
	{
		UI_FULLCMP_TEMPLATE& strVec = it->second;
		if(strVec.find(strIn) != strVec.end()) bFind = TRUE;
	}

	return !bFind;
}

// ����ʱת��
// ��������ת�����Ѻ�����ַ�ת����utf32�����Ұ�stringҲת����utf32
VOID CUIStringSystem::ParserString_Runtime(const STRING& strSourceNotCheck, CEGUI::String& strOutUtf32)
{

	return ParserString_RuntimeNew(strSourceNotCheck, strOutUtf32);

	const CHAR KeyParserBegin	= '{';
	const CHAR KeyParserMiddle	= '=';
	const CHAR KeyParserEnd		= '}';

	STRING::size_type nValidBegin = 0;
	STRING strOutMBCS = "";

	//����ַ����Ϸ���
	STRING strSource;
	_CheckStringCode(strSourceNotCheck, strSource);
	
	//����������Ϣ�еķǷ���Ϣ
	if(FALSE == _CheckStringFilter(strSource))
		strSource = "#R��ע������Դǣ���";

	do
	{
		if(nValidBegin >= strSource.size()) break;

		STRING::size_type nValidEnd = strSource.find(KeyParserBegin, nValidBegin);

		//���һ������ֵ
		if(nValidEnd == STRING::npos) 
		{
			strOutMBCS = strSource.substr(nValidBegin);

			std::string strTemp;
			CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
			strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
			break;
		}

		//��������ֵ
		if(nValidEnd != nValidBegin) 
		{
			strOutMBCS = strSource.substr(nValidBegin, nValidEnd-nValidBegin);

			std::string strTemp;
			CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
			strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
		}

		STRING::size_type nKeyStart = nValidEnd+1;									//"{"
		STRING::size_type nKeyMiddle = strSource.find(KeyParserMiddle, nKeyStart);	//"="
		STRING::size_type nKeyEnd = strSource.find(KeyParserEnd, nKeyStart);		//"}"
		
		//�Ϸ���Key����
		if(nKeyEnd != STRING::npos && nKeyMiddle != STRING::npos && nKeyEnd > nKeyMiddle)
		{
			STRING strKey_Left = strSource.substr(nKeyStart, nKeyMiddle-nKeyStart);
			STRING strKey_Right = strSource.substr(nKeyMiddle+1, nKeyEnd-nKeyMiddle-1);

			if(strKey_Left == "ID")
			{
				std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strKey_Right);
				//�ҵ�key
				if(isValidKey.first)
				{
					//�����keyת��������ֵ
					strOutMBCS += isValidKey.second->second;

					nValidBegin = nKeyEnd+1;

					std::string strTemp;
					CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
					strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());

					strOutMBCS = "";
					continue;
				}
			}
			else if(strKey_Left == "PIC") // ���鶯��
			{
				// ����Ѿ�������
			/*	CODE_CONVERT::iterator  pos = m_mapCodeConvertTable.find( strKey_Right );
//				if( pos != m_mapCodeConvertTable.end() )
				{
					CEGUI::utf32 val = atoi(strKey_Right.c_str());
					val |= 0xFA000000;
					strOutUtf32 += val;
					nValidBegin = nKeyEnd + 1;
					continue;
				}*/
			}
			else if( strKey_Left == "COL" ) // ������ɫ
			{
				CEGUI::utf32 val = 0;
				sscanf( strKey_Right.c_str(), " %8X", &val);
				val = 0xFB000000 | ( val & 0x00FFFFFF );
				strOutUtf32 += val;
				nValidBegin = nKeyEnd + 1;
				continue;
			}
			else if( strKey_Left == "BAK" ) // ���屳��ɫ
			{
				CEGUI::utf32 val = 0;
				sscanf( strKey_Right.c_str(), " %8X", &val);
				val = 0xFC000000 | ( val & 0x00FFFFFF );
				strOutUtf32 += val;
				nValidBegin = nKeyEnd + 1;
				continue;
			}
		}
		strOutMBCS = strSource.substr(nValidEnd, nKeyEnd-nValidEnd);

		std::string strTemp;
		CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
		strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());

		nValidBegin = nKeyEnd;
	}while(true);
	
}

VOID CUIStringSystem::ParserString_RuntimeNew(const STRING& strSourceNotCheck, CEGUI::String& strOutUtf32)
{
	/*
	#R		��ʾ���������Ϊ��ɫ(red)
	#G		��ʾ���������Ϊ��ɫ(green)
	#B		��ʾ���������Ϊ��ɫ(blue)
	#K		��ʾ���������Ϊ��ɫ(black)
	#Y		��ʾ���������Ϊ��ɫ(yellow)
	#W		��ʾ���������Ϊ��ɫ(white)
	#b		��ʾ���������Ϊ��˸(blink)
	#c		+ �������ֻ���A-F��ĸ �Զ�����ɫ�����磺c008000=����ɫ
	#e		+ �������ֻ���A-F��ĸ �Զ�����ɫ�����磺e008000=����ɫ,��ʾ������չ�ߵ�ɫ
	#u		+ ���� + #u �������»��ߡ�
	#n		��������״̬�ָ�������
	#r		���ֻ��С�
	##		���һ��#�š�
	#aB		��ʾ�������һ�������ӣ����ǳ����Ӳ��ֵĿ�ʼ
	#aE		��ʾ�������һ�������ӣ����ǳ����Ӳ��ֵĽ���

	#{IDSTR}  ����滻�ַ���

	#0~99	  ����
	#-01~-99	������ʹ�ã������������Ķ���
	*/

	const CHAR			KeyParser			= '#';
	const CHAR			KeyParserIDBegin	= '{';
	const CHAR			KeyParserIDEnd		= '}';
	STRING::size_type	nValidBegin			= 0;
	STRING				strOutMBCS			= "";

	BOOL				bBlink				= FALSE;

	//����ַ����Ϸ���
	STRING strSource;
	_CheckStringCode(strSourceNotCheck, strSource);

	//����������Ϣ�еķǷ���Ϣ
	if(FALSE == _CheckStringFilter(strSource))
		strSource = "#R��ע������Դǣ���";

	do
	{
		if(nValidBegin >= strSource.size()) break;

		STRING::size_type nValidEnd = strSource.find(KeyParser, nValidBegin);

		//���һ������ֵ
		if(nValidEnd == STRING::npos) 
		{
			strOutMBCS = strSource.substr(nValidBegin);
			std::string strTemp;
			CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
			strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
			break;
		}

		//��������ֵ
		if(nValidEnd != nValidBegin) 
		{
			strOutMBCS = strSource.substr(nValidBegin, nValidEnd-nValidBegin);
			std::string strTemp;
			CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
			strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
		}

		STRING::size_type nKeyStart = nValidEnd+1;	//"#"�����Ǹ��ַ���λ��
		STRING::size_type nKeyEnd;					//��һ����ָ���ַ���λ��							

		//�Ϸ���Key����
		if(nKeyStart != STRING::npos )
		{
			STRING strKey_Opt	= strSource.substr(nKeyStart, 1);
			CEGUI::utf32 val = 0;
			if( strKey_Opt == "R" )
			{//#R��ɫ
				val = 0xFBFF0000;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "G" )
			{//#G��ɫ
				val = 0xFB00FF00;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "B" )
			{//#B��ɫ
				val = 0xFB0000FF;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "K" )
			{//#K��ɫ
				val = 0xFB000000;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "Y" )
			{//#Y��ɫ
				val = 0xFBFFFF00;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "W" )
			{//#W��ɫ
				val = 0xFBFFFFFF;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "b" )
			{//#b��˸
				val = 0xE0000000;
				strOutUtf32 += val;
				nKeyEnd = nKeyStart+1;

				bBlink = TRUE;
			}
			else if( strKey_Opt == "c" )
			{//#c�Զ�����ɫ
				STRING strColor = strSource.substr(nKeyStart+1, 6);
				CEGUI::utf32 val = 0;
				sscanf( strColor.c_str(), " %6X", &val);
				val = 0xFB000000 | ( val & 0x00FFFFFF );
				strOutUtf32 += val;
				nKeyEnd = nKeyStart + 7;
			}
			else if( strKey_Opt == "e" )
			{//#e�Զ�����չ����ɫ
				STRING strColor = strSource.substr(nKeyStart+1, 6);
				CEGUI::utf32 val = 0;
				sscanf( strColor.c_str(), " %6X", &val);
				val = 0xFC000000 | ( val & 0x00FFFFFF );
				strOutUtf32 += val;
				nKeyEnd = nKeyStart + 7;
			}
			else if( strKey_Opt == "u" )
			{//#u�»���
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "n" )
			{//#n�ظ�״̬
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "r" )
			{//#r�س�����
				strOutMBCS = "\n";
				std::string strTemp;
				CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
				strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "#" )
			{//##
				strOutMBCS = "#";
				std::string strTemp;
				CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
				strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
				nKeyEnd = nKeyStart+1;
			}
			else if( strKey_Opt == "a")
			{//HyperLink #aB{linktxt}showtxt#aE
				STRING strManipulate = strSource.substr(nKeyStart+1, 1);
				if(strManipulate == "B")//#aB
				{
					val = 0xFD000000;
					strOutUtf32 += val;
					nKeyEnd = nKeyStart+2;
				}
				else if(strManipulate == "E")//#aE
				{
					val = 0xFE000000;
					strOutUtf32 += val;
					nKeyEnd = nKeyStart+2;
				}
				else
				{
					//�Ƿ�HyperLink#a�ִ�
					nKeyEnd = nKeyStart+2;
				}
			}
			else if( strKey_Opt.c_str()[0] == KeyParserIDBegin )
			{//#{IDSTR} == ���
				STRING::size_type nIDBegin	= nKeyStart+1;
				STRING::size_type nIDEnd	= strSource.find(KeyParserIDEnd, nIDBegin);

				if(nIDEnd != STRING::npos)
				{
					STRING strID = strSource.substr(nIDBegin, nIDEnd-nIDBegin);

					if(strID.substr(0,5) == "_ITEM")
					{//#{_ITEM10100005}
						STRING szTBindex = strID.substr(5,8);
						UINT	TBindex = atoi(szTBindex.c_str());
						strOutMBCS	=	g_pObjectSystem->ItemNameByTBIndex(TBindex);
						std::string strTemp;
						CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
						strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
					}
					if(strID.substr(0,5) == "_TIME")
					{//#{_TIME0507211233}
					 //���磺0507211233 ��ʾ "2005��07��21�� 12:33"
						STRING szTime	= strID.substr(5,10);
						UINT	uTime	= atoi(szTime.c_str());

						INT min,hour,day,month,year;

						min = uTime % 100;
						uTime /= 100;

						hour = uTime % 100;
						uTime /= 100;

						day = uTime % 100;
						uTime /= 100;

						month = uTime % 100;
						uTime /= 100;

						year = 2000 + uTime % 100;
						//uTime /= 100;

						CHAR szFormat[] = "%d��%d��%d�� %d:%d";
						CHAR szDate[32] = {0};
						_snprintf(szDate,sizeof(szDate), szFormat, year,month,day,hour,min);
						strOutMBCS = szDate;

						std::string strTemp;
						CUIStringSystem::mbcs_to_utf8(strOutMBCS, strTemp);
						strOutUtf32 += (CEGUI::String)(CEGUI::utf8*)(strTemp.c_str());
					}
					//#{_MONEY8583045}
					if(strID.substr(0,6) == "_MONEY")
					{
						INT nMoney;
						sscanf(strID.c_str(),"_MONEY%d",&nMoney);

						INT nGoldCoin = nMoney / 10000;
						INT nSilverCoin = (nMoney % 10000) / 100;
						INT nCopperCoin = nMoney % 100;
						
						CHAR szData[128] = {0};

						if(nGoldCoin != 0)
						{
							_snprintf(szData, sizeof(szData), "%d#-02", nGoldCoin);
						}
						if(nSilverCoin != 0)
						{
							_snprintf(szData, sizeof(szData), "%s%d#-03", szData, nSilverCoin);
						}
						if(nCopperCoin != 0)
						{
							_snprintf(szData, sizeof(szData), "%s%d#-04", szData, nCopperCoin);
						}

						strOutMBCS = szData;

						CEGUI::String strTemp;
						ParserString_RuntimeNew(strOutMBCS, strTemp);

						strOutUtf32 += strTemp;
					}
					else
					{//��������������
						std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strID);
						//�ҵ�key
						if(isValidKey.first)
						{
							//�����keyת��������ֵ
							strOutMBCS = isValidKey.second->second;
							CEGUI::String strOutTemp;
							//�ݹ����
							ParserString_RuntimeNew(strOutMBCS, strOutTemp);
							strOutUtf32 += strOutTemp;
						}
					}

					nKeyEnd = nIDEnd+1;
				}
				else
				{
					//�Ƿ�#{��Ҫ���˵�
					nKeyEnd = nKeyStart + 2; 
				}
			}
			else if( strKey_Opt == "-")
			{//#-01~-99(��������λ��ֵ��һλ��ֵ��Ƿ�ֵ) ������ʹ�õı���������������ַ�������ʹ�á�
				STRING strNum = strSource.substr(nKeyStart+1,2);
				if((strNum.size() == 2) &&
				   (strNum.at(0) >= 48 && strNum.at(0) <= 57) &&
				   (strNum.at(1) >= 48 && strNum.at(1) <= 57))
				{
					UINT nNum;
					sscanf(strNum.c_str(), " %u", &nNum);
					
					if(1 != nNum)
					{
						CEGUI::utf32 val = nNum;
						val |= 0xE3000000;
						strOutUtf32 += val;

						nKeyEnd = nKeyStart + 3;
					}
					else
					{
						// -1�ǷǷ�������ʶ�������-2��ʼ [4/24/2006]
						//���Ե�#-
						nKeyEnd = nKeyStart + 1;
					}
				}
				else
				{//�Ƿ�����
					//���Ե�#-
					nKeyEnd = nKeyStart + 1;
				}
			}
			else if( strKey_Opt.c_str()[0] >= 48 && strKey_Opt.c_str()[0] <= 57 )
			{//#0~99 ����
				UINT	nDigital	= strKey_Opt.c_str()[0] - 48;
				STRING strDigital	= strSource.substr(nKeyStart+1, 1);
			
				if(strDigital.c_str()[0] >= 48  && strDigital.c_str()[0] <= 57 )
				{//��λ����
					nDigital = nDigital*10 + (strDigital.c_str()[0] - 48);
					nKeyEnd = nKeyStart + 2;
				}
				else
				{//һλ����
					nKeyEnd = nKeyStart + 1;
				}

//				CODE_CONVERT::iterator  pos = m_mapCodeConvertTable.find( nDigital );
//				if( pos != m_mapCodeConvertTable.end() )
				{
					CEGUI::utf32 val = nDigital;
					val |= 0xFA000000;
					strOutUtf32 += val;
				}
			}
			else
			{//�Ƿ�����
				//���Ե�#
				nKeyEnd = nKeyStart;
			}
		}
		nValidBegin = nKeyEnd;
	}while(true);

	if(bBlink)
	{
		CEGUI::utf32 val = 0xE1000000;
		strOutUtf32 += val;
	}

}

#define	SELFMSG_MAX_LENGTH 2048
//���ݴ����ϵͳ�ִ�ID�����ֵ�����Ҹ�ʽ�ַ���
STRING CUIStringSystem::ParserString_VarParam(const STRING &strID, va_list ptr)
{
	STRING strOut = "";

	std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strID);
	if(isValidKey.first)
	{
		if(ptr)
		{
			STRING strFormat = isValidKey.second->second;
			CHAR  result[SELFMSG_MAX_LENGTH+1];
			memset(result,0,SELFMSG_MAX_LENGTH+1);

			//��ʽ������ַ���
			_vsnprintf(result, SELFMSG_MAX_LENGTH, strFormat.c_str(), ptr);

			strOut = result;
		}
	}

	return strOut;
}

STRING CUIStringSystem::ParserString_NoVarParam(const STRING& strID)
{
	STRING strOut = "";

	std::pair< BOOL, DIRECTIONARY::iterator > isValidKey = _IsValidDictionKey(strID);
	if(isValidKey.first)
	{
		strOut = isValidKey.second->second;
	}

	return strOut;
}

#undef SELFMSG_MAX_LENGTH

VOID CUIStringSystem::ParserString_NoColor(const STRING& strSrc, STRING& strOut, BOOL bControl)
{
	const CHAR KeyStart = '#';
	strOut.clear();
	
	STRING::size_type sB = 0;
	STRING::size_type sE = strSrc.find_first_of(KeyStart);
	STRING::size_type sLen = strSrc.size();

	do
	{	
		if(sE == STRING::npos)
		{
			//copy last
			strOut += strSrc.substr(sB);
			break;
		}
		
		//copy normal
		strOut += strSrc.substr(sB, sE-sB);

		//get op
		STRING strOp = strSrc.substr(sE+1, 1);

		//skip color
		if(strOp == "R" || strOp == "G" || strOp == "B" ||
		   strOp == "K" || strOp == "Y" || strOp == "W")
		{
			sE += 2;
		}
		else if(strOp == "c" || strOp == "e")
		{
			sE += 8;
		}
		else if(bControl && (strOp == "b" || strOp == "u" || strOp == "r" || strOp == "n"))
		{
			sE += 2;
		}
		else
		{
			//not color means
			strOut += strSrc.at(sE);	//same as --> strOut += KeyStart;
			sE += 1;
		}

		if(sE >= sLen) break;

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strSrc.find(KeyStart, sB);

	}while(TRUE);
}

STRING CUIStringSystem::getTalkTemplate(const STRING& strKey, UINT index)
{
	STRING strOut = "";

	if(!(strKey.empty()))
	{
		TALK_TEMPLATE_MAP::iterator itFind = m_talkTemplateTable.find(strKey);
		if(itFind != m_talkTemplateTable.end() && index < (itFind->second).size())
		{
			strOut = (itFind->second)[index];
		}
	}

	return strOut;
}

STRING CUIStringSystem::getTalkRandHelpMsg()
{
	TALK_TEMPLATE_MAP::iterator itFind = m_talkTemplateTable.find(TALK_HELPSTRING_KEY);
	if(itFind != m_talkTemplateTable.end())
	{
		UINT idx = abs(rand()%(INT)((itFind->second).size()));
		return (itFind->second)[idx];
	}
	return "";
}

void CUIStringSystem::mbcs_to_utf8(const std::string& strIn, std::string& strOut)
{
	//�������Ի���
	AUTO_LOCALE_CONVERT theAutoConvert;
	strOut = utf::mbcs_to_utf8(strIn);
}

void CUIStringSystem::utf8_to_mbcs(const std::string& strIn, std::string& strOut)
{
	//�������Ի���
	AUTO_LOCALE_CONVERT theAutoConvert;
	strOut = utf::utf8_to_mbcs(strIn);
}

VOID CUIStringSystem::GenerateFullCompareTable()
{
	tDataBaseSystem* pUIDBC = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(pUIDBC);
	const tDataBase* pDBCFullCmp = pUIDBC->GetDataBase(DBC_FULLCMP_FILTER);
	TDAssert(pDBCFullCmp);
	for(INT i=0; i<(INT)pDBCFullCmp->GetRecordsNum(); i++)
	{
		const _DBC_FULLCMP_FILTER* pLine = (const _DBC_FULLCMP_FILTER*)((tDataBase*)pDBCFullCmp)->Search_LineNum_EQU(i);
		FULLCMP_MAP::iterator itFind = m_fullCompareTable.find(pLine->szFilterType);

		//�����µ�����
		if(itFind == m_fullCompareTable.end())
		{
			UI_FULLCMP_TEMPLATE	talkTemplate;

			m_fullCompareTable.insert(std::make_pair(pLine->szFilterType, talkTemplate));
			itFind = m_fullCompareTable.find(pLine->szFilterType);
		}

		//����ִ�
		UI_FULLCMP_TEMPLATE& strVec = itFind->second;
		strVec.insert(pLine->szFullTxt);
	}
}

VOID CUIStringSystem::GenerateTalkTemplateTable()
{
	DIRECTIONARY::iterator it = m_theDirectionary.begin();

	UI_TALK_TEMPLATE	talkTemplate;
	STRING				talkKey;

	do
	{
		//������ģ�巢��
		if("TALK_" == (it->first).substr(0,5))
		{
			talkKey = (it->first).substr(5, (it->first).find("_", 6)-(it->first).find("_") -1);

			TALK_TEMPLATE_MAP::iterator itTemplate = m_talkTemplateTable.find(talkKey);
			if(itTemplate == m_talkTemplateTable.end())
			{
				INT idmax = (TALK_HELPSTRING_KEY == talkKey)?TALK_HELPSTRING_MAX:TALK_TEMPLATE_MAX;
				for(INT id = 0; id < idmax; ++id)
				{
					CHAR num[4] = {0};
					_snprintf(num,4,"%03d",id+1);

					STRING strFind = "TALK_" + talkKey + "_" + num;

					DIRECTIONARY::iterator itFind = m_theDirectionary.find(strFind);
		
					if(itFind != m_theDirectionary.end())
					{
						talkTemplate.push_back(itFind->second);
                		m_theDirectionary.erase(itFind);
					}
					else
					{
						if(idmax == TALK_TEMPLATE_MAX)
							talkTemplate.push_back("");	//not found insert an empty string.
					}
				}

				m_talkTemplateTable.insert(std::make_pair(talkKey, talkTemplate));

				talkKey.clear();
				talkTemplate.clear();
				it = m_theDirectionary.begin();
			}
			else
				m_theDirectionary.erase(it++); //˵������ģ�����д��ʽ����ɾ�������¼
		}
		else
			it++;
	}
	while(it != m_theDirectionary.end());
}

STRING CUIStringSystem::ModifyChatTemplate(const STRING& strSrc, const STRING& strTalker, const STRING& strTarget)
{
	const CHAR KeyStart = '$';

	STRING::size_type sB = 0;
	STRING::size_type sE = strSrc.find_first_of(KeyStart);
	STRING::size_type sLen = strSrc.size();

	STRING strOut;
	strOut.clear();

	do
	{	
		if(sE == STRING::npos)
		{
			//copy last
			strOut += strSrc.substr(sB);
			break;
		}
			
		//copy normal
		strOut += strSrc.substr(sB, sE-sB);

		//get op
		STRING strOp = strSrc.substr(sE+1, 1);

		//replace talker
		if(strOp == "N")
		{
			strOut += strTalker;
			sE += 2;
		}
		else if(strOp == "n") //replace target
		{
			strOut += strTarget;
			sE += 2;
		}
		else
		{
			//ignore wrong '$'
			//strOut += strSrc.at(sE);
			sE += 1;
		}

		if(sE >= sLen) break;

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strSrc.find(KeyStart, sB);

	}while(TRUE);

	return strOut;
}
