////////////////////////////////////////////////////////////////////////////////
// HlDebug.cpp

// Includes
#include <stdio.h>
#include "HlDebug.h"
#include "HlFont.h"
#include "HlDraw.h"
#include "BtString.h"
#include "BtQueue.h"
#include "RsRenderTarget.h"
#include "RsMaterial.h"
#include "BaArchive.h"
#include "UiKeyboard.h"
#include "RsUtil.h"
#include "BtTime.h"
#include "HlKeyboard.h"
#include "ApConfig.h"
#include "FsFile.h"
#include "TinyXML2.h"
#include "HlUserData.h"
#include "BaArchive.h"
#include "ShHMD.h"

BtArray<HlItem, 1024> HlDebug::m_items;
BtS32 DebugIndex = 0;
BtBool HlDebug::m_isLoaded = BtFalse;
BtChar HlDebug::m_console[1024];
BtFloat HlDebug::m_scale;
BtFloat HlDebug::m_height;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlDebug::Setup( BaArchive *pArchive )
{
	m_isLoaded = BtFalse;

	m_scale = 1.0f;
	m_height = 15.0f;

	if (ShHMD::IsHMD())
	{
		m_scale *= 3.0f;
		m_height *= 3.0f;
	}
	else
	{
		MtVector2 v2Dimension = RsUtil::GetDimension();

		if( v2Dimension.x <= 800.0f )
		{
			m_scale  *= 1.5f;
			m_height *= 1.5f;
		}
		else
		{
			m_scale  *= 3.0f;
			m_height *= 3.0f;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetIndex

BtU32 HlDebug::GetIndex()
{
	return DebugIndex;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void HlDebug::Reset()
{
	m_items.Empty();

	HlItem item;
	BtStrCopy(item.m_name, 256, "Save" );
	item.m_type = HlDebugSave;
	item.m_pParent = BtNull;
	item.m_isReadOnly = BtFalse;
	m_items.Add(item);

	BtStrCopy(m_console, "");

	AddFPS(BtNull);
}

////////////////////////////////////////////////////////////////////////////////
// AddFPS

HlItem* HlDebug::AddFPS( HlItem *pParent )
{
	HlItem item;
	BtStrCopy(item.m_name, 256, "FPS");
	item.m_type = HlDebugFPS;
	item.m_pParent = pParent;
	return AddItem( item );
}

////////////////////////////////////////////////////////////////////////////////
// AddItem

HlItem *HlDebug::AddItem(HlItem &item)
{
	m_items.Add(item);
	return &m_items.GetLast();
}

////////////////////////////////////////////////////////////////////////////////
// AddItem

HlItem *HlDebug::AddItem(HlItem &item, HlAnimation &animation )
{
	m_items.Add(item);
	return &m_items.GetLast();
}

////////////////////////////////////////////////////////////////////////////////
// AddNode

HlItem *HlDebug::AddNode( HlItem *pParent, const BtChar *name )
{
	HlItem item;
	BtStrCopy( item.m_name, 256, name );
	item.m_type = HlDebugNode;
	item.m_pParent = pParent;
	item.m_isReadOnly = BtFalse;
	return AddItem( item );
}

////////////////////////////////////////////////////////////////////////////////
// AddFloat

HlItem* HlDebug::AddFloat( HlItem *pParent, const BtChar *name, BtFloat *pValue, BtBool isReadOnly, HlUnits unitsSystem, BtFloat minFloat, BtFloat maxFloat )
{
	HlItem item;
	BtStrCopy( item.m_name, 256, name );
	item.m_pFloat = pValue;
	item.m_type = HlDebugFloat;
	item.m_minFloat = minFloat;
	item.m_maxFloat = maxFloat;
	item.m_pParent = pParent;
	item.m_isReadOnly = isReadOnly;
	item.m_unitsSystem = unitsSystem;
	return AddItem( item );
}

////////////////////////////////////////////////////////////////////////////////
// AddInteger

HlItem* HlDebug::AddEnum(HlItem *pParent, const BtChar *name, BtChar *pEnumString, BtU32 *value, BtBool isReadOnly, BtU32 range)
{
	HlItem item;
	BtStrCopy(item.m_name, name);
	item.m_pEnum = value;
	item.m_type = HlDebugEnum;
	item.m_minInt = 0;
	item.m_maxInt = range;
	item.m_pParent = pParent;
	item.m_isReadOnly = isReadOnly;
	item.m_pEnumString = pEnumString;
	return AddItem( item );
}

////////////////////////////////////////////////////////////////////////////////
// AddUInt

HlItem* HlDebug::AddUInt(HlItem *pParent, const BtChar *name, BtU32 *pValue, BtBool isReadOnly )
{
	return AddUInt( pParent, name, pValue, isReadOnly, 0, 1000, 1 );
}

////////////////////////////////////////////////////////////////////////////////
// AddUInt

HlItem* HlDebug::AddUInt(HlItem *pParent, const BtChar *name, BtU32 *pValue, BtBool isReadOnly, BtU32 minInt, BtU32 maxInt, BtU32 diffInt)
{
	HlItem item;
	BtStrCopy(item.m_name, 256, name);
	item.m_pInteger = (BtS32*)pValue;
	item.m_type = HlDebugInteger;
	item.m_minInt = minInt;
	item.m_maxInt = maxInt;
	item.m_diffInt = diffInt;
	item.m_pParent = pParent;
	item.m_isReadOnly = isReadOnly;
	return AddItem(item);
}

////////////////////////////////////////////////////////////////////////////////
// AddInteger

HlItem* HlDebug::AddInteger( HlItem *pParent, const BtChar *name, BtS32 *pValue, BtBool isReadOnly, BtS32 minInt, BtS32 maxInt, BtS32 diffInt )
{
	HlItem item;
	BtStrCopy( item.m_name, 256, name );
	item.m_pInteger = pValue;
	item.m_type = HlDebugInteger;
	item.m_minInt = minInt;
	item.m_maxInt = maxInt;
	item.m_diffInt = diffInt;
	item.m_pParent = pParent;
	item.m_isReadOnly = isReadOnly;
	return AddItem( item );
}

////////////////////////////////////////////////////////////////////////////////
// AddBool

HlItem* HlDebug::AddBool(HlItem *pParent, const BtChar *name, BtBool *pValue, BtBool isReadOnly)
{
	HlItem item;
	BtStrCopy(item.m_name, 256, name);
	item.m_pBool = pValue;
	item.m_type = HlDebugBool;
	item.m_pParent = pParent;
	item.m_isReadOnly = isReadOnly;
	return AddItem( item );
}

////////////////////////////////////////////////////////////////////////////////
// AddVector

HlItem* HlDebug::AddVector(HlItem *pParent, const BtChar *name, MtVector3 *pValue, BtBool isReadOnly)
{
	HlItem item;
	BtStrCopy(item.m_name, 256, name);
	item.m_pVector = pValue;
	item.m_type = HlDebugVector;
	item.m_pParent = pParent;
	item.m_isReadOnly = isReadOnly;
	return AddItem( item );
}

////////////////////////////////////////////////////////////////////////////////
// AddColour

HlItem* HlDebug::AddColour(HlItem *pParent, const BtChar *name, RsColour *pValue, BtBool isReadOnly)
{
	HlItem item;
	BtStrCopy(item.m_name, 256, name);
	item.m_pColour = pValue;
	item.m_type = HlDebugColour;
	item.m_pParent = pParent;
	item.m_isReadOnly = isReadOnly;
	return AddItem( item );
}

////////////////////////////////////////////////////////////////////////////////
// GetItem

HlItem *HlDebug::GetItem( const BtChar *name )
{
	for(BtU32 i = 0; i < m_items.GetNumItems(); i++)
	{
		HlItem &item = m_items[i];

		if(BtStrCompare(item.m_name, name))
		{
			return &item;
		}
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// Load

void HlDebug::Load()
{
    // Decide which DebugAttributes are collapsed or expanded
    if( ApConfig::IsWin() )
    {
        tinyxml2::XMLNode *pNode;
        tinyxml2::XMLDocument doc;
        BtBool isParsed = HlUserData::GetXMLFromFile(doc, BtNull, "debug" );
        if(isParsed)
        {
            // Load in the aerofoil configuration
            pNode = doc.FirstChildElement("DebugAttributes");
            if(pNode)
            {
                tinyxml2::XMLNode *pItem = pNode->FirstChildElement();
                while( pItem )
                {
                    const char *pName = pItem->Value();
                
                    
                    tinyxml2::XMLNode *pElement = pItem->FirstChild();

                    const char *pNumAsString = pElement->Value();

                    for( BtU32 i=0; i<m_items.GetNumItems(); i++ )
                    {
                        HlItem &item = m_items[i];

                        if( BtStrCompare( item.m_name, pName ) )
                        {
                            item.m_isCollapsed = atoi(pNumAsString);
                        }
                    }
                    pItem = pItem->NextSibling();
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Save

void HlDebug::Save()
{
	BtChar filename[256];
	sprintf(filename, "%sdebug.txt", ApConfig::GetDocuments() );

	FsFile file;
	file.Open( filename, FsMode_WriteAscii );

	if( file.IsOpen() )
	{
		BtQueue<HlItem*, 32> itemStack;

		file.Write( "<DebugAttributes>\n" );
		BtU32 numItems = m_items.GetNumItems();
		for( BtU32 i=0; i<numItems; i++ )
		{
			HlItem &item = m_items[i];
			if( item.m_type == HlDebugNode )
			{
				file.Write( "<%s>%d</%s>\n", item.m_name, item.m_isCollapsed, item.m_name );
			}
		}		

		file.Write("</DebugAttributes>\n");
		file.Close();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void HlDebug::Update()
{
    if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_LSHIFT ) )
    {
        return;
    }
    
	static BtBool isLoaded = BtFalse;
	if(isLoaded == BtFalse)
	{
		Load();
		isLoaded = BtTrue;
	}

	if( ApConfig::IsDebug() == BtFalse )
	{
		return;
	}
	
	// Select options
	if(UiKeyboard::pInstance()->IsPressed(UiKeyCode_UP))
	{
		Decrement();
	}
	else if(UiKeyboard::pInstance()->IsPressed(UiKeyCode_DOWN))
	{
		Increment();
	}

	HlItem &item = m_items[DebugIndex];
	
	if( item.m_isReadOnly )
	{
		return;
	}

	BtFloat diff = ( item.m_maxFloat - item.m_minFloat ) / 20.0f;
		
	diff = diff * BtTime::GetTick();

    UiKeyCode speedUpKey = UiKeyCode_PLUS;
    
	if( UiKeyboard::pInstance()->IsHeld( speedUpKey ) )
	{
		diff *= 10.0f;
	}

	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_LEFT ) ||
		( UiKeyboard::pInstance()->IsHeld( UiKeyCode_LEFT ) && UiKeyboard::pInstance()->IsHeld( speedUpKey )))
	{
		if(item.m_type == HlDebugEnum)
		{
			BtU32 value = *item.m_pEnum;

			if( value > 0 )
			{
				value--;
				*item.m_pEnum = value;
			}
		}
		else if(item.m_type == HlDebugSave)
		{
			Save();
		}
		else if(item.m_type == HlDebugNode)
		{
			item.m_isCollapsed = BtTrue;
		}
		else if(item.m_type == HlDebugBool)
		{
			*item.m_pBool = BtFalse;
		}
		else if(item.m_type == HlDebugFloat)
		{
			*item.m_pFloat -= diff;

			MtClamp( *item.m_pFloat, item.m_minFloat, item.m_maxFloat );
		}
		else if( item.m_type == HlDebugInteger )
		{
			*item.m_pInteger -= item.m_diffInt;

			MtClamp( *item.m_pInteger, item.m_minInt, item.m_maxInt );
		}
	}
	if( UiKeyboard::pInstance()->IsPressed(UiKeyCode_RIGHT) ||
		( UiKeyboard::pInstance()->IsHeld(UiKeyCode_RIGHT) && UiKeyboard::pInstance()->IsHeld(speedUpKey)))
	{
		if( item.m_type == HlDebugEnum )
		{
			BtU32 value = *item.m_pEnum;
			value++;
			if( value < item.m_maxInt )
			{
				*item.m_pEnum = value;
			}
		}
		else if(item.m_type == HlDebugSave )
		{
			Save();
		}
		else if(item.m_type == HlDebugBool)
		{
			*item.m_pBool = BtTrue;
		}
		else if(item.m_type == HlDebugNode)
		{
			item.m_isCollapsed = BtFalse;
		}
		else if( item.m_type == HlDebugFloat )
		{
			*item.m_pFloat += diff;

			MtClamp( *item.m_pFloat, item.m_minFloat, item.m_maxFloat );
		}
		else if( item.m_type == HlDebugInteger )
		{
			*item.m_pInteger += item.m_diffInt;

			MtClamp( *item.m_pInteger, item.m_minInt, item.m_maxInt );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsVisible

BtBool HlDebug::IsHidden(HlItem &item)
{
	if(item.m_pParent)
	{
		if( item.m_pParent->m_isCollapsed )
		{
			return BtTrue;
		}
		else
		{
			return IsHidden( *item.m_pParent );
		}
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Increment

void HlDebug::Increment()
{
	if( DebugIndex < m_items.GetNumItems() )
	{
		for( BtU32 i=DebugIndex + 1; i<m_items.GetNumItems(); i++ )
		{
			if( !IsHidden(m_items[i]) )
			{
				DebugIndex = i;
				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNumParents

BtU32 HlDebug::GetNumParents(HlItem *pItem)
{
	BtU32 parents = 0;
	while(pItem->m_pParent)
	{
		++parents;
		pItem = pItem->m_pParent;
	}
	return parents;
}

////////////////////////////////////////////////////////////////////////////////
// Decrement

void HlDebug::Decrement()
{
	if( DebugIndex > 0 )
	{
		for(BtS32 i = DebugIndex - 1; i>=0; i--)
		{
			if( !IsHidden(m_items[i]) )
			{
				DebugIndex = i;
				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetConsoleText

void HlDebug::SetConsoleText(const BtChar *console)
{
	BtStrCopy( m_console, console );
}

////////////////////////////////////////////////////////////////////////////////
// AddConsoleText

void HlDebug::AddConsoleText(const BtChar *console)
{
	BtStrCat( m_console, sizeof(m_console), console );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlDebug::Render()
{
	if( !ApConfig::IsDebug() )
	{
		return;
	}

	BtU32 numItems = m_items.GetNumItems();
	
	MtVector2 v2Position( 0, 0 );
    MtVector2 v2Dimension( 0, 0 );
	m_height = 20.0f;
	m_scale = 1.0f;

	for( BtU32 i=0; i<numItems; i++ )
	{
		HlItem &item = m_items[i];

		if( !IsHidden(item) )
		{
			RsColour textColour = RsColour::WhiteColour();
			if( item.m_isReadOnly )
			{
				textColour = RsColour( 0.7f, 0.7f, 0.7f, 1.0f );
			}

			BtChar text[2048];
			sprintf( text, "" );

			for(BtU32 j = 0; j < GetNumParents(&item) * 4; j++)
			{
				sprintf( text + strlen(text), " ");
			}

			if(item.m_type == HlDebugEnum )
			{
				BtU32 current = *item.m_pEnum;

				sprintf( text + strlen(text), "%s ", item.m_name );

				BtU32 index = 0;

				BtChar *pStr = item.m_pEnumString;
				while( pStr )
				{
					BtChar temp[256];
					BtStrCopy( temp, 256, pStr, BtStrLength( pStr ) );
					BtChar *eol = BtStrStr(temp, ";");
					if( eol )
					{
						*eol = 0;
						pStr = BtStrStr(pStr, ";") + 1;
					}
					else
					{
						pStr = 0;
					}
	
					if(current == index)
					{
						sprintf(text + strlen(text), "%s", temp);
					}
					++index;
				}
			}
			else if(item.m_type == HlDebugSave )
			{
				sprintf(text + strlen(text), "Save");
			}
			else if( item.m_type == HlDebugNode )
			{
				sprintf( text + strlen(text), "%s", item.m_name);

				if( item.m_isCollapsed )
				{
					sprintf( text + strlen(text), "+" );
				}
				else
				{
					sprintf(text + strlen(text), "-" );
				}
			}
			else if( item.m_type == HlDebugFloat )
			{
				BtFloat value = *item.m_pFloat;
				
				// Do the conversion
				if( item.m_unitsSystem == HLUnits_Knots )
				{
					value = MtMetersPerSecondToKnots( value );
					sprintf(text + strlen(text), "%s %.2fKnts", item.m_name, value);
				}
				else
				{
					sprintf( text + strlen(text), "%s %.2f", item.m_name, value );
				}
			}
			else if( item.m_type == HlDebugInteger )
			{
				sprintf( text + strlen(text), "%s %d", item.m_name, *item.m_pInteger );
			}
			else if(item.m_type == HlDebugBool )
			{
				if( *item.m_pBool )
				{
					sprintf(text + strlen(text), "%s True", item.m_name );
				}
				else
				{
					sprintf(text + strlen(text), "%s False", item.m_name );
				}
			}
			else if( item.m_type == HlDebugVector )
			{
				sprintf(text + strlen(text), "%s %.2f %.2f %.2f", item.m_name, item.m_pVector->x, item.m_pVector->y, item.m_pVector->z );
			}
			else if( item.m_type == HlDebugColour )
			{
				sprintf(text + strlen(text), "%s %.2f %.2f %.2f", item.m_name, item.m_pColour->Red(), item.m_pColour->Green(), item.m_pColour->Blue() );
			}
			else if( item.m_type == HlDebugFPS )
			{
				sprintf( text + strlen(text), "FPS %.2f", RsUtil::GetFPS() );
			}

			if( DebugIndex == i )
			{
				sprintf( text + strlen(text), "*" );
			}
			
			MtVector2 v2FontDimension = HlFont::GetDimension( text ) * m_scale;

			v2Dimension.x = MtMax( v2Dimension.x, v2Position.x + v2FontDimension.x + 15.0f );
			v2Dimension.y = MtMax( v2Dimension.y, v2Position.y + v2FontDimension.y + 15.0f );

			HlFont::Render(v2Position, m_scale, text, textColour, MaxSortOrders - 1);
			v2Position.y += m_height;
		}
	}
	
	// Display the list of archives
	{
		v2Position.y += m_height;
		BtChar text[2048];
		sprintf(text, "Archives:" );
		HlFont::Render(v2Position, m_scale, text, RsColour(0.7f, 0.7f, 0.7f, 1.0f), MaxSortOrders - 1);
		v2Position.y += m_height;

		BtLinkedList<BaArchiveNode> archives = BaArchive::GetArchives();
		BaArchiveNode *pCurrent = archives.pHead();
		while (pCurrent)
		{
			BtChar text[2048];
			sprintf(text, "   %s", pCurrent->m_pArchive->GetFilename());
			MtVector2 v2FontDimension = HlFont::GetDimension(text) * m_scale;
			v2Dimension.x = MtMax(v2Dimension.x, v2Position.x + v2FontDimension.x + 30.0f);
			v2Dimension.y = MtMax(v2Dimension.y, v2Position.y + v2FontDimension.y + 30.0f);
			HlFont::Render(v2Position, m_scale, text, RsColour(0.7f, 0.7f, 0.7f, 1.0f), MaxSortOrders - 1);
			v2Position.y += m_height;
			pCurrent = pCurrent->pNext();
		}

		// Display the console
		if (BtStrLength(m_console))
		{
			v2Position.y += m_height;
			MtVector2 v2FontDimension;
			v2FontDimension = HlFont::GetDimension("Console:") * m_scale;
			HlFont::Render(v2Position, "Console:", RsColour(0.7f, 0.7f, 0.7f, 1.0f), MaxSortOrders - 1);
			v2Dimension.x = MtMax(v2Dimension.x, v2Position.x + v2FontDimension.x + 30.0f);
			v2Dimension.y = MtMax(v2Dimension.y, v2Position.y + v2FontDimension.y + 30.0f);
			v2Position.y += m_height;
			v2FontDimension = HlFont::GetDimension(m_console) * m_scale;
			HlFont::Render(v2Position, m_console, RsColour(0.7f, 0.7f, 0.7f, 1.0f), MaxSortOrders - 1);
			v2Dimension.x = MtMax(v2Dimension.x, v2Position.x + v2FontDimension.x + 30.0f);
			v2Dimension.y = MtMax(v2Dimension.y, v2Position.y + v2FontDimension.y + 30.0f);
		}
	}
	HlDraw::RenderQuad( MtVector2( 0, 0 ), v2Dimension, RsColour( 0.1f, 0.1f, 0.1f, 1.0f ), MaxSortOrders-1 );
}

////////////////////////////////////////////////////////////////////////////////
// RenderFPS

void HlDebug::RenderFPS()
{
	BtChar text[32];
	sprintf(text, "%.2f", RsUtil::GetFPS());
	MtVector2 v2Dimension = HlFont::GetDimension(text);
	MtVector2 v2Position;
	v2Position = RsUtil::GetDimension() - v2Dimension;
	v2Position.y = 0;
	HlFont::Render(v2Position, text, RsColour::BlackColour(), MaxSortOrders - 1);
}
