/* WheelItemBase - An item on the wheel. */

#ifndef WHEEL_ITEM_BASE_H
#define WHEEL_ITEM_BASE_H

#include "ActorFrame.h"
#include "BitmapText.h"
#include "ThemeMetric.h"
#include "GameConstantsAndTypes.h"
#include "AutoActor.h"

struct WheelItemBaseData;

enum WheelItemDataType 
{
	TYPE_GENERIC,
	TYPE_SECTION, 
	TYPE_SONG, 
	TYPE_ROULETTE, 
	TYPE_RANDOM, 
	TYPE_PORTAL, 
	TYPE_COURSE, 
	TYPE_SORT 
};

struct WheelItemBaseData
{
	WheelItemBaseData() {}
	WheelItemBaseData( WheelItemDataType type, RString sText, RageColor color );
	virtual ~WheelItemBaseData() {}
	WheelItemDataType	m_Type;
	RString			m_sText;
	RageColor		m_color;	// either text color or section background color
};

class WheelItemBase : public ActorFrame
{
public:
	WheelItemBase( RString sType );
	WheelItemBase( const WheelItemBase &cpy );
	virtual void DrawPrimitives();
	virtual WheelItemBase *Copy() const { return new WheelItemBase(*this); }
	
	void Load( RString sType );
	void DrawGrayBar( Actor& bar );
	void SetExpanded( bool bExpanded ) { m_bExpanded = bExpanded; }

	virtual void LoadFromWheelItemData( const WheelItemBaseData* pWID, int iIndex, bool bHasFocus, int iDrawIndex );

	RageColor	m_colorLocked;

protected:
	void SetGrayBar( Actor *pBar ) { m_pGrayBar = pBar; }

	const WheelItemBaseData* m_pData;
	bool m_bExpanded;	// if TYPE_SECTION, whether this section is expanded

	Actor*		m_pGrayBar;
};

#endif

/*
 * (c) 2001-2006 Chris Danford, Chris Gomez, Glenn Maynard, Josh Allen
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
