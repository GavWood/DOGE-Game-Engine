////////////////////////////////////////////////////////////////////////////////
// SbPeers.h

// Include guard
#pragma once
#include "BtArray.h"
#include "MtMatrix4.h"
#include <vector>
#include "BtMemory.h"

class RsSprite;
class BaArchive;

enum SbGamePacketType
{
	SbGamePacketType_Message,
	SbGamePacketType_PeerShutdown,
	SbGamePacketType_Ping,
	SbGamePacketType_ShareState,
};

struct SbGamePacket
{
public:
	
	struct SbGamePacketHeader
	{
		SbGamePacketType						m_type;
		BtU32									m_hashcode;
	};

	SbGamePacket(SbGamePacketType type)
	{
		m_header.m_type = type;
		m_header.m_hashcode = 0;

		m_current = 0;
		m_length = 0;
	}
	
	// Public function
	void									SendReliable();
	void									SendUnsequenced();
	void									SendFragment();

	// Accessors
	SbGamePacketType						GetType();
	BtU8								   *GetData() { return (BtU8*)&m_data[0]; }
	BtU32									GetLength();
	void									ValidateHashcode();

	// To quickly send arbitrary types
	template< typename T > inline void operator >> (T& Data)
	{
		BtU32 sizeOfTemplate = sizeof(T);
		BtMemory::Copy( m_data + m_current, &Data, sizeOfTemplate);
		m_current += sizeOfTemplate;
		m_length += sizeOfTemplate;
	}

	template< typename T > inline void operator << (T& Data)
	{
		BtU32 sizeOfTemplate = sizeof(T);
		BtMemory::Copy(&Data, m_data + m_current, sizeOfTemplate);
		m_current += sizeOfTemplate;
		m_length += sizeOfTemplate;
	}

private:

	SbGamePacketHeader						m_header;
	BtChar									m_data[4096];
	BtU32									m_length = 0;
	BtU32									m_current = 0;
};

// Class definition
class SbPeers
{
public:

	// Public functions
	void									Setup();
	void									Update();
	void									Render();

	void									UpdateDebug();

	// Accessors
	BtChar					 			   *GetStatus() { return m_status; }

private:

	// Private functions

	// Private members
	BtU32									m_numPlayers;
	BtChar									m_status[256];
	BtBool									m_isOldest;
};
