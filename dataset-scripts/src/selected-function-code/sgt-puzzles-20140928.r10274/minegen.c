static char *minegen(int w, int h, int n, int x, int y, int unique,
		     random_state *rs)
{
    char *ret = snewn(w*h, char);
    int success;
    int ntries = 0;

    do {
	success = FALSE;
	ntries++;

	memset(ret, 0, w*h);

	/*
	 * Start by placing n mines, none of which is at x,y or within
	 * one square of it.
	 */
	{
	    int *tmp = snewn(w*h, int);
	    int i, j, k, nn;

	    /*
	     * Write down the list of possible mine locations.
	     */
	    k = 0;
	    for (i = 0; i < h; i++)
		for (j = 0; j < w; j++)
		    if (abs(i - y) > 1 || abs(j - x) > 1)
			tmp[k++] = i*w+j;

	    /*
	     * Now pick n off the list at random.
	     */
	    nn = n;
	    while (nn-- > 0) {
		i = random_upto(rs, k);
		ret[tmp[i]] = 1;
		tmp[i] = tmp[--k];
	    }

	    sfree(tmp);
	}

#ifdef GENERATION_DIAGNOSTICS
	{
	    int yy, xx;
	    printf("grid after initial generation:\n");
	    for (yy = 0; yy < h; yy++) {
		for (xx = 0; xx < w; xx++) {
		    int v = ret[yy*w+xx];
		    if (yy == y && xx == x) {
			assert(!v);
			putchar('S');
		    } else if (v) {
			putchar('*');
		    } else {
			putchar('-');
		    }
		}
		putchar('\n');
	    }
	    printf("\n");
	}
#endif

	/*
	 * Now set up a results grid to run the solver in, and a
	 * context for the solver to open squares. Then run the solver
	 * repeatedly; if the number of perturb steps ever goes up or
	 * it ever returns -1, give up completely.
	 *
	 * We bypass this bit if we're not after a unique grid.
         */
	if (unique) {
	    signed char *solvegrid = snewn(w*h, signed char);
	    struct minectx actx, *ctx = &actx;
	    int solveret, prevret = -2;

	    ctx->grid = ret;
	    ctx->w = w;
	    ctx->h = h;
	    ctx->sx = x;
	    ctx->sy = y;
	    ctx->rs = rs;
	    ctx->allow_big_perturbs = (ntries > 100);

	    while (1) {
		memset(solvegrid, -2, w*h);
		solvegrid[y*w+x] = mineopen(ctx, x, y);
		assert(solvegrid[y*w+x] == 0); /* by deliberate arrangement */

		solveret =
		    minesolve(w, h, n, solvegrid, mineopen, mineperturb, ctx, rs);
		if (solveret < 0 || (prevret >= 0 && solveret >= prevret)) {
		    success = FALSE;
		    break;
		} else if (solveret == 0) {
		    success = TRUE;
		    break;
		}
	    }

	    sfree(solvegrid);
	} else {
	    success = TRUE;
	}

    } while (!success);

    return ret;
}