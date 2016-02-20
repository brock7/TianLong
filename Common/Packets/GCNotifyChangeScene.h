#ifndef __GC_NOTIFYCHANGESCENE_H__
#define __GC_NOTIFYCHANGESCENE_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNotifyChangeScene : public Packet
{
public:
	GCNotifyChangeScene( ){} ;
	virtual ~GCNotifyChangeScene( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NOTIFYCHANGESCENE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(SceneID_t) +
															sizeof(SceneID_t)+
															sizeof(WORLD_POS)+
															sizeof(BYTE)*2 ; }

public:
	//ʹ�����ݽӿ�
	ObjID_t			getCurrentSceneID( ){ return m_CurrentSceneID ; }
	VOID			setCurrentSceneID( SceneID_t id ){ m_CurrentSceneID = id ; }

	ObjID_t			getTargetSceneID( ){ return m_TargetSceneID ; }
	VOID			setTargetSceneID( SceneID_t id ){ m_TargetSceneID = id ; }

	WORLD_POS*		getTargetPos( ){ return &m_TargetPos ; }
	VOID			setTargetPos( const WORLD_POS* pPos ){ m_TargetPos = *pPos ; }

	BYTE			getTargetDir( ){ return m_TargetDir ; }
	VOID			setTargetDir( BYTE dir ){ m_TargetDir = dir ; }

	BYTE			getFlag( ){ return m_Flag ; }
	VOID			setFlag( BYTE bRelive ){ m_Flag = bRelive; }

private:
	SceneID_t		m_CurrentSceneID;		// ObjID
	SceneID_t		m_TargetSceneID;		// ObjID
	WORLD_POS		m_TargetPos ;			//Ŀ�곡��λ��
	BYTE			m_TargetDir ;			//Ŀ�곡������
	BYTE			m_Flag;

};


class GCNotifyChangeSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNotifyChangeScene() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NOTIFYCHANGESCENE; }
	UINT		GetPacketMaxSize()const { return	sizeof(SceneID_t) +
													sizeof(SceneID_t)+
													sizeof(WORLD_POS)+
													sizeof(BYTE)*2 ; }
};

class GCNotifyChangeSceneHandler 
{
public:
	static UINT Execute( GCNotifyChangeScene* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif