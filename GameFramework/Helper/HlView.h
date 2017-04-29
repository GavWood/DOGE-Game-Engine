////////////////////////////////////////////////////////////////////////////////
// HlView.h

// Include guard
#pragma once

// Includes
#include "BaArchive.h"
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "ShJoystick.h"
#include "RsRenderTarget.h"
#include "BtString.h"
#include <vector>

class RsTexture;
class RsMaterial;

enum HlMenuJoystick
{
	HlMenuJoystick_Yes,
	HlMenuJoystick_No,
	HlMenuJoystick_Any,
	HlMenuJoystick_MAX,
};

const BtU32 HlMenuItemAlignment_HorizontalCentre = 1 << 0;
const BtU32 HlMenuItemAlignment_VerticalCentre = 1 << 1;
const BtU32 HlMenuItemAlignment_Left = 1 << 2;
const BtU32 HlMenuItemAlignment_Right = 1 << 3;
const BtU32 HlMenuItemAlignment_Top = 1 << 4;
const BtU32 HlMenuItemAlignment_Bottom = 1 << 5;

struct HlMenuItems
{
	HlMenuItems()
	{
		m_selectedSpriteIndex = -1;
		m_pSprite = BtNull;
		m_alignment = 0;
		m_angle = 0;						// default orientation

		m_darkenWhenNotCurrent = m_darkenWhenSelected = 1.0f;

		BtStrCopy(m_id, 64, "unset_id");

		// Set the default sort order
		m_sortOrder = MaxSortOrders - 1;

		// Set the default fade
		m_fade = 1.0f;

		// We model menu items with a specific resolution in mind. Set this here.
		// Eventually we can pull this from the XML

		m_v2OriginalScreenSize = MtVector2(800.0f, 600.0f);

		// The default for each menu item is that it doesn't matter if it's a joystick or not
		m_joystick = HlMenuJoystick_Any;

		// Shown on which platforms
		m_isAppleTV = BtTrue;
		m_isPC = BtTrue;

		// Which joystick button presses the ID
		m_joystickButtonID = JoystickButton_MAX;

		// Navigation
		m_down = -1;
		m_up = -1;
		m_left = -1;
		m_right = -1;
	}

	// ID and tags
	BtChar									m_id[64];

	// Sprite rendering
	BtS32									m_spriteIndex;						// Index of sprite
	BtS32									m_selectedSpriteIndex;				// Index of sprite when selected
	RsSprite							   *m_pSprite;

	// Rendering properties
	MtVector2								m_v2Position;
	MtVector2								m_v2Dimension;
	BtBool									m_isVisible;
	BtFloat									m_fade;
	BtFloat									m_darkenWhenSelected;
	BtFloat									m_darkenWhenNotCurrent;
	BtU32									m_sortOrder;

	// Touch interaction
	BtBool									m_isPressed;
	BtBool									m_isReleased;
	BtBool									m_isHeld;
	BtBool									m_isSelectable;

	// What is the positioning of the item
	MtVector2								m_v2OriginalScreenSize;
	MtVector2								m_v2OriginalPosition;
	MtVector2								m_v2OriginalDimension;
	BtU32									m_alignment;
	BtFloat									m_angle;

	ShJoystickButton						m_joystickButtonID;

	// Do we need all the items
	BtBool									m_isAppleTV;
	BtBool									m_isPC;
	HlMenuJoystick							m_joystick;

	// Navigation
	BtS32									m_down;
	BtS32									m_up;
	BtS32									m_left;
	BtS32									m_right;
};

// Class definition
class HlView
{
public:
	
	// Public functions		
	void									Setup( const BtChar *archiveName,
												   const BtChar *screenName,
												   BtBool isBackground );
	void									Load();

	virtual void							Update();
	virtual void							Render();

	virtual void							OnExit();
	virtual void							OnEntry();

	void									RenderOVR();
	void									Unload();

	// Accessors
	void									SetRenderDimension( MtVector2 v2Dimension );
	
	void									SetVisible( BtU32 index, BtBool isVisible );
	void									SetVisible( const BtChar *name, BtBool isVisible );
	BtBool									GetVisible( BtU32 index );

	HlMenuItems							   *GetCurrentItemSelected();
	HlMenuItems							   *GetCurrentItem();

	HlMenuItems						  	   *FindItem( const BtChar *name );
	HlMenuItems						  	   *FindItem( BtU32 id );
	
	BtU32									GetNumItems();
	
	void									SetBackground( BtBool isBackground );
	
	BtBool									IsEqual( const BtChar *string );

	static BaArchive					   &GetArchive() { return m_archive; }

protected:

	HlMenuItems							   *m_pCurrentMenuItemSelected;				// Is selection
	HlMenuItems							   *m_pCurrentMenuItem;						// Going to be new selection

	BtBool									m_isBackground;

	void									UpdateMenu();
	void									RenderMenu();
	void									LoadMenu();
	
	BtChar									m_archiveName[32];
	BtChar									m_screenName[32];
	
	// Background
	RsMaterial							   *m_pBackgroundMaterial;					// For the background material
	
	MtVector2								m_v2RenderDimension;

	// Menu
	static BtArray<HlMenuItems, 64>			m_items;

	// This is so that we can have one archive open for views at any one time
	static HlView						   *m_pLast;
	static BaArchive						m_archive;
	static BtChar							m_lastArchiveName[32];
    BtBool					                m_isLoaded;
	BtU32									m_backgroundSortOrder;
};

inline HlMenuItems *HlView::GetCurrentItemSelected()
{
	return m_pCurrentMenuItemSelected;
}

inline HlMenuItems *HlView::GetCurrentItem()
{
	return m_pCurrentMenuItem;
}