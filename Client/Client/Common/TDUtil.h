/****************************************\
*										*
*			  ���ߺ�����				*
*										*
\****************************************/
#pragma once

#include "TDBasicType.h"
#include "TDMath.h"

//====================================
//����Log
void	TDU_Log(LPCSTR pszFmt, ...);

//====================================
//�ַ������
//���ַ������չؼ��ַָ�
INT		TDU_ConvertStringToVector(LPCTSTR strStrintgSource, std::vector< STRING >& vRet, LPCTSTR szKey=_T("\\/"), BOOL bOneOfKey = TRUE, BOOL bIgnoreEmpty = TRUE);

//====================================
//��ѧ���
#define TDU_PI			(3.1415926535)
#define TDU_MINFLOAT	(1.0E-9)

//ȡ�������ľ���
FLOAT TDU_GetDist(const fVector2& fvPos1, const fVector2& fvPos2);
FLOAT TDU_GetDist(const fVector3& fvPos1, const fVector3& fvPos2);

//ȡ�������ľ���ƽ��
FLOAT TDU_GetDistSq(const fVector2& fvPos1, const fVector2& fvPos2);
FLOAT TDU_GetDistSq(const fVector3& fvPos1, const fVector3& fvPos2);

template<class T, class U, class V>	inline void TDU_Clamp(T &v, const U &min, const V &max)
{
	v = (v < min) ? min : v;
	v = (v > max) ? max : v;
}

/*
|
| �����������ĵ��
|
*/
inline float TDU_DotProduct(const fVector2& vec1, const fVector2& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

/*
|
|ȡ�ô�Pos1��Pos2��ʸ�������Y�����ת�Ƕ�, ��z��Ϊ0��
|   
|              ->x
|                  o pos1
|          |       |\
|       z  v       |  \
|                  |    \
|                  |      \
|                  |        o pos2
|
|  Return [0 ~ 2pi)
*/

FLOAT TDU_GetYAngle(const fVector2& fvPos1, const fVector2& fvPos2);


/*
|
|ȡ�ô�Pos1��Pos2֮������ĵ�
|        
|             ->x
|                    o pos2
|          |       /    
|       z  v      o <- This point!    
|               /    
|              o 
|             pos1        
|
*/
fVector3 TDU_GetCenter(const fVector3& fvPos1, const fVector3& fvPos2);


/*
|
|ȡ��v1����ڴ�Pos1��Pos2֮��ֱ�ߵľ����
|        
|             ->x
|                       o pos2
|        |  v1        /    
|     z  v    \     / 
|                \/     
|               /  \
|              o      \ 
|             pos1      v2  <- This point!    
|
*/
fVector2 TDU_GetReflect(const fVector2& fvPos1, const fVector2& fvPos2, const fVector2& v1);

/*
|
| ���ڴ��ж�ȡһ���ı�(�൱��fgets)
|
*/
const char* TDU_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd);

//����ַ����ķǷ��ַ�
//����ֵ��
//TRUE	�����зǷ��ַ�
//FALSE	�зǷ��ַ�
BOOL	TDU_CheckStringValid(const char* pStr);