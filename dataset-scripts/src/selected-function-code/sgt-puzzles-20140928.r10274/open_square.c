static int open_square(game_state *state, int x, int y)
{
    int w = state->w, h = state->h;
    int xx, yy, nmines, ncovered;

    if (!state->layout->mines) {
	/*
	 * We have a preliminary game in which the mine layout
	 * hasn't been generated yet. Generate it based on the
	 * initial click location.
	 */
	char *desc, *privdesc;
	state->layout->mines = new_mine_layout(w, h, state->layout->n,
					       x, y, state->layout->unique,
					       state->layout->rs,
					       &desc);
	/*
	 * Find the trailing substring of the game description
	 * corresponding to just the mine layout; we will use this
	 * as our second `private' game ID for serialisation.
	 */
	privdesc = desc;
	while (*privdesc && isdigit((unsigned char)*privdesc)) privdesc++;
	if (*privdesc == ',') privdesc++;
	while (*privdesc && isdigit((unsigned char)*privdesc)) privdesc++;
	if (*privdesc == ',') privdesc++;
	assert(*privdesc == 'm');
	midend_supersede_game_desc(state->layout->me, desc, privdesc);
	sfree(desc);
	random_free(state->layout->rs);
	state->layout->rs = NULL;
    }

    if (state->layout->mines[y*w+x]) {
	/*
	 * The player has landed on a mine. Bad luck. Expose the
	 * mine that killed them, but not the rest (in case they
	 * want to Undo and carry on playing).
	 */
	state->dead = TRUE;
	state->grid[y*w+x] = 65;
	return -1;
    }

    /*
     * Otherwise, the player has opened a safe square. Mark it to-do.
     */
    state->grid[y*w+x] = -10;	       /* `todo' value internal to this func */

    /*
     * Now go through the grid finding all `todo' values and
     * opening them. Every time one of them turns out to have no
     * neighbouring mines, we add all its unopened neighbours to
     * the list as well.
     * 
     * FIXME: We really ought to be able to do this better than
     * using repeated N^2 scans of the grid.
     */
    while (1) {
	int done_something = FALSE;

	for (yy = 0; yy < h; yy++)
	    for (xx = 0; xx < w; xx++)
		if (state->grid[yy*w+xx] == -10) {
		    int dx, dy, v;

		    assert(!state->layout->mines[yy*w+xx]);

		    v = 0;

		    for (dx = -1; dx <= +1; dx++)
			for (dy = -1; dy <= +1; dy++)
			    if (xx+dx >= 0 && xx+dx < state->w &&
				yy+dy >= 0 && yy+dy < state->h &&
				state->layout->mines[(yy+dy)*w+(xx+dx)])
				v++;

		    state->grid[yy*w+xx] = v;

		    if (v == 0) {
			for (dx = -1; dx <= +1; dx++)
			    for (dy = -1; dy <= +1; dy++)
				if (xx+dx >= 0 && xx+dx < state->w &&
				    yy+dy >= 0 && yy+dy < state->h &&
				    state->grid[(yy+dy)*w+(xx+dx)] == -2)
				    state->grid[(yy+dy)*w+(xx+dx)] = -10;
		    }

		    done_something = TRUE;
		}

	if (!done_something)
	    break;
    }

    /*
     * Finally, scan the grid and see if exactly as many squares
     * are still covered as there are mines. If so, set the `won'
     * flag and fill in mine markers on all covered squares.
     */
    nmines = ncovered = 0;
    for (yy = 0; yy < h; yy++)
	for (xx = 0; xx < w; xx++) {
	    if (state->grid[yy*w+xx] < 0)
		ncovered++;
	    if (state->layout->mines[yy*w+xx])
		nmines++;
	}
    assert(ncovered >= nmines);
    if (ncovered == nmines) {
	for (yy = 0; yy < h; yy++)
	    for (xx = 0; xx < w; xx++) {
		if (state->grid[yy*w+xx] < 0)
		    state->grid[yy*w+xx] = -1;
	}
	state->won = TRUE;
    }

    return 0;
}