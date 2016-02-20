/************************************\
*									*
*			Tooltips�ӿ�			*
*									*
\************************************/


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}
class CActionItem;

//
//�ű�����ɳ�䣬��Щ����Ӧ���ɽű�����
//
namespace SCRIPT_SANDBOX
{

class SuperTooltips
{
public:

	//��ѯTooltip��Title
	//
	// ������� 
	//   
	// ��������
	//   0[String] - Title
	//
	INT		GetTitle(LuaPlus::LuaState* state);

	//��ѯTooltip��Icon
	//
	// ������� 
	//   
	// ��������
	//   0[String] - Image(set:imageset image:imagename)
	//
	INT		GetIcon(LuaPlus::LuaState* state);

	//��ѯTooltip����ϸ�߼�����
	//
	// ������� 
	//   0[String] - AttributeName
	//   
	// ��������
	//   0[String] - AttributeValue
	//
	INT		GetAttributeValue(LuaPlus::LuaState* state);

	//��ѯTooltip������˵��
	//
	//
	//   
	// ��������
	//   0[String] - �����ַ���
	//
	INT		GetItemDesc(LuaPlus::LuaState* state);

	//��ѯ��Ʒ���㴫�Ͳ���

	INT GetItemTransferParam( LuaPlus::LuaState* state);
	INT GetItemIndex( LuaPlus::LuaState* state);
	//��ѯTooltip����Ʒ�ȼ�
	//
	//
	//   
	// ��������
	//   0[int] - -1: �����Ʒû���������
	//
	INT		GetItemLevel(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ�;�
	//
	//
	//   
	// ������������
	//   0[int] - ��ǰ�;� -1: �����Ʒû���������
	//   1[int] - ����;� -1: �����Ʒû���������
	//
	INT		GetItemDur(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ���������
	//
	//
	//   
	// ��������1��
	//   0[int] - ������� -1: �����Ʒû���������
	//   
	//
	INT		GetItemRepairCount(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ�İ���Ϣ
	//
	//
	//   
	// ��������1��
	//   0[int] - ������� -1: �����Ʒû���������
	//   
	//
	INT		GetItemBindInfo(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ������npc�ļ۸�
	//
	//
	//   
	// ��������1��
	//   0[int] - ������� -1: �����Ʒû���������
	//   
	//
	INT		GetItemPrice(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ��������
	//
	//
	//   
	// ��������1��
	//   0[int] - ������� -1: �����Ʒû���������
	//   
	//
	INT		GetItemManufacturer(LuaPlus::LuaState* state);

	//��ѯTooltip����Ʒ�İ�ɫ����
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//
	INT		GetItemBaseWhiteAttrInfo(LuaPlus::LuaState* state);

	
	//��ѯTooltip����Ʒ����ɫ����
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//
	INT		GetItemExtBlueAttrInfo(LuaPlus::LuaState* state);


	//��ѯTooltip����Ʒ����ɫ����
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//
	INT		GetItemGreenAttrInfo(LuaPlus::LuaState* state);


	//��ѯTooltip����Ʒ������
	//
	//
	//   
	// ��������1��
	//   0[str] - "": �����Ʒû���������
	//   
	//
	INT	GetItemType(LuaPlus::LuaState* state);


	//��ѯTooltip��װ����Ʒ��(��, ��, ��)
	//
	//
	//   
	// ��������1��
	//   0[int] - 1 : ��
	//            2 : ��
    //            3 : ��
	INT	GetItemEquipQuantity(LuaPlus::LuaState* state);


	//��ѯTooltip��װ������������
	//
	//
	//   
	// ��������1��
	//   0[int] - ��Ʒ����
	INT	GetItemClassType(LuaPlus::LuaState* state);


	

	//
	// �õ�����������
	// ��������1��
	// 0[int] - ����������
	INT	GetEquipMaxRepair(LuaPlus::LuaState* state);


	
	//��ѯTooltip��װ���ı�ʯ��Ϣ
	//
	//
	//   
	// ��������15��
	//   0[int] - ��ʯ����
	//   1[str] - ��ʯ1��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 2[str] - ��ʯ2��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 3[str] - ��ʯ2��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 4[str] - ��ʯ�������Ե�����
	INT	GetGemInEquipInfo(LuaPlus::LuaState* state);
	

	INT GetItem_IsProtected(LuaPlus::LuaState* state);

	//----------------------------------------------------------------------------------------------------------------------
	// �ķ�����
	//
	
	// �õ��ķ��ȼ�
	// ��������1��
	// 0[int] - ��ǰѧ����ķ��ȼ�.
	//INT	GetXinfaLevel(LuaPlus::LuaState* state);


	//----------------------------------------------------------------------------------------------------------------------
	// ���ܺ���
	//

	// �õ����ܵȼ�
	// ��������1��
	// 0[int] - ��ǰѧ��ļ��ܵȼ�.
	INT	GetSkillLevel(LuaPlus::LuaState* state);

	// �õ��Ƿ�ѧ�Ἴ��
	// ��������1��
	// 0[int] - ��ǰѧ��ļ��ܵȼ�.
	INT	GetIsLearnedSkill(LuaPlus::LuaState* state);

	// �õ���������
	// ��������1��
	// 0[str] - �õ���������
	INT	GetSkillDesc(LuaPlus::LuaState* state);

	// �õ����ܶ�Ӧ���ķ��ȼ�
	//
	// ��������1��
	// 0[int] - �ķ��ȼ�
	//INT	GetSkillXinfaLevel(LuaPlus::LuaState* state);




	//----------------------------------------------------------------------------------------------------------------------
	//
	// ��ʯ��Ϣ
	//

	// �õ���ʯ�ȼ�
	// ��������1��
	// 0[int] - ��ʯ�ȼ�
	INT	GetGemLevel(LuaPlus::LuaState* state);

	// �õ���ʯ����
	// ��������1��
	// 0[str] - ��ʯ����
	INT	GetGemArribInfo(LuaPlus::LuaState* state);


	//----------------------------------------------------------------------------------------------------------------------
	//
	// �õ���Ʒ��quality
	//

	// ��������1��
	// 0[int] - ҩƷ��ʯƷ��
	INT	GetItemQuality(LuaPlus::LuaState* state);


	// �õ���Ʒ�ڱ��е�����
	// ��������1��
	// 0[int] - ���е����͡�
	INT	GetItemTableType(LuaPlus::LuaState* state);


	
	// �õ��䷽��Ҫ�ļ������֡�
	// ��������1��
	// 0[int] - ���е����͡�
	INT	GetPrescrSkillName(LuaPlus::LuaState* state);

	// �õ��䷽��Ҫ�ļ��ܵȼ���
	// ��������1��
	// 0[int] - ���еĵȼ���
	INT	GetPrescrNeedSkillLevel(LuaPlus::LuaState* state);


	// �õ��䷽��Ҫ���ܵĵ�ǰ�ȼ���
	// ��������1��
	// 0[int] - �䷽��Ҫ�ĵ�ǰ�ȼ���
	INT	GetPrescrNeedSkillCurLevel(LuaPlus::LuaState* state);


	// �����������item����ϸ��Ϣ
	// 
	// 
	INT	SendAskItemInfoMsg(LuaPlus::LuaState* state);

	
	// �õ���ɫװ���Ƿ����
	// 
	// 
	INT	IsBlueEquipIdentify(LuaPlus::LuaState* state);

	//
	// npc�̵��Ƿ�򿪡�
	// 
	INT	IsNpcShopOpen(LuaPlus::LuaState* state);

	//�Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);

	//�õ�������Ϣ2006��4��48
	INT GetTypeDesc(LuaPlus::LuaState* state);

	//�õ���Ʒ�Ƿ��ڱ�����2006-5-10
	INT IsItemInPacket(LuaPlus::LuaState* state);

	//
	// �õ��Ƿ���ʾ�۸� 2006-5-18
	//
	INT IsShowPrice(LuaPlus::LuaState* state);

	//
	// �õ���ʾ�۸� 2006-5-18
	//
	INT GetShowPrice(LuaPlus::LuaState* state);

	//
	// �õ���Ʒ���εȼ����� 2006-5-18
	//
	INT GetItemLevelDesc(LuaPlus::LuaState* state);

	// 
	// �õ���λ��������Ʒ�Ĳ���
	INT GetAnchorValue( LuaPlus::LuaState* state);

	// �õ���װ���Ժ���
	// �Ƿ�����װ
	INT IsItemSuit(  LuaPlus::LuaState* state );
	// �õ���װ������
	INT GetItemSuitNumber( LuaPlus::LuaState* state );
	//// �ж��Լ��ǲ����еڼ�����װ����оͷ������֣�û�оͷ��ؿ��ַ���
	INT IsItemExist( LuaPlus::LuaState* state );
	// �õ��ڼ�����װ������,
	//INT GetSuitName( LuaPlus::LuaState* state );
	// �õ���װ������
	INT GetSuitAtt( LuaPlus::LuaState* state );
	//
	// �õ���Ʊ�ļ�ֵ����
	INT GetTwoMoneyValue(LuaPlus::LuaState* state);

	// �õ���Ʒ����ʾ��ɫ
	INT GetSysColour(LuaPlus::LuaState* state);
public:
	//����Ϸ�߼����ü����е�ActionItem
	VOID	SetActionItem(CActionItem* pActionitem);
	CActionItem* GetActionItem(void) { return m_pActionItem; }

protected:

	INT			 m_nActionId;
	CActionItem* m_pActionItem;

public:
	SuperTooltips();
	virtual ~SuperTooltips();

	static SuperTooltips s_SupperTooltip;
	static LuaPlus::LuaObject* s_pMetaTable;
};




}