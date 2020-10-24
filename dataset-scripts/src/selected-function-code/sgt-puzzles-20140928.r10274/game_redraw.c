static void game_redraw(drawing *dr, game_drawstate *ds,
                        const game_state *oldstate, const game_state *state,
                        int dir, const game_ui *ui,
                        float animtime, float flashtime)
{
    int i, pass, bgcolour;

    if (flashtime > 0) {
        int frame = (int)(flashtime / FLASH_FRAME);
        bgcolour = (frame % 2 ? COL_LOWLIGHT : COL_HIGHLIGHT);
    } else
        bgcolour = COL_BACKGROUND;

    if (!ds->started) {
        int coords[10];

	draw_rect(dr, 0, 0,
		  TILE_SIZE * state->w + 2 * BORDER,
		  TILE_SIZE * state->h + 2 * BORDER, COL_BACKGROUND);
	draw_update(dr, 0, 0,
		    TILE_SIZE * state->w + 2 * BORDER,
		    TILE_SIZE * state->h + 2 * BORDER);

        /*
         * Recessed area containing the whole puzzle.
         */
        coords[0] = COORD(state->w) + HIGHLIGHT_WIDTH - 1;
        coords[1] = COORD(state->h) + HIGHLIGHT_WIDTH - 1;
        coords[2] = COORD(state->w) + HIGHLIGHT_WIDTH - 1;
        coords[3] = COORD(0) - HIGHLIGHT_WIDTH;
        coords[4] = coords[2] - TILE_SIZE;
        coords[5] = coords[3] + TILE_SIZE;
        coords[8] = COORD(0) - HIGHLIGHT_WIDTH;
        coords[9] = COORD(state->h) + HIGHLIGHT_WIDTH - 1;
        coords[6] = coords[8] + TILE_SIZE;
        coords[7] = coords[9] - TILE_SIZE;
        draw_polygon(dr, coords, 5, COL_HIGHLIGHT, COL_HIGHLIGHT);

        coords[1] = COORD(0) - HIGHLIGHT_WIDTH;
        coords[0] = COORD(0) - HIGHLIGHT_WIDTH;
        draw_polygon(dr, coords, 5, COL_LOWLIGHT, COL_LOWLIGHT);

        ds->started = TRUE;
    }

    /*
     * Now draw each tile. We do this in two passes to make
     * animation easy.
     */
    for (pass = 0; pass < 2; pass++) {
        for (i = 0; i < state->n; i++) {
            int t, t0;
            /*
             * Figure out what should be displayed at this
             * location. It's either a simple tile, or it's a
             * transition between two tiles (in which case we say
             * -1 because it must always be drawn).
             */

            if (oldstate && oldstate->tiles[i] != state->tiles[i])
                t = -1;
            else
                t = state->tiles[i];

            t0 = t;

            if (ds->bgcolour != bgcolour ||   /* always redraw when flashing */
                ds->tiles[i] != t || ds->tiles[i] == -1 || t == -1) {
                int x, y;

                /*
                 * Figure out what to _actually_ draw, and where to
                 * draw it.
                 */
                if (t == -1) {
                    int x0, y0, x1, y1;
                    int j;

                    /*
                     * On the first pass, just blank the tile.
                     */
                    if (pass == 0) {
                        x = COORD(X(state, i));
                        y = COORD(Y(state, i));
                        t = 0;
                    } else {
                        float c;

                        t = state->tiles[i];

                        /*
                         * Don't bother moving the gap; just don't
                         * draw it.
                         */
                        if (t == 0)
                            continue;

                        /*
                         * Find the coordinates of this tile in the old and
                         * new states.
                         */
                        x1 = COORD(X(state, i));
                        y1 = COORD(Y(state, i));
                        for (j = 0; j < oldstate->n; j++)
                            if (oldstate->tiles[j] == state->tiles[i])
                                break;
                        assert(j < oldstate->n);
                        x0 = COORD(X(state, j));
                        y0 = COORD(Y(state, j));

                        c = (animtime / ANIM_TIME);
                        if (c < 0.0F) c = 0.0F;
                        if (c > 1.0F) c = 1.0F;

                        x = x0 + (int)(c * (x1 - x0));
                        y = y0 + (int)(c * (y1 - y0));
                    }

                } else {
                    if (pass == 0)
                        continue;
                    x = COORD(X(state, i));
                    y = COORD(Y(state, i));
                }

                draw_tile(dr, ds, state, x, y, t, bgcolour);
            }
            ds->tiles[i] = t0;
        }
    }
    ds->bgcolour = bgcolour;

    /*
     * Update the status bar.
     */
    {
	char statusbuf[256];

        /*
         * Don't show the new status until we're also showing the
         * new _state_ - after the game animation is complete.
         */
        if (oldstate)
            state = oldstate;

	if (state->used_solve)
	    sprintf(statusbuf, "Moves since auto-solve: %d",
		    state->movecount - state->completed);
	else
	    sprintf(statusbuf, "%sMoves: %d",
		    (state->completed ? "COMPLETED! " : ""),
		    (state->completed ? state->completed : state->movecount));

	status_bar(dr, statusbuf);
    }
}