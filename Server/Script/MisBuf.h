//-----------------------------------------------------------------------------
// �ļ��� : MisBuf.h
// ģ��	:	Script
// ����	 :	����ϵͳ�õĳ��������ڵ����ݿ�
// �޸���ʷ:
//-----------------------------------------------------------------------------
#ifndef __MISBUF__H_
#define __MISBUF__H_


typedef struct _MisBuf
{
	enum 
	{
		MISSIONBUFLEN = 64,
		MISSIONCHARBUFLEN = MISSIONBUFLEN * 4 * 8,//��Ϊ�ͻ��˵�bug,���ԷŴ�buf����
	};

	typedef struct _MisItemBuf
	{
		//���buf�洢�˷��͸��ͻ��˵��ַ���Ϣ��,�ͻ�����Ҫע���ַ���������
		_MisItemBuf()
		{
			ResetBuf();
		}
		
		CHAR buf[MISSIONCHARBUFLEN];
		enum BufType
		{
			BufType_Int,
			BufType_Str,
			BufType_IntStr,
			BufType_Money,
			BufType_Item,
			BufType_RandItem,
			BufType_RadioItem,
			BufType_Skill
		}mType;	
		INT m_BufUse;
		VOID ResetBuf(void)
		{

			m_BufUse = 0;
			memset(buf,0,MISSIONCHARBUFLEN);
		}
		VOID SetBufUse(INT NewBufUse){
			m_BufUse += NewBufUse;
		}
		INT GetBufUse(void){
			return m_BufUse;
		}
		INT GetBufMaxSize(){
			return MISSIONCHARBUFLEN;
		}
		INT AddBuf(VOID* vpNewBuf, INT nNewBufLen){
			if (m_BufUse+nNewBufLen > MISSIONCHARBUFLEN)
			{
				return 0;
			}
			memcpy(buf+m_BufUse, (VOID*)(vpNewBuf), nNewBufLen);
			m_BufUse += nNewBufLen;
			return m_BufUse;
		}
	}MisItemBuf[MISSIONBUFLEN];

	MisItemBuf Buff;
	INT mUseNum;

	VOID ResetMisBuf()
	{
		//��Ϊ����Q_strncyz,���Բ�������0
		//memset(&Buff,0,sizeof(Buff));

		mUseNum = 0;
		for (UINT i = 0; i < MISSIONBUFLEN; i++)
		{
			Buff[i].ResetBuf();
		}
	}
}MisBuf;

#endif