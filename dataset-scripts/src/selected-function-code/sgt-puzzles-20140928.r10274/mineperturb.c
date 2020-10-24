static struct perturbations *mineperturb(void *vctx, signed char *grid,
					 int setx, int sety, int mask)
{
    struct minectx *ctx = (struct minectx *)vctx;
    struct square *sqlist;
    int x, y, dx, dy, i, n, nfull, nempty;
    struct square **tofill, **toempty, **todo;
    int ntofill, ntoempty, ntodo, dtodo, dset;
    struct perturbations *ret;
    int *setlist;

    if (!mask && !ctx->allow_big_perturbs)
	return NULL;

    /*
     * Make a list of all the squares in the grid which we can
     * possibly use. This list should be in preference order, which
     * means
     * 
     *  - first, unknown squares on the boundary of known space
     *  - next, unknown squares beyond that boundary
     * 	- as a very last resort, known squares, but not within one
     * 	  square of the starting position.
     * 
     * Each of these sections needs to be shuffled independently.
     * We do this by preparing list of all squares and then sorting
     * it with a random secondary key.
     */
    sqlist = snewn(ctx->w * ctx->h, struct square);
    n = 0;
    for (y = 0; y < ctx->h; y++)
	for (x = 0; x < ctx->w; x++) {
	    /*
	     * If this square is too near the starting position,
	     * don't put it on the list at all.
	     */
	    if (abs(y - ctx->sy) <= 1 && abs(x - ctx->sx) <= 1)
		continue;

	    /*
	     * If this square is in the input set, also don't put
	     * it on the list!
	     */
	    if ((mask == 0 && grid[y*ctx->w+x] == -2) ||
		(x >= setx && x < setx + 3 &&
		 y >= sety && y < sety + 3 &&
		 mask & (1 << ((y-sety)*3+(x-setx)))))
		continue;

	    sqlist[n].x = x;
	    sqlist[n].y = y;

	    if (grid[y*ctx->w+x] != -2) {
		sqlist[n].type = 3;    /* known square */
	    } else {
		/*
		 * Unknown square. Examine everything around it and
		 * see if it borders on any known squares. If it
		 * does, it's class 1, otherwise it's 2.
		 */

		sqlist[n].type = 2;

		for (dy = -1; dy <= +1; dy++)
		    for (dx = -1; dx <= +1; dx++)
			if (x+dx >= 0 && x+dx < ctx->w &&
			    y+dy >= 0 && y+dy < ctx->h &&
			    grid[(y+dy)*ctx->w+(x+dx)] != -2) {
			    sqlist[n].type = 1;
			    break;
			}
	    }

	    /*
	     * Finally, a random number to cause qsort to
	     * shuffle within each group.
	     */
	    sqlist[n].random = random_bits(ctx->rs, 31);

	    n++;
	}

    qsort(sqlist, n, sizeof(struct square), squarecmp);

    /*
     * Now count up the number of full and empty squares in the set
     * we've been provided.
     */
    nfull = nempty = 0;
    if (mask) {
	for (dy = 0; dy < 3; dy++)
	    for (dx = 0; dx < 3; dx++)
		if (mask & (1 << (dy*3+dx))) {
		    assert(setx+dx <= ctx->w);
		    assert(sety+dy <= ctx->h);
		    if (ctx->grid[(sety+dy)*ctx->w+(setx+dx)])
			nfull++;
		    else
			nempty++;
		}
    } else {
	for (y = 0; y < ctx->h; y++)
	    for (x = 0; x < ctx->w; x++)
		if (grid[y*ctx->w+x] == -2) {
		    if (ctx->grid[y*ctx->w+x])
			nfull++;
		    else
			nempty++;
		}
    }

    /*
     * Now go through our sorted list until we find either `nfull'
     * empty squares, or `nempty' full squares; these will be
     * swapped with the appropriate squares in the set to either
     * fill or empty the set while keeping the same number of mines
     * overall.
     */
    ntofill = ntoempty = 0;
    if (mask) {
	tofill = snewn(9, struct square *);
	toempty = snewn(9, struct square *);
    } else {
	tofill = snewn(ctx->w * ctx->h, struct square *);
	toempty = snewn(ctx->w * ctx->h, struct square *);
    }
    for (i = 0; i < n; i++) {
	struct square *sq = &sqlist[i];
	if (ctx->grid[sq->y * ctx->w + sq->x])
	    toempty[ntoempty++] = sq;
	else
	    tofill[ntofill++] = sq;
	if (ntofill == nfull || ntoempty == nempty)
	    break;
    }

    /*
     * If we haven't found enough empty squares outside the set to
     * empty it into _or_ enough full squares outside it to fill it
     * up with, we'll have to settle for doing only a partial job.
     * In this case we choose to always _fill_ the set (because
     * this case will tend to crop up when we're working with very
     * high mine densities and the only way to get a solvable grid
     * is going to be to pack most of the mines solidly around the
     * edges). So now our job is to make a list of the empty
     * squares in the set, and shuffle that list so that we fill a
     * random selection of them.
     */
    if (ntofill != nfull && ntoempty != nempty) {
	int k;

	assert(ntoempty != 0);

	setlist = snewn(ctx->w * ctx->h, int);
	i = 0;
	if (mask) {
	    for (dy = 0; dy < 3; dy++)
		for (dx = 0; dx < 3; dx++)
		    if (mask & (1 << (dy*3+dx))) {
			assert(setx+dx <= ctx->w);
			assert(sety+dy <= ctx->h);
			if (!ctx->grid[(sety+dy)*ctx->w+(setx+dx)])
			    setlist[i++] = (sety+dy)*ctx->w+(setx+dx);
		    }
	} else {
	    for (y = 0; y < ctx->h; y++)
		for (x = 0; x < ctx->w; x++)
		    if (grid[y*ctx->w+x] == -2) {
			if (!ctx->grid[y*ctx->w+x])
			    setlist[i++] = y*ctx->w+x;
		    }
	}
	assert(i > ntoempty);
	/*
	 * Now pick `ntoempty' items at random from the list.
	 */
	for (k = 0; k < ntoempty; k++) {
	    int index = k + random_upto(ctx->rs, i - k);
	    int tmp;

	    tmp = setlist[k];
	    setlist[k] = setlist[index];
	    setlist[index] = tmp;
	}
    } else
	setlist = NULL;

    /*
     * Now we're pretty much there. We need to either
     * 	(a) put a mine in each of the empty squares in the set, and
     * 	    take one out of each square in `toempty'
     * 	(b) take a mine out of each of the full squares in the set,
     * 	    and put one in each square in `tofill'
     * depending on which one we've found enough squares to do.
     * 
     * So we start by constructing our list of changes to return to
     * the solver, so that it can update its data structures
     * efficiently rather than having to rescan the whole grid.
     */
    ret = snew(struct perturbations);
    if (ntofill == nfull) {
	todo = tofill;
	ntodo = ntofill;
	dtodo = +1;
	dset = -1;
	sfree(toempty);
    } else {
	/*
	 * (We also fall into this case if we've constructed a
	 * setlist.)
	 */
	todo = toempty;
	ntodo = ntoempty;
	dtodo = -1;
	dset = +1;
	sfree(tofill);
    }
    ret->n = 2 * ntodo;
    ret->changes = snewn(ret->n, struct perturbation);
    for (i = 0; i < ntodo; i++) {
	ret->changes[i].x = todo[i]->x;
	ret->changes[i].y = todo[i]->y;
	ret->changes[i].delta = dtodo;
    }
    /* now i == ntodo */
    if (setlist) {
	int j;
	assert(todo == toempty);
	for (j = 0; j < ntoempty; j++) {
	    ret->changes[i].x = setlist[j] % ctx->w;
	    ret->changes[i].y = setlist[j] / ctx->w;
	    ret->changes[i].delta = dset;
	    i++;
	}
	sfree(setlist);
    } else if (mask) {
	for (dy = 0; dy < 3; dy++)
	    for (dx = 0; dx < 3; dx++)
		if (mask & (1 << (dy*3+dx))) {
		    int currval = (ctx->grid[(sety+dy)*ctx->w+(setx+dx)] ? +1 : -1);
		    if (dset == -currval) {
			ret->changes[i].x = setx + dx;
			ret->changes[i].y = sety + dy;
			ret->changes[i].delta = dset;
			i++;
		    }
		}
    } else {
	for (y = 0; y < ctx->h; y++)
	    for (x = 0; x < ctx->w; x++)
		if (grid[y*ctx->w+x] == -2) {
		    int currval = (ctx->grid[y*ctx->w+x] ? +1 : -1);
		    if (dset == -currval) {
			ret->changes[i].x = x;
			ret->changes[i].y = y;
			ret->changes[i].delta = dset;
			i++;
		    }
		}
    }
    assert(i == ret->n);

    sfree(sqlist);
    sfree(todo);

    /*
     * Having set up the precise list of changes we're going to
     * make, we now simply make them and return.
     */
    for (i = 0; i < ret->n; i++) {
	int delta;

	x = ret->changes[i].x;
	y = ret->changes[i].y;
	delta = ret->changes[i].delta;

	/*
	 * Check we're not trying to add an existing mine or remove
	 * an absent one.
	 */
	assert((delta < 0) ^ (ctx->grid[y*ctx->w+x] == 0));

	/*
	 * Actually make the change.
	 */
	ctx->grid[y*ctx->w+x] = (delta > 0);

	/*
	 * Update any numbers already present in the grid.
	 */
	for (dy = -1; dy <= +1; dy++)
	    for (dx = -1; dx <= +1; dx++)
		if (x+dx >= 0 && x+dx < ctx->w &&
		    y+dy >= 0 && y+dy < ctx->h &&
		    grid[(y+dy)*ctx->w+(x+dx)] != -2) {
		    if (dx == 0 && dy == 0) {
			/*
			 * The square itself is marked as known in
			 * the grid. Mark it as a mine if it's a
			 * mine, or else work out its number.
			 */
			if (delta > 0) {
			    grid[y*ctx->w+x] = -1;
			} else {
			    int dx2, dy2, minecount = 0;
			    for (dy2 = -1; dy2 <= +1; dy2++)
				for (dx2 = -1; dx2 <= +1; dx2++)
				    if (x+dx2 >= 0 && x+dx2 < ctx->w &&
					y+dy2 >= 0 && y+dy2 < ctx->h &&
					ctx->grid[(y+dy2)*ctx->w+(x+dx2)])
					minecount++;
			    grid[y*ctx->w+x] = minecount;
			}
		    } else {
			if (grid[(y+dy)*ctx->w+(x+dx)] >= 0)
			    grid[(y+dy)*ctx->w+(x+dx)] += delta;
		    }
		}
    }

#ifdef GENERATION_DIAGNOSTICS
    {
	int yy, xx;
	printf("grid after perturbing:\n");
	for (yy = 0; yy < ctx->h; yy++) {
	    for (xx = 0; xx < ctx->w; xx++) {
		int v = ctx->grid[yy*ctx->w+xx];
		if (yy == ctx->sy && xx == ctx->sx) {
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

    return ret;
}