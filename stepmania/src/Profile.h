/* Profile - Player data that persists between sessions.  Can be stored on a local disk or on a memory card. */

#ifndef Profile_H
#define Profile_H

#include "GameConstantsAndTypes.h"
#include "Grade.h"
#include <map>
#include <set>
#include <deque>
#include "HighScore.h"
#include "DateTime.h"
#include "SongUtil.h"	// for SongID
#include "StepsUtil.h"	// for StepsID
#include "CourseUtil.h"	// for CourseID
#include "TrailUtil.h"	// for TrailID
#include "StyleUtil.h"	// for StyleID

class XNode;
struct lua_State;
class Character;

//
// Current file versions
//
extern const RString STATS_XML;

extern const RString EDITABLE_INI;

// Editable data is an INI because the default INI file association on Windows 
// systems will open the ini file in an editor.  The default association for 
// XML will open in IE.  Users have a much better chance of discovering how to 
// edit this data if they don't have to fight against the file associations.
extern const RString DONT_SHARE_SIG;

// The "don't share" file is something that the user should always keep private.
// They can safely share STATS_XML with STATS_XML's signature so that others
// can authenticate the STATS_XML data.  However, others can't copy that data
// to their own profile for use in the game unless they also have the "don't 
// share" file.  DontShare contains a piece of information that we can 
// construct using STATS_XML but the user can't construct using STATS_XML.
// The file contains a signature of the STATS_XML's signature.
extern const RString PUBLIC_KEY_FILE;
extern const RString SCREENSHOTS_SUBDIR;
extern const RString EDIT_STEPS_SUBDIR;
extern const RString EDIT_COURSES_SUBDIR;
extern const RString LASTGOOD_SUBDIR;

const unsigned int PROFILE_MAX_DISPLAY_NAME_LENGTH	= 12;


class Style;

class Song;
class Steps;
class Course;
class Game;

class Profile
{
public:
	Profile()
	{
		InitAll();
	}

	//
	// smart accessors
	//
	RString GetDisplayNameOrHighScoreName() const;
	Character *GetCharacter() const;
	RString GetDisplayTotalCaloriesBurned() const;		// remove me and use Lua instead
	RString GetDisplayTotalCaloriesBurnedToday() const;	// remove me and use Lua instead
	int GetCalculatedWeightPounds() const;	// returns a default value if m_iWeightPounds isn't set
	float GetCaloriesBurnedToday() const;
	int GetTotalNumSongsPassed() const;
	int GetTotalStepsWithTopGrade( StepsType st, Difficulty d, Grade g ) const;
	int GetTotalTrailsWithTopGrade( StepsType st, CourseDifficulty d, Grade g ) const;
	float GetSongsPossible( StepsType st, Difficulty dc ) const;
	float GetCoursesPossible( StepsType st, CourseDifficulty cd ) const;
	float GetSongsActual( StepsType st, Difficulty dc ) const;
	float GetCoursesActual( StepsType st, CourseDifficulty cd ) const;
	float GetSongsPercentComplete( StepsType st, Difficulty dc ) const;
	float GetCoursesPercentComplete( StepsType st, CourseDifficulty cd ) const;
	float GetSongsAndCoursesPercentCompleteAllDifficulties( StepsType st ) const;
	bool GetDefaultModifiers( const Game* pGameType, RString &sModifiersOut ) const;
	void SetDefaultModifiers( const Game* pGameType, const RString &sModifiers );
	bool IsCodeUnlocked( RString sUnlockEntryID ) const;
	Song *GetMostPopularSong() const;
	Course *GetMostPopularCourse() const;
	
	void AddStepTotals( int iNumTapsAndHolds, int iNumJumps, int iNumHolds, int iNumRolls, int iNumMines, int iNumHands, float fCaloriesBurned );

	bool IsMachine() const;

	//
	// Editable data
	//
	RString m_sDisplayName;
	RString m_sCharacterID;
	RString m_sLastUsedHighScoreName;	// this doesn't really belong in "editable", but we need it in the smaller editable file so that it can be ready quickly.
	int m_iWeightPounds;	// 0 == not set

	//
	// General data
	//
	static RString MakeGuid();

	RString m_sGuid;
	map<RString,RString> m_sDefaultModifiers;
	SortOrder m_SortOrder;
	Difficulty m_LastDifficulty;
	CourseDifficulty m_LastCourseDifficulty;
	StepsType m_LastStepsType;
	SongID m_lastSong;
	CourseID m_lastCourse;
	int m_iTotalSessions;
	int m_iTotalSessionSeconds;
	int m_iTotalGameplaySeconds;
	float m_fTotalCaloriesBurned;
	GoalType m_GoalType;
	int m_iGoalCalories;
	int m_iGoalSeconds;
	int m_iTotalDancePoints;
	int m_iNumExtraStagesPassed;
	int m_iNumExtraStagesFailed;
	int m_iNumToasties;
	int m_iTotalTapsAndHolds;
	int m_iTotalJumps;
	int m_iTotalHolds;
	int m_iTotalRolls;
	int m_iTotalMines;
	int m_iTotalHands;
	set<RString> m_UnlockedEntryIDs;
	mutable RString m_sLastPlayedMachineGuid;	// mutable because we overwrite this on save, and I don't want to remove const from the whole save chain. -Chris
	mutable DateTime m_LastPlayedDate;
	/* These stats count twice in the machine profile if two players are playing;
	 * that's the only approach that makes sense for ByDifficulty and ByMeter. */
	int m_iNumSongsPlayedByPlayMode[NUM_PlayMode];
	map<StyleID,int> m_iNumSongsPlayedByStyle;
	int m_iNumSongsPlayedByDifficulty[NUM_Difficulty];
	int m_iNumSongsPlayedByMeter[MAX_METER+1];
	/* This stat counts once per song, even if two players are active. */
	int m_iNumTotalSongsPlayed;
	int m_iNumStagesPassedByPlayMode[NUM_PlayMode];
	int m_iNumStagesPassedByGrade[NUM_Grade];

	//
	// Song high scores
	//
	struct HighScoresForASteps
	{
		HighScoreList hsl;
	};
	struct HighScoresForASong
	{
		std::map<StepsID,HighScoresForASteps>	m_StepsHighScores;
		int GetNumTimesPlayed() const;
	};
	std::map<SongID,HighScoresForASong>	m_SongHighScores;

	void AddStepsHighScore( const Song* pSong, const Steps* pSteps, HighScore hs, int &iIndexOut );
	const HighScoreList& GetStepsHighScoreList( const Song* pSong, const Steps* pSteps ) const;
	HighScoreList& GetStepsHighScoreList( const Song* pSong, const Steps* pSteps );
	int GetStepsNumTimesPlayed( const Song* pSong, const Steps* pSteps ) const;
	void IncrementStepsPlayCount( const Song* pSong, const Steps* pSteps );
	void GetGrades( const Song* pSong, StepsType st, int iCounts[NUM_Grade] ) const;
	int GetSongNumTimesPlayed( const Song* pSong ) const;
	int GetSongNumTimesPlayed( const SongID& songID ) const;
	DateTime GetSongLastPlayedDateTime( const Song* pSong ) const;
	bool HasPassedSteps( const Song* pSong, const Steps* pSteps ) const;
	bool HasPassedAnyStepsInSong( const Song* pSong ) const;

	//
	// Course high scores
	//
	// struct was a typedef'd array of HighScores, but VC6 freaks out 
	// in processing the templates for map::operator[].
	struct HighScoresForATrail
	{
		HighScoreList hsl;
	};
	struct HighScoresForACourse	
	{
		std::map<TrailID,HighScoresForATrail>	m_TrailHighScores;
		int GetNumTimesPlayed() const;
	};
	std::map<CourseID,HighScoresForACourse>	m_CourseHighScores;

	void AddCourseHighScore( const Course* pCourse, const Trail* pTrail, HighScore hs, int &iIndexOut );
	HighScoreList& GetCourseHighScoreList( const Course* pCourse, const Trail* pTrail );
	const HighScoreList& GetCourseHighScoreList( const Course* pCourse, const Trail* pTrail ) const;
	int GetCourseNumTimesPlayed( const Course* pCourse ) const;
	int GetCourseNumTimesPlayed( const CourseID& courseID ) const;
	DateTime GetCourseLastPlayedDateTime( const Course* pCourse ) const;
	void IncrementCoursePlayCount( const Course* pCourse, const Trail* pTrail );


	//
	// Category high scores
	//
	HighScoreList m_CategoryHighScores[NUM_StepsType][NUM_RankingCategory];

	void AddCategoryHighScore( StepsType st, RankingCategory rc, HighScore hs, int &iIndexOut );
	HighScoreList& GetCategoryHighScoreList( StepsType st, RankingCategory rc );
	const HighScoreList& GetCategoryHighScoreList( StepsType st, RankingCategory rc ) const;
	int GetCategoryNumTimesPlayed( StepsType st ) const;
	void IncrementCategoryPlayCount( StepsType st, RankingCategory rc );


	//
	// Screenshot Data
	//
	vector<Screenshot> m_vScreenshots;
	void AddScreenshot( const Screenshot &screenshot );
	int GetNextScreenshotIndex() { return m_vScreenshots.size(); }


	//
	// Calorie Data
	//
	// Why track calories in a map, and not in a static sized array like 
	// Bookkeeping?  The machine's clock is not guaranteed to be set correctly.
	// If calorie array is in a static sized array, playing on a machine with 
	// a mis-set clock could wipe out all your past data.  With this scheme, 
	// the worst that could happen is that playing on a mis-set machine will 
	// insert some garbage entries into the map.
	struct Calories
	{
		Calories() { fCals = 0; }
		float fCals;
	};
	map<DateTime,Calories> m_mapDayToCaloriesBurned;
	float GetCaloriesBurnedForDay( DateTime day ) const;
	

	//
	// Init'ing
	//
	void InitAll()
	{
		InitEditableData(); 
		InitGeneralData(); 
		InitSongScores(); 
		InitCourseScores(); 
		InitCategoryScores(); 
		InitScreenshotData(); 
		InitCalorieData(); 
	}
	void InitEditableData(); 
	void InitGeneralData(); 
	void InitSongScores(); 
	void InitCourseScores(); 
	void InitCategoryScores(); 
	void InitScreenshotData(); 
	void InitCalorieData(); 
	void ClearStats();

	//
	// Loading and saving
	//
	ProfileLoadResult LoadAllFromDir( RString sDir, bool bRequireSignature );
	bool SaveAllToDir( RString sDir, bool bSignData ) const;

	ProfileLoadResult LoadEditableDataFromDir( RString sDir );
	ProfileLoadResult LoadStatsXmlFromNode( const XNode* pNode, bool bIgnoreEditable = true );
	void LoadGeneralDataFromNode( const XNode* pNode );
	void LoadSongScoresFromNode( const XNode* pNode );
	void LoadCourseScoresFromNode( const XNode* pNode );
	void LoadCategoryScoresFromNode( const XNode* pNode );
	void LoadScreenshotDataFromNode( const XNode* pNode );
	void LoadCalorieDataFromNode( const XNode* pNode );
	
	void SaveEditableDataToDir( RString sDir ) const;
	bool SaveStatsXmlToDir( RString sDir, bool bSignData ) const;
	XNode* SaveStatsXmlCreateNode() const;
	XNode* SaveGeneralDataCreateNode() const;
	XNode* SaveSongScoresCreateNode() const;
	XNode* SaveCourseScoresCreateNode() const;
	XNode* SaveCategoryScoresCreateNode() const;
	XNode* SaveScreenshotDataCreateNode() const;
	XNode* SaveCalorieDataCreateNode() const;

	XNode* SaveCoinDataCreateNode() const;

	void SaveStatsWebPageToDir( RString sDir ) const;
	void SaveMachinePublicKeyToDir( RString sDir ) const;

	static void MoveBackupToDir( RString sFromDir, RString sToDir );
	static RString MakeUniqueFileNameNoExtension( RString sDir, RString sFileNameBeginning );
	static RString MakeFileNameNoExtension( RString sFileNameBeginning, int iIndex );


	// Lua
	void PushSelf( lua_State *L );

private:
	const HighScoresForASong *GetHighScoresForASong( const SongID& songID ) const;
	const HighScoresForACourse *GetHighScoresForACourse( const CourseID& courseID ) const;
};


#endif

/*
 * (c) 2001-2004 Chris Danford
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
