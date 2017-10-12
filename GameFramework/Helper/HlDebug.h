////////////////////////////////////////////////////////////////////////////////
// HlDebug.h

#pragma once
#include "BtArray.h"
#include "BtTypes.h"
#include "RsColour.h"
#include "MtVector3.h"
#include "HlAnimation.h"

const BtU32 HlDebugNode	   = 0;
const BtU32 HlDebugInteger = 1;
const BtU32 HlDebugFloat   = 2;
const BtU32 HlDebugFPS     = 3;
const BtU32 HlDebugVector  = 4;
const BtU32 HlDebugBool    = 5;
const BtU32 HlDebugSave	   = 6;
const BtU32 HlDebugEnum    = 7;
const BtU32 HlDebugColour  = 8;

class RsMaterial;
class BaArchive;

enum HlUnits
{
	HLUnits_StandardIndex,
	HLUnits_Knots,
	HLUnits_Max,
};

struct HlItem
{
	HlItem()
	{
		m_isReadOnly = BtTrue;
		m_isCollapsed = BtFalse;
		m_unitsSystem = HLUnits_StandardIndex;
	}

	BtU32							m_type;
	HlUnits							m_unitsSystem;
	BtChar							m_name[256];
	BtChar						   *m_pEnumString;
	BtFloat						   *m_pFloat;
	MtVector3					   *m_pVector;
	RsColour					   *m_pColour;
	BtBool						   *m_pBool;
	BtS32						   *m_pInteger;
	BtU32						   *m_pEnum;
	BtU32							m_minInt;
	BtU32							m_maxInt;
	BtFloat							m_minFloat;
	BtFloat							m_maxFloat;
	BtU32							m_diffInt;
	HlItem						   *m_pParent;
	BtBool							m_isReadOnly;
	BtBool							m_isCollapsed;
};

// Class definition
class HlDebug
{
public:

	// Public functions
	static void						Setup( BaArchive *pArchive );
	static void						Reset();

	static HlItem*					AddFPS( HlItem *pParent );
	static HlItem*					AddNode( HlItem *pParent, const BtChar *name );
	static HlItem*					AddFloat( HlItem *pParent, const BtChar *name, BtFloat *value, BtBool isReadOnly, HlUnits unitsSystem = HLUnits_StandardIndex, BtFloat minFloat = 0.0f, BtFloat maxFloat = 1.0f );
	static HlItem*					AddEnum( HlItem *pParent, const BtChar *name, BtChar *enumString, BtU32 *value, BtBool isReadOnly, BtU32 range);
	static HlItem*					AddInteger(HlItem *pParent, const BtChar *name, BtS32 *value, BtBool isReadOnly, BtS32 minInt, BtS32 maxInt, BtS32 diffInt);
	
	static HlItem*					AddUInt(HlItem *pParent, const BtChar *name, BtU32 *value, BtBool isReadOnly, BtU32 minInt, BtU32 maxInt, BtU32 diffInt);
	static HlItem*					AddUInt(HlItem *pParent, const BtChar *name, BtU32 *value, BtBool isReadOnly);
	
	static HlItem*					AddVector(HlItem *pParent, const BtChar *name, MtVector3 *value, BtBool isReadOnly);
	static HlItem*					AddColour( HlItem *pParent, const BtChar *name, RsColour *value, BtBool isReadOnly);
	static HlItem*					AddBool( HlItem *pParent, const BtChar *name, BtBool *value, BtBool isReadOnly);
	static HlItem*					AddItem( HlItem &item );

	static HlItem*					AddItem( HlItem &item, HlAnimation &animation );

	static void						SetConsoleText(const BtChar *console);
	static void						AddConsoleText(const BtChar *console);

	static void						Update();
	static void						Render();
	static BtU32					GetIndex();
	static HlItem				   *GetItem( const BtChar *name );

	static void						RenderFPS();

private:

	static void						Load();
	static void						Save();
	static BtU32					GetNumParents( HlItem *pItem );
	static BtBool					IsHidden( HlItem &item );
	static void						Increment();
	static void						Decrement();

	static BtArray<HlItem, 1024>	m_items;
	static BtChar					m_console[1024];
	static BtBool					m_isLoaded;
	static BtFloat					m_scale;
	static BtFloat					m_height;
};
