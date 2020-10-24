static int find_errors(const game_state *state, int *report)
{
    int const w = state->params.w, h = state->params.h, n = w * h;
    int *dsf;

    int r, c, i;

    int nblack = 0, any_white_cell = -1;
    game_state *dup = dup_game(state);

    for (i = r = 0; r < h; ++r)
        for (c = 0; c < w; ++c, ++i) {
            switch (state->grid[i]) {

	      case BLACK:
		{
		    int j;
		    ++nblack;
		    for (j = 0; j < 4; ++j) {
			int const rr = r + dr[j], cc = c + dc[j];
			if (out_of_bounds(rr, cc, w, h)) continue;
			if (state->grid[idx(rr, cc, w)] != BLACK) continue;
			if (!report) goto found_error;
			report[i] = TRUE;
			break;
		    }
		}
                break;
	      default:
		{
		    int j, runs;
		    for (runs = 1, j = 0; j < 4; ++j) {
			int const rr = r + dr[j], cc = c + dc[j];
			runs += runlength(rr, cc, dr[j], dc[j], state,
					  ~MASK(BLACK));
		    }
		    if (!report) {
			if (runs != state->grid[i]) goto found_error;
		    } else if (runs < state->grid[i]) report[i] = TRUE;
		    else {
			for (runs = 1, j = 0; j < 4; ++j) {
			    int const rr = r + dr[j], cc = c + dc[j];
			    runs += runlength(rr, cc, dr[j], dc[j], state,
					      ~(MASK(BLACK) | MASK(EMPTY)));
			}
			if (runs > state->grid[i]) report[i] = TRUE;
		    }
		}

                /* note: fallthrough _into_ these cases */
	      case EMPTY:
	      case WHITE: any_white_cell = i;
            }
        }

    /*
     * Check that all the white cells form a single connected component.
     */
    dsf = snew_dsf(n);
    for (r = 0; r < h-1; ++r)
        for (c = 0; c < w; ++c)
            if (state->grid[r*w+c] != BLACK &&
                state->grid[(r+1)*w+c] != BLACK)
                dsf_merge(dsf, r*w+c, (r+1)*w+c);
    for (r = 0; r < h; ++r)
        for (c = 0; c < w-1; ++c)
            if (state->grid[r*w+c] != BLACK &&
                state->grid[r*w+(c+1)] != BLACK)
                dsf_merge(dsf, r*w+c, r*w+(c+1));
    if (nblack + dsf_size(dsf, any_white_cell) < n) {
        int biggest, canonical;

        if (!report) {
            printf("dfs fail at %d\n", any_white_cell);
            goto found_error;
        }

        /*
         * Report this error by choosing one component to be the
         * canonical one (we pick the largest, arbitrarily
         * tie-breaking towards lower array indices) and highlighting
         * as an error any square in a different component.
         */
        canonical = -1;
        biggest = 0;
        for (i = 0; i < n; ++i)
            if (state->grid[i] != BLACK) {
                int size = dsf_size(dsf, i);
                if (size > biggest) {
                    biggest = size;
                    canonical = dsf_canonify(dsf, i);
                }
            }

        for (i = 0; i < n; ++i)
            if (state->grid[i] != BLACK && dsf_canonify(dsf, i) != canonical)
                report[i] = TRUE;
    }
    sfree(dsf);

    free_game(dup);
    return FALSE; /* if report != NULL, this is ignored */

found_error:
    free_game(dup);
    return TRUE;
}