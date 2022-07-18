#include "res_sounds.h"

#include "snd_bank.inc"

#include "audio.h"

#define SND_LIB_ENTRY( name ) { snd_##name, snd_##name##_size }

const SoundLibEntry soundLib[ Snd_Count ] = {
	SND_LIB_ENTRY( lobby ),
	SND_LIB_ENTRY( gameplay ),
	SND_LIB_ENTRY( click ),
	SND_LIB_ENTRY( touch ),
	SND_LIB_ENTRY( bonus ),
	SND_LIB_ENTRY( cow ),
	SND_LIB_ENTRY( shoot ),
	SND_LIB_ENTRY( shoot_power ),
	SND_LIB_ENTRY( player_shoot ),
	SND_LIB_ENTRY( expl ),
	SND_LIB_ENTRY( buy ),
	SND_LIB_ENTRY( levelup ),
	SND_LIB_ENTRY( fail ),
	SND_LIB_ENTRY( state ),
	SND_LIB_ENTRY( dialog )
};
