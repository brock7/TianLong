/************************************\
*									*
*			��Ұ����ӿ�			*
*									*
\************************************/

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}


//
//�ű�����ɳ�䣬��Щ����Ӧ���ɽű�����
//
namespace SCRIPT_SANDBOX
{

class PlayerPackage
{
public:

	//��ѯ���������е���Ʒ
	//
	// ������� 
	//		0[String]	- ��������
	//						"base"		������Ʒ
	//						"material"	������Ʒ
	//						"quest"		������Ʒ
	//
	//		1[Int]		- ��������[1, 20]
	//
	// ��������
	//   0 - ActionItem
	//
	INT		EnumItem(LuaPlus::LuaState* state);
	// �򿪰�̯�����ۣ�����
	INT		OpenStallSaleFrame(LuaPlus::LuaState* state);

	//--------------------------------------------------------
	// �����Ʒ����ʹ�õĽӿں���
	INT		GetSplitSum(LuaPlus::LuaState* state);
	// ������ȷ����ֺ�Ĵ���
	INT		SplitItem(LuaPlus::LuaState* state);
	// ȡ����Ʒ���
	INT		CancelSplitItem(LuaPlus::LuaState* state);
	//������Ʒ
	INT		PackUpPacket(LuaPlus::LuaState* state);
	//��õȴ������Ʒ�ļ۸�
	INT		GetSplitName(LuaPlus::LuaState* state);
	//�򿪳������
	INT		OpenPetList(LuaPlus::LuaState* state);
	//����������
	INT		OpenLockFrame(LuaPlus::LuaState* state);
	//������Ʒ�����
	INT		Lock(LuaPlus::LuaState* state);
	//�����Ʒ�Ƿ�����
	INT		IsLock(LuaPlus::LuaState* state);

public:
	static PlayerPackage s_PlayerPackage;
	static LuaPlus::LuaObject* s_pMetaTable;
};

};