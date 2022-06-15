#ifndef __MAPSTUFF_H__
#define __MAPSTUFF_H__

#include "mapclasses.h"
#include <climits>
#include "MultiMul.hpp"

const UI08 MAX_Z_STEP	= 9;
const UI08 MAX_Z_FALL	= 20;

/*
enum UOMapType
{
	UOMT_BRITANNIA	= 0,
	UOMT_UNKNOWN1	= 1,
	UOMT_ILSHENAR	= 2,
	UOMT_MALAS		= 3,
	UOMT_COUNT		= 4,
	UOMT_UNKNOWN	= 0xFF
};
*/

struct StaticsIndex_st
{
	UI32 offset;
	UI32 length;
	UI32 unknown;
	StaticsIndex_st() : offset( 0 ), length( 0 ), unknown( 0 )
	{
	}
};

struct MapData_st
{
	std::string		mapFile;
	std::string		mapFileUOPWrap;
	std::string		staticsFile;
	std::string		staidxFile;
	std::string		mapDiffFile;
	std::string		mapDiffListFile;
	std::string		staticsDiffFile;
	std::string		staticsDiffListFile;
	std::string		staticsDiffIndexFile;
	UI16			xBlock;
	UI16			yBlock;
	UOXFile	*mapObj;
	UOXFile 	*staticsObj;
	UOXFile 	*staidxObj;
	UOXFile 	*mapDiffObj;
	UOXFile 	*staticsDiffObj;

	std::map< UI32, UI32 > mapDiffList;
	std::map< UI32, StaticsIndex_st > staticsDiffIndex;

	MapData_st() : mapFile( "" ), mapFileUOPWrap( "" ), staticsFile( "" ), staidxFile( "" ), mapDiffFile( "" ), mapDiffListFile( "" ), staticsDiffFile( "" ),
	staticsDiffListFile( "" ), staticsDiffIndexFile( "" ), xBlock( 0 ), yBlock( 0 ), mapObj( nullptr ), staticsObj( nullptr ),
	staidxObj( nullptr ), mapDiffObj( nullptr ), staticsDiffObj( nullptr )
	{
		mapDiffList.clear();
		staticsDiffIndex.clear();
	}
	~MapData_st();
};

// full comments on this class are available in mapstuff.cpp
class CStaticIterator
{
private:
	Static_st	staticArray;
	SI16		baseX, baseY;
	UI32		pos;
	UI08		remainX, remainY;
	UI32		index, length;
	bool		exactCoords;
	UI08		worldNumber;
	bool		useDiffs;
public:
	CStaticIterator( SI16 x, SI16 y, UI08 world, bool exact = true );
	~CStaticIterator()
	{
	};

	Static_st *	First( void );
	Static_st *	Next( void );
	SI32		GetPos() const		{ return pos; }
	UI32		GetLength() const	{ return length; }
};



//===============================================================================================
// Replacement classes
//===============================================================================================
//===============================================================================================
// tileinfo 
//===============================================================================================
//===============================================================================================
class tileinfo {
	constexpr static auto hssize = 3188736;
	std::vector<CLand > terrain_data ;
	std::vector<CTile> art_data;
	auto processTerrain(std::ifstream &input) ->void ;
	auto processArt(std::ifstream &input) ->void ;
	bool isHS_format ;
public:
	tileinfo(const std::string &filename = "");
	auto load(const std::string &filename) ->bool ;
	auto terrain(std::uint16_t tileid) const ->const CLand &;
	auto terrain(std::uint16_t tileid) ->CLand & ;
	auto art(std::uint16_t tileid) ->CTile &;
	auto art(std::uint16_t tileid) const ->const CTile & ;
	
	auto sizeTerrain() const -> size_t ;
	auto sizeArt() const -> size_t ;
	
	auto collectionTerrain() const -> const std::vector<CLand>&;
	auto collectionTerrain() ->std::vector<CLand> &;
	auto collectionArt() const -> const std::vector<CTile>& ;
	auto collectionArt() ->std::vector<CTile>& ;
	auto totalMemory() const ->size_t ;
	
};


//==========================================================================================
// until we can replace
//==========================================================================================
class CMulHandler
{
private:
	tileinfo tile_info ;

	multicollection multi_data ;
	typedef std::vector< MapData_st >					MAPLIST;
	typedef std::vector< MapData_st >::iterator			MAPLIST_ITERATOR;
	
	//Variables
	
	MAPLIST MapList;
	
	// Functions
	CItem* DynTile( SI16 x, SI16 y, SI08 oldz, UI08 worldNumber, UI16 instanceID, bool checkOnlyMultis, bool checkOnlyNonMultis );
	
	// caching functions
	void			LoadMultis( const std::string& basePath );
	void			LoadDFNOverrides( void );
	void			LoadMapAndStatics( MapData_st& mMap, const std::string& basePath, UI08 &totalMaps );
	void			LoadTileData( const std::string& basePath );
	
public:
	
	
	
	
	
	CMulHandler() = default ;
	void			LoadMapsDFN();

	~CMulHandler() = default;
	
	void			Load( void );
	
	SI08			MultiHeight( CItem *i, SI16 x, SI16 y, SI08 oldz, SI08 maxZ, bool checkHeight = false );
	UI16			MultiTile( CItem *i, SI16 x, SI16 y, SI08 oldz, bool checkVisible = true );
	
	bool			DoesStaticBlock( SI16 x, SI16 y, SI08 oldz, UI08 worldNumber, bool checkWater = false );
	bool			DoesDynamicBlock( SI16 x, SI16 y, SI08 z, UI08 worldNumber, UI16 instanceID, bool checkWater, bool waterWalk, bool checkOnlyMultis, bool checkOnlyNonMultis );
	bool			DoesMapBlock( SI16 x, SI16 y, SI08 z, UI08 worldNumber, bool checkWater, bool waterWalk, bool checkMultiPlacement, bool checkForRoad );
	bool			CheckStaticFlag( SI16 x, SI16 y, SI08 oldz, UI08 worldNumber, TileFlags toCheck, bool checkSpawnSurface = false );
	bool			CheckDynamicFlag( SI16 x, SI16 y, SI08 z, UI08 worldNumber, UI16 instanceID, TileFlags toCheck );
	bool			CheckTileFlag( UI16 itemID, TileFlags flagToCheck );
	
	// height functions
	SI08			StaticTop( SI16 x, SI16 y, SI08 oldz, UI08 worldNumber, SI08 maxZ );
	SI08			DynamicElevation( SI16 x, SI16 y, SI08 oldz, UI08 worldNumber, SI08 maxZ, UI16 instanceID );
	SI08			MapElevation( SI16 x, SI16 y, UI08 worldNumber );
	SI08			TileHeight( UI16 tilenum );
	SI08			Height( SI16 x, SI16 y, SI08 oldz, UI08 worldNumber, UI16 instanceID );
	bool			inBuilding( SI16 x, SI16 y, SI08 z, UI08 worldNumber, UI16 instanceID );
	bool IsIgnored(UI16 landnum) {
		if (landnum == 2 || landnum == 0x1DB || ( landnum >= 0x1AE && landnum <= 0x1B5 )){
			return true;
		}
		else{
			return false;
		}
	}
	// ==================================================================================
	// Transisition methods
	auto artAt(std::int16_t x, std::int16_t y, std::uint8_t world) ->std::vector<tile_t> ;
	auto sizeOfMap(std::uint8_t worldNumber) const ->std::pair<int,int> ;
	auto diffCountForMap(std::uint8_t worldNumber) const ->std::pair<int,int> ;
	// look at tile functions
	void			MultiArea( CMultiObj *i, SI16 &x1, SI16 &y1, SI16 &x2, SI16 &y2 );
	bool			multiExists(UI16 multinum);
	
	auto SeekMulti( UI16 multinum ) const ->const collection_item&;
	bool			IsValidTile( UI16 tileNum );
	CTile &		SeekTile( UI16 tileNum );
	CLand &		SeekLand( UI16 landNum );
	tile_t		SeekMap( SI16 x, SI16 y, UI08 worldNumber );
	
	// misc functions
	bool			ValidSpawnLocation( SI16 x, SI16 y, SI08 z, UI08 worldNumber, UI16 instanceID, bool checkWater = true );
	UI08			ValidMultiLocation( SI16 x, SI16 y, SI08 oldz, UI08 worldNumber, UI16 instanceID, bool checkWater,
							 bool checkOnlyOtherMultis, bool checkOnlyNonMultis, bool checkForRoads );
	bool			MapExists( UI08 worldNumber );
	bool			InsideValidWorld( SI16 x, SI16 y, UI08 worldNumber = 0xFF );
	
	MapData_st&		GetMapData( UI08 mapNum );
	UI08			MapCount() const;
	
	size_t		GetTileMem() const;
	size_t		GetMultisMem() const;
};

extern CMulHandler *Map;
#endif

