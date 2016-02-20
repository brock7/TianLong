#ifndef GAMESTRUCT_TEAM_H
#define GAMESTRUCT_TEAM_H

#include "DB_Struct.h"

class SimpleImpactList
{
	INT						m_nNumOfImpacts;				// BUFF ����
	ImpactID_t				m_aBuffID[MAX_IMPACT_NUM];		// BUFF �б�

public:
	SimpleImpactList()		{ CleanUp(); }

	VOID					CleanUp() { m_nNumOfImpacts = 0; }

	INT						GetImpactsCount() { return m_nNumOfImpacts; }

	VOID					AddImpact( ImpactID_t impact )
	{
		Assert( m_nNumOfImpacts <= MAX_IMPACT_NUM );
		m_aBuffID[m_nNumOfImpacts++] = impact;
	}

	VOID					RemoveImpact( ImpactID_t impact )
	{
		Assert( m_nNumOfImpacts > 0 );
		for( INT i=0; i<m_nNumOfImpacts; ++i )
		{
			if( m_aBuffID[i] == impact )
			{
				--m_nNumOfImpacts;

				for( INT j=i; j<m_nNumOfImpacts; ++j )
				{
					m_aBuffID[j] = m_aBuffID[j+1];
				}

				return;
			}
		}
	}

	VOID					SetImpactList( const SimpleImpactList* pSimpleImpactList )
	{
		memcpy( this, pSimpleImpactList, sizeof(*this) );
	}

	VOID					SetImpactList( const _IMPACT_LIST* pImpactList )
	{
		m_nNumOfImpacts = pImpactList->m_Count;
		for( INT i=0; i<m_nNumOfImpacts; ++i )
		{
			m_aBuffID[i] = pImpactList->m_aImpacts[i].GetImpactID();
		}
	}

	BOOL					operator==( const _IMPACT_LIST& ImpactList ) const
	{
		if( m_nNumOfImpacts != ImpactList.m_Count )
		{
			return FALSE;
		}

		for( INT i=0; i<ImpactList.m_Count; ++i )
		{ // �����Ƿ� Buff �б��Ѿ��ı䣬ÿ����ǰ ID ��ȥ Cache �бȽ�һ���Ƿ����
			INT j;
			for( j=0; j<m_nNumOfImpacts; ++j )
			{
				if( ImpactList.m_aImpacts[i].GetImpactID() == m_aBuffID[j] )
				{
					break;
				}
			}

			if ( j >= m_nNumOfImpacts )
			{ // ��� ID ������
				return FALSE;
			}
		}

		return TRUE;
	}

	BOOL					operator!=( const _IMPACT_LIST& ImpactList ) const
	{
		return (*this == ImpactList) == FALSE;
	}
};

#endif // GAMESTRUCT_TEAM_H
