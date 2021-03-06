/* Steps - Holds note information for a Song.  A Song may have one or more Notes. */

#ifndef STEPS_H
#define STEPS_H

#include "GameConstantsAndTypes.h"
#include "PlayerNumber.h"
#include "Grade.h"
#include "RadarValues.h"
#include "Difficulty.h"
#include "RageUtil_AutoPtr.h"
#include "RageUtil_CachedObject.h"
class Profile;
class NoteData;
struct lua_State;

const int MAX_EDIT_STEPS_DESCRIPTION_LENGTH = 12;

class Steps
{
public:
	Steps();
	~Steps();

	// initializers
	void AutogenFrom( const Steps *parent, StepsType ntTo );
	void CopyFrom( Steps* pSource, StepsType ntTo, float fMusicLengthSeconds );
	void CreateBlank( StepsType ntTo );

	void Compress() const;
	void Decompress() const;
	bool IsAutogen() const				{ return parent != NULL; } // Was created by autogen?

	// Use a special value of difficulty
	bool IsAnEdit() const				{ return m_Difficulty == Difficulty_Edit; }
	bool IsAPlayerEdit() const			{ return IsAnEdit() && GetLoadedFromProfileSlot() < ProfileSlot_Machine; }
	bool WasLoadedFromProfile() const		{ return m_LoadedFromProfile != ProfileSlot_Invalid; }
	ProfileSlot GetLoadedFromProfileSlot() const	{ return m_LoadedFromProfile; }
	RString GetDescription() const			{ return Real()->m_sDescription; }
	Difficulty GetDifficulty() const		{ return Real()->m_Difficulty; }
	int GetMeter() const				{ return Real()->m_iMeter; }
	const RadarValues& GetRadarValues( PlayerNumber pn ) const { return Real()->m_CachedRadarValues[pn]; }

	void SetFilename( RString fn )			{ m_sFilename = fn; }
	RString GetFilename() const			{ return m_sFilename; }
	void SetSavedToDisk( bool b )			{ DeAutogen(); m_bSavedToDisk = b; }
	bool GetSavedToDisk() const			{ return Real()->m_bSavedToDisk; }
	void SetDifficulty( Difficulty dc )		{ SetDifficultyAndDescription( dc, GetDescription() ); }
	void SetDescription( RString sDescription ) 	{ SetDifficultyAndDescription( this->GetDifficulty(), sDescription ); }
	void SetDifficultyAndDescription( Difficulty dc, RString sDescription );
	static bool MakeValidEditDescription( RString &sPreferredDescription );	// return true if was modified

	void SetLoadedFromProfile( ProfileSlot slot )	{ m_LoadedFromProfile = slot; }
	void SetMeter( int meter );
	void SetCachedRadarValues( const RadarValues v[NUM_PLAYERS] );
	float PredictMeter() const;
	
	unsigned GetHash() const;
	void GetNoteData( NoteData& noteDataOut ) const;
	void SetNoteData( const NoteData& noteDataNew );
	void SetSMNoteData( const RString &notes_comp );
	void GetSMNoteData( RString &notes_comp_out ) const;

	void TidyUpData();
	void CalculateRadarValues( float fMusicLengthSeconds );
	
	// Lua
	void PushSelf( lua_State *L );

	StepsType			m_StepsType;

	CachedObject<Steps> m_CachedObject;

private:
	inline const Steps *Real() const		{ return parent ? parent : this; }
	void DeAutogen( bool bCopyNoteData = true ); /* If this Steps is autogenerated, make it a real Steps. */

	/* If this Steps is autogenerated, this will point to the autogen
	 * source.  If this is true, m_sNoteDataCompressed will always be empty. */
	const Steps			*parent;

	/* We can have one or both of these; if we have both, they're always identical.
	 * Call Compress() to force us to only have m_sNoteDataCompressed; otherwise, creation of 
	 * these is transparent. */
	mutable HiddenPtr<NoteData>	m_pNoteData;
	mutable bool			m_bNoteDataIsFilled;
	mutable RString			m_sNoteDataCompressed;

	RString				m_sFilename;
	bool				m_bSavedToDisk;		// true if this was loaded from disk or has been saved to disk.
	ProfileSlot			m_LoadedFromProfile;	// ProfileSlot_Invalid if wasn't loaded from a profile

	/* These values are pulled from the autogen source first, if there is one. */
	mutable unsigned		m_iHash;		// only used if m_Difficulty == Difficulty_Edit
	RString				m_sDescription;		// Step author, edit name, or something meaningful
	Difficulty			m_Difficulty;		// difficulty classification
	int				m_iMeter;		// difficulty rating from MIN_METER to MAX_METER
	RadarValues			m_CachedRadarValues[NUM_PLAYERS];
};

#endif

/*
 * (c) 2001-2004 Chris Danford, Glenn Maynard
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
