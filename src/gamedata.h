#ifndef GAMEDATA_H
#define GAMEDATA_H

class GameData
{
public:
    int playerLevel;
    int playerArmor;
    int playerArmorMax;
    int playerShield;
    int playerShieldMax;
    int playerDamageMax;

    int playerResEssence;
    int playerResLiquid;
    int playerResOre;

    int missionAccess;

    void calc(bool reset = false);
    void reset();
    int priceBuyLevel();
    bool checkBuyLevel();
    bool buyLevel();
    int priceBuyArmor();
    bool checkBuyArmor();
    bool buyArmor();
    int priceBuyShield();
    bool checkBuyShield();
    bool buyShield();

    void save();
    void load();
    bool hasSave();

    static int calcHealth( int level );
    static int calcDamage( int level );
    static int calcShield( int level );
};

extern GameData gameData;

#endif // GAMEDATA_H
