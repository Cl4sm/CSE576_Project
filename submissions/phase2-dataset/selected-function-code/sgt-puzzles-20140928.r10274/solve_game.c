static char *solve_game(const game_state *state, const game_state *currstate,
                        const char *aux, char **error)
{
    int n = state->params.n, w = n+2, h = n+1, wh = w*h;
    int *placements;
    char *ret;
    int retlen, retsize;
    int i, v;
    char buf[80];
    int extra;

    if (aux) {
	retsize = 256;
	ret = snewn(retsize, char);
	retlen = sprintf(ret, "S");

	for (i = 0; i < wh; i++) {
	    if (aux[i] == 'L')
		extra = sprintf(buf, ";D%d,%d", i, i+1);
	    else if (aux[i] == 'T')
		extra = sprintf(buf, ";D%d,%d", i, i+w);
	    else
		continue;

	    if (retlen + extra + 1 >= retsize) {
		retsize = retlen + extra + 256;
		ret = sresize(ret, retsize, char);
	    }
	    strcpy(ret + retlen, buf);
	    retlen += extra;
	}

    } else {

	placements = snewn(wh*2, int);
	for (i = 0; i < wh*2; i++)
	    placements[i] = -3;
	solver(w, h, n, state->numbers->numbers, placements);

	/*
	 * First make a pass putting in edges for -1, then make a pass
	 * putting in dominoes for +1.
	 */
	retsize = 256;
	ret = snewn(retsize, char);
	retlen = sprintf(ret, "S");

	for (v = -1; v <= +1; v += 2)
	    for (i = 0; i < wh*2; i++)
		if (placements[i] == v) {
		    int p1 = i / 2;
		    int p2 = (i & 1) ? p1+1 : p1+w;

		    extra = sprintf(buf, ";%c%d,%d",
				    (int)(v==-1 ? 'E' : 'D'), p1, p2);

		    if (retlen + extra + 1 >= retsize) {
			retsize = retlen + extra + 256;
			ret = sresize(ret, retsize, char);
		    }
		    strcpy(ret + retlen, buf);
		    retlen += extra;
		}

	sfree(placements);
    }

    return ret;
}