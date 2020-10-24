static void game_print(drawing *dr, const game_state *state, int tilesize)
{
    int w = state->w, h = state->h;
    int c, x, y;

    /* Ick: fake up `ds->tilesize' for macro expansion purposes */
    game_drawstate ads, *ds = &ads;
    game_set_size(dr, ds, NULL, tilesize);

    c = print_mono_colour(dr, 1); assert(c == COL_BACKGROUND);
    c = print_mono_colour(dr, 0); assert(c == COL_TEXT);
    c = print_mono_colour(dr, 0); assert(c == COL_DOMINO);
    c = print_mono_colour(dr, 0); assert(c == COL_DOMINOCLASH);
    c = print_mono_colour(dr, 1); assert(c == COL_DOMINOTEXT);
    c = print_mono_colour(dr, 0); assert(c == COL_EDGE);

    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++) {
            int n = y*w+x;
	    unsigned long c;

            if (state->grid[n] == n-1)
                c = TYPE_R;
            else if (state->grid[n] == n+1)
                c = TYPE_L;
            else if (state->grid[n] == n-w)
                c = TYPE_B;
            else if (state->grid[n] == n+w)
                c = TYPE_T;
            else
                c = TYPE_BLANK;

	    draw_tile(dr, ds, state, x, y, c);
	}
}