static int solver_hard(struct latin_solver *solver, void *vctx)
{
    struct solver_ctx *ctx = (struct solver_ctx *)vctx;
    int w = ctx->w;
    int c, i, j, n, best, clue, start, step, ret;
    long bitmap;
#ifdef STANDALONE_SOLVER
    char prefix[256];
#endif

    /*
     * Go over every clue analysing all possibilities.
     */
    for (c = 0; c < 4*w; c++) {
	clue = ctx->clues[c];
	if (!clue)
	    continue;
	CSTARTSTEP(start, step, c, w);

	for (i = 0; i < w; i++)
	    ctx->iscratch[i] = 0;

	/*
	 * Instead of a tedious physical recursion, I iterate in the
	 * scratch array through all possibilities. At any given
	 * moment, i indexes the element of the box that will next
	 * be incremented.
	 */
	i = 0;
	ctx->dscratch[i] = 0;
	best = n = 0;
	bitmap = 0;

	while (1) {
	    if (i < w) {
		/*
		 * Find the next valid value for cell i.
		 */
		int limit = (n == clue ? best : w);
		int pos = start + step * i;
		for (j = ctx->dscratch[i] + 1; j <= limit; j++) {
		    if (bitmap & (1L << j))
			continue;      /* used this one already */
		    if (!solver->cube[pos*w+j-1])
			continue;      /* ruled out already */

		    /* Found one. */
		    break;
		}

		if (j > limit) {
		    /* No valid values left; drop back. */
		    i--;
		    if (i < 0)
			break;	       /* overall iteration is finished */
		    bitmap &= ~(1L << ctx->dscratch[i]);
		    if (ctx->dscratch[i] == best) {
			n--;
			best = 0;
			for (j = 0; j < i; j++)
			    if (best < ctx->dscratch[j])
				best = ctx->dscratch[j];
		    }
		} else {
		    /* Got a valid value; store it and move on. */
		    bitmap |= 1L << j;
		    ctx->dscratch[i++] = j;
		    if (j > best) {
			best = j;
			n++;
		    }
		    ctx->dscratch[i] = 0;
		}
	    } else {
		if (n == clue) {
		    for (j = 0; j < w; j++)
			ctx->iscratch[j] |= 1L << ctx->dscratch[j];
		}
		i--;
		bitmap &= ~(1L << ctx->dscratch[i]);
		if (ctx->dscratch[i] == best) {
		    n--;
		    best = 0;
		    for (j = 0; j < i; j++)
			if (best < ctx->dscratch[j])
			    best = ctx->dscratch[j];
		}
	    }
	}

#ifdef STANDALONE_SOLVER
	if (solver_show_working)
	    sprintf(prefix, "%*sexhaustive analysis of clue %s %d:\n",
		    solver_recurse_depth*4, "",
		    cluepos[c/w], c%w+1);
	else
	    prefix[0] = '\0';	       /* placate optimiser */
#endif

	ret = 0;

	for (i = 0; i < w; i++) {
	    int pos = start + step * i;
	    for (j = 1; j <= w; j++) {
		if (solver->cube[pos*w+j-1] &&
		    !(ctx->iscratch[i] & (1L << j))) {
#ifdef STANDALONE_SOLVER
		    if (solver_show_working) {
			printf("%s%*s  ruling out %d at (%d,%d)\n",
			       prefix, solver_recurse_depth*4, "",
			       j, pos/w+1, pos%w+1);
			prefix[0] = '\0';
		    }
#endif
		    solver->cube[pos*w+j-1] = 0;
		    ret = 1;
		}
	    }

	    /*
	     * Once we find one clue we can do something with in
	     * this way, revert to trying easier deductions, so as
	     * not to generate solver diagnostics that make the
	     * problem look harder than it is.
	     */
	    if (ret)
		return ret;
	}
    }

    return 0;
}