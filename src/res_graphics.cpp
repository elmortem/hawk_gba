#include "res_graphics.h"

#include "gr_sprites.pal.inc"
#include "gr_sprites.til.inc"

#include "gr_tiles.pal.inc"
#include "gr_tiles.til.inc"

#include "gr_mainmenu.pal.inc"
#include "gr_mainmenu.til.inc"

#include "gr_meta.pal.inc"
#include "gr_meta.til.inc"

#include "gr_end.pal.inc"
#include "gr_end.til.inc"

#include "gr_dialog.pal.inc"
#include "gr_dialog.til.inc"

#include "map_meta_bg.inc"
#include "map_meta_fg.inc"
#include "map_end_bg.inc"
#include "map_end_fg.inc"
#include "map_dialog_bg.inc"
#include "map_dialog_chr1.inc"
#include "map_dialog_chr2.inc"
#include "map_dialog_chr3.inc"
#include "map_mainmenu_bg.inc"
#include "map_mainmenu_fg.inc"
#include "map_mission1_bg.inc"
#include "map_mission1_fg.inc"
#include "map_mission1_obj.inc"
#include "map_mission2_bg.inc"
#include "map_mission2_fg.inc"
#include "map_mission2_obj.inc"
#include "map_mission3_bg.inc"
#include "map_mission3_fg.inc"
#include "map_mission3_obj.inc"
#include "map_mission4_bg.inc"
#include "map_mission4_fg.inc"
#include "map_mission4_obj.inc"
#include "map_mission5_bg.inc"
#include "map_mission5_fg.inc"
#include "map_mission5_obj.inc"

TileSet globalTileSet
{
    gr_tiles_til, gr_tiles_til_size,
    gr_tiles_opt, gr_tiles_opt_size,
    gr_tiles_pal, gr_tiles_pal_size,

    gr_sprites_til, gr_sprites_til_size,
    gr_sprites_pal, gr_sprites_pal_size,

    32
};
