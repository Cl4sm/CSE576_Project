static void tile_redraw(drawing *dr, game_drawstate *ds,
                        const game_state *state, int x, int y)
{
    unsigned int ds_flags = GRID(ds, flags, x, y);
    int dx = COORD(x), dy = COORD(y);
    int lit = (ds_flags & DF_FLASH) ? COL_GRID : COL_LIT;

    if (ds_flags & DF_BLACK) {
        draw_rect(dr, dx, dy, TILE_SIZE, TILE_SIZE, COL_BLACK);
        if (ds_flags & DF_NUMBERED) {
            int ccol = (ds_flags & DF_NUMBERWRONG) ? COL_ERROR : COL_LIGHT;
            char str[32];

            /* We know that this won't change over the course of the game
             * so it's OK to ignore this when calculating whether or not
             * to redraw the tile. */
            sprintf(str, "%d", GRID(state, lights, x, y));
            draw_text(dr, dx + TILE_SIZE/2, dy + TILE_SIZE/2,
                      FONT_VARIABLE, TILE_SIZE*3/5,
		      ALIGN_VCENTRE | ALIGN_HCENTRE, ccol, str);
        }
    } else {
        draw_rect(dr, dx, dy, TILE_SIZE, TILE_SIZE,
                  (ds_flags & DF_LIT) ? lit : COL_BACKGROUND);
        draw_rect_outline(dr, dx, dy, TILE_SIZE, TILE_SIZE, COL_GRID);
        if (ds_flags & DF_LIGHT) {
            int lcol = (ds_flags & DF_OVERLAP) ? COL_ERROR : COL_LIGHT;
            draw_circle(dr, dx + TILE_SIZE/2, dy + TILE_SIZE/2, TILE_RADIUS,
                        lcol, COL_BLACK);
        } else if ((ds_flags & DF_IMPOSSIBLE)) {
            static int draw_blobs_when_lit = -1;
            if (draw_blobs_when_lit < 0) {
		char *env = getenv("LIGHTUP_LIT_BLOBS");
		draw_blobs_when_lit = (!env || (env[0] == 'y' ||
                                                env[0] == 'Y'));
            }
            if (!(ds_flags & DF_LIT) || draw_blobs_when_lit) {
                int rlen = TILE_SIZE / 4;
                draw_rect(dr, dx + TILE_SIZE/2 - rlen/2,
                          dy + TILE_SIZE/2 - rlen/2,
                          rlen, rlen, COL_BLACK);
            }
        }
    }

    if (ds_flags & DF_CURSOR) {
        int coff = TILE_SIZE/8;
        draw_rect_outline(dr, dx + coff, dy + coff,
                          TILE_SIZE - coff*2, TILE_SIZE - coff*2, COL_CURSOR);
    }

    draw_update(dr, dx, dy, TILE_SIZE, TILE_SIZE);
}