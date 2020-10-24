static int check_errors(const game_state *state, int *errors)
{
    int w = state->par.w /*, a = w*w */;
    int W = w+2, A = W*W;	       /* the errors array is (w+2) square */
    int *clues = state->clues->clues;
    digit *grid = state->grid;
    int i, x, y, errs = FALSE;
    int tmp[32];

    assert(w < lenof(tmp));

    if (errors)
	for (i = 0; i < A; i++)
	    errors[i] = 0;

    for (y = 0; y < w; y++) {
	unsigned long mask = 0, errmask = 0;
	for (x = 0; x < w; x++) {
	    unsigned long bit = 1UL << grid[y*w+x];
	    errmask |= (mask & bit);
	    mask |= bit;
	}

	if (mask != (1L << (w+1)) - (1L << 1)) {
	    errs = TRUE;
	    errmask &= ~1UL;
	    if (errors) {
		for (x = 0; x < w; x++)
		    if (errmask & (1UL << grid[y*w+x]))
			errors[(y+1)*W+(x+1)] = TRUE;
	    }
	}
    }

    for (x = 0; x < w; x++) {
	unsigned long mask = 0, errmask = 0;
	for (y = 0; y < w; y++) {
	    unsigned long bit = 1UL << grid[y*w+x];
	    errmask |= (mask & bit);
	    mask |= bit;
	}

	if (mask != (1 << (w+1)) - (1 << 1)) {
	    errs = TRUE;
	    errmask &= ~1UL;
	    if (errors) {
		for (y = 0; y < w; y++)
		    if (errmask & (1UL << grid[y*w+x]))
			errors[(y+1)*W+(x+1)] = TRUE;
	    }
	}
    }

    for (i = 0; i < 4*w; i++) {
	int start, step, j, n, best;
	STARTSTEP(start, step, i, w);

	if (!clues[i])
	    continue;

	best = n = 0;
	for (j = 0; j < w; j++) {
	    int number = grid[start+j*step];
	    if (!number)
		break;		       /* can't tell what happens next */
	    if (number > best) {
		best = number;
		n++;
	    }
	}

	if (n > clues[i] || (j == w && n < clues[i])) {
	    if (errors) {
		int x, y;
		CLUEPOS(x, y, i, w);
		errors[(y+1)*W+(x+1)] = TRUE;
	    }
	    errs = TRUE;
	}
    }

    return errs;
}