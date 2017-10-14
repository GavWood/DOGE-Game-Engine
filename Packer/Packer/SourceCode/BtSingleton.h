////////////////////////////////////////////////////////////////////////////////
// BtSingleton.h

#pragma once
#include "BtBase.h"
#pragma warning( disable : 4311 4312 )

template <typename T> class BtSingleton
{
static T* m_pInstance;

public:

	BtSingleton( void )
	{
		BtAssert( !m_pInstance );
		m_pInstance = (T*)this;
	}

	~BtSingleton( void )
	{
		BtAssert( m_pInstance );
		m_pInstance = 0;
	}

	static T& Instance( void )
	{
		BtAssert( m_pInstance );
		return ( *m_pInstance );
	}

	static T* InstancePtr( void )
	{
		return ( m_pInstance );
	}
};

template <typename T> T* BtSingleton <T>::m_pInstance = BtNull;
