static game_state *execute_move(const game_state *from, const char *move)
{
    int gx, gy, dx, dy, ux, uy, up, p;
    game_state *ret;

    if (!strcmp(move, "S")) {
	int i;

	ret = dup_game(from);

	/*
	 * Simply replace the grid with a solved one. For this game,
	 * this isn't a useful operation for actually telling the user
	 * what they should have done, but it is useful for
	 * conveniently being able to get hold of a clean state from
	 * which to practise manoeuvres.
	 */
	for (i = 0; i < ret->n; i++)
	    ret->tiles[i] = (i+1) % ret->n;
	ret->gap_pos = ret->n-1;
	ret->used_solve = TRUE;
	ret->completed = ret->movecount = 1;

	return ret;
    }

    gx = X(from, from->gap_pos);
    gy = Y(from, from->gap_pos);

    if (move[0] != 'M' ||
	sscanf(move+1, "%d,%d", &dx, &dy) != 2 ||
	(dx == gx && dy == gy) || (dx != gx && dy != gy) ||
	dx < 0 || dx >= from->w || dy < 0 || dy >= from->h)
	return NULL;

    /*
     * Find the unit displacement from the original gap
     * position towards this one.
     */
    ux = (dx < gx ? -1 : dx > gx ? +1 : 0);
    uy = (dy < gy ? -1 : dy > gy ? +1 : 0);
    up = C(from, ux, uy);

    ret = dup_game(from);

    ret->gap_pos = C(from, dx, dy);
    assert(ret->gap_pos >= 0 && ret->gap_pos < ret->n);

    ret->tiles[ret->gap_pos] = 0;

    for (p = from->gap_pos; p != ret->gap_pos; p += up) {
        assert(p >= 0 && p < from->n);
        ret->tiles[p] = from->tiles[p + up];
	ret->movecount++;
    }

    /*
     * See if the game has been completed.
     */
    if (!ret->completed) {
        ret->completed = ret->movecount;
        for (p = 0; p < ret->n; p++)
            if (ret->tiles[p] != (p < ret->n-1 ? p+1 : 0))
                ret->completed = 0;
    }

    return ret;
}