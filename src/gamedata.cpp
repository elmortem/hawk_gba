#include "gamedata.h"
#include "utils.h"

GameData gameData;

void GameData::calc( bool reset )
{
    if ( playerLevel > 50 )
        playerLevel = 50;

    // armor
    int arm = calcHealth( playerLevel );
    if ( arm > 999 )
        arm = 999;
    playerArmorMax = pix2unit( arm );
    if ( reset )
        playerArmor = playerArmorMax;
    else if ( playerArmor < pix2unit( 20 ) )
        playerArmor = pix2unit( 20 );

    // shield
    int shld = calcShield( playerLevel );
    if ( shld > 60 )
        shld = 60;
    playerShieldMax = shld * 60;
    if ( reset )
        playerShield = playerShieldMax;

    // damage
    int dmg = calcDamage( playerLevel );
    if ( dmg > 999 )
        dmg = 999;
    playerDamageMax = pix2unit( dmg );
}

void GameData::reset()
{
    playerLevel = 1;

    playerResEssence = 5;
    playerResLiquid = 10;
    playerResOre = 25;

    missionAccess = 0;

    calc( true );
}

int GameData::priceBuyLevel()
{
    if ( playerLevel >= 50 )
        return 9999;
    return std::max( 10, std::min( 999, ( playerLevel + 1 ) / 3 * 7 + 1 ) );
}

bool GameData::checkBuyLevel()
{
    if ( playerLevel >= 50 )
        return false;
    int price = priceBuyLevel();
    if ( playerResEssence < price )
        return false;

    return true;
}

bool GameData::buyLevel()
{
    if ( !checkBuyLevel() )
        return false;

    int price = priceBuyLevel();
    playerLevel += 1;

    playerResEssence -= price;

    calc( false );
    return true;
}

int GameData::priceBuyArmor()
{
    return 10;
}

bool GameData::checkBuyArmor()
{
    if ( playerArmor >= playerArmorMax )
        return false;
    int price = priceBuyArmor();
    if ( playerResOre < price )
        return false;

    return true;
}

bool GameData::buyArmor()
{
    if ( !checkBuyArmor() )
        return false;

    int price = priceBuyArmor();
    playerArmor += pix2unit( 30 );
    if( playerArmor > playerArmorMax )
        playerArmor = playerArmorMax;

    playerResOre -= price;
    return true;
}

int GameData::priceBuyShield()
{
    return 15;
}

bool GameData::checkBuyShield()
{
    if ( playerShield >= playerShieldMax )
        return false;
    int price = priceBuyShield();
    if ( playerResLiquid < price )
        return false;

    return true;
}

bool GameData::buyShield()
{
    if ( !checkBuyShield() )
        return false;

    int price = priceBuyShield();
    playerShield += 3 * 60;
    if( playerShield > playerShieldMax )
        playerShield = playerShieldMax;

    playerResLiquid -= price;
    return true;
}

void GameData::save()
{
    int s = 32;
    SRAM_BUFFER[ 16 ] = 138;
    SRAM_BUFFER[ s + 0 ] = playerLevel;
    SRAM_BUFFER[ s + 4 ] = playerResEssence;
    SRAM_BUFFER[ s + 8 ] = playerResLiquid;
    SRAM_BUFFER[ s + 12 ] = playerResOre;
    SRAM_BUFFER[ s + 16 ] = missionAccess;
}

void GameData::load()
{
    if ( !hasSave() )
    {
        reset();
        return;
    }

    int s = 32;
    playerLevel = SRAM_BUFFER[ s + 0 ];
    playerResEssence = SRAM_BUFFER[ s + 4 ];
    playerResLiquid = SRAM_BUFFER[ s + 8 ];
    playerResOre = SRAM_BUFFER[ s + 12 ];
    missionAccess = SRAM_BUFFER[ s + 16 ];

    calc( true );
}

bool GameData::hasSave()
{
    int check = SRAM_BUFFER[ 16 ];
    return check == 138;
}

int GameData::calcHealth( int level )
{
    if ( level < 1 )
        level = 1;

    return 25 + level * 3;
}

int GameData::calcDamage( int level )
{
    if ( level < 1 )
        level = 1;

    return ( 60 + ( level * level ) / 15 ) / 4;
}

int GameData::calcShield( int level )
{
    if ( level < 10 )
        return 0;

    return std::min( 15, level / 2 - 1 );
}


