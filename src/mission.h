#ifndef MISSION_H
#define MISSION_H

#include "video.h"

struct MissionDesc
{
    int width, height;
	GameMapLayerDesc background;
	GameMapLayerDesc foreground;
	int foregroundMult;

	const char * const * actors;

	int levelStart;
	int levelRandom;
};

class Mission
{
private:
    int line;
    int speed;
    int level;

public:
    MissionDesc* mission;

    Mission() {}
    ~Mission() {}

    void activate( MissionDesc* mission );
    void deactivate();
    void tick();
    void draw();

    int uLine() { return line; }
    int pLine() { return unit2pix( line ); }
    int uSpeed() { return speed; }
    int getLevel() { return level; }
};

extern Mission currentMission;

#endif // MISSION_H
