static int solver_easy(struct latin_solver *solver, void *vctx)
{
    struct solver_ctx *ctx = (struct solver_ctx *)vctx;
    int w = ctx->w;
    int c, i, j, n, m, furthest;
    int start, step, cstart, cstep, clue, pos, cpos;
    int ret = 0;
#ifdef STANDALONE_SOLVER
    char prefix[256];
#endif

    if (!ctx->started) {
	ctx->started = TRUE;
	/*
	 * One-off loop to help get started: when a pair of facing
	 * clues sum to w+1, it must mean that the row consists of
	 * two increasing sequences back to back, so we can
	 * immediately place the highest digit by knowing the
	 * lengths of those two sequences.
	 */
	for (c = 0; c < 3*w; c = (c == w-1 ? 2*w : c+1)) {
	    int c2 = c + w;

	    if (ctx->clues[c] && ctx->clues[c2] &&
		ctx->clues[c] + ctx->clues[c2] == w+1) {
		STARTSTEP(start, step, c, w);
		CSTARTSTEP(cstart, cstep, c, w);
		pos = start + (ctx->clues[c]-1)*step;
		cpos = cstart + (ctx->clues[c]-1)*cstep;
		if (solver->cube[cpos*w+w-1]) {
#ifdef STANDALONE_SOLVER
		    if (solver_show_working) {
			printf("%*sfacing clues on %s %d are maximal:\n",
			       solver_recurse_depth*4, "",
			       c>=2*w ? "row" : "column", c % w + 1);
			printf("%*s  placing %d at (%d,%d)\n",
			       solver_recurse_depth*4, "",
			       w, pos%w+1, pos/w+1);
		    }
#endif
		    latin_solver_place(solver, pos%w, pos/w, w);
		    ret = 1;
		} else {
		    ret = -1;
		}
	    }
	}

	if (ret)
	    return ret;
    }

    /*
     * Go over every clue doing reasonably simple heuristic
     * deductions.
     */
    for (c = 0; c < 4*w; c++) {
	clue = ctx->clues[c];
	if (!clue)
	    continue;
	STARTSTEP(start, step, c, w);
	CSTARTSTEP(cstart, cstep, c, w);

	/* Find the location of each number in the row. */
	for (i = 0; i < w; i++)
	    ctx->dscratch[i] = w;
	for (i = 0; i < w; i++)
	    if (solver->grid[start+i*step])
		ctx->dscratch[solver->grid[start+i*step]-1] = i;

	n = m = 0;
	furthest = w;
	for (i = w; i >= 1; i--) {
	    if (ctx->dscratch[i-1] == w) {
		break;
	    } else if (ctx->dscratch[i-1] < furthest) {
		furthest = ctx->dscratch[i-1];
		m = i;
		n++;
	    }
	}
	if (clue == n+1 && furthest > 1) {
#ifdef STANDALONE_SOLVER
	    if (solver_show_working)
		sprintf(prefix, "%*sclue %s %d is nearly filled:\n",
			solver_recurse_depth*4, "",
			cluepos[c/w], c%w+1);
	    else
		prefix[0] = '\0';	       /* placate optimiser */
#endif
	    /*
	     * We can already see an increasing sequence of the very
	     * highest numbers, of length one less than that
	     * specified in the clue. All of those numbers _must_ be
	     * part of the clue sequence, so the number right next
	     * to the clue must be the final one - i.e. it must be
	     * bigger than any of the numbers between it and m. This
	     * allows us to rule out small numbers in that square.
	     *
	     * (This is a generalisation of the obvious deduction
	     * that when you see a clue saying 1, it must be right
	     * next to the largest possible number; and similarly,
	     * when you see a clue saying 2 opposite that, it must
	     * be right next to the second-largest.)
	     */
	    j = furthest-1;  /* number of small numbers we can rule out */
	    for (i = 1; i <= w && j > 0; i++) {
		if (ctx->dscratch[i-1] < w && ctx->dscratch[i-1] >= furthest)
		    continue;	       /* skip this number, it's elsewhere */
		j--;
		if (solver->cube[cstart*w+i-1]) {
#ifdef STANDALONE_SOLVER
		    if (solver_show_working) {
			printf("%s%*s  ruling out %d at (%d,%d)\n",
			       prefix, solver_recurse_depth*4, "",
			       i, start%w+1, start/w+1);
			prefix[0] = '\0';
		    }
#endif
		    solver->cube[cstart*w+i-1] = 0;
		    ret = 1;
		}
	    }
	}

	if (ret)
	    return ret;

#ifdef STANDALONE_SOLVER
	    if (solver_show_working)
		sprintf(prefix, "%*slower bounds for clue %s %d:\n",
			solver_recurse_depth*4, "",
			cluepos[c/w], c%w+1);
	    else
		prefix[0] = '\0';	       /* placate optimiser */
#endif

	i = 0;
	for (n = w; n > 0; n--) {
	    /*
	     * The largest number cannot occur in the first (clue-1)
	     * squares of the row, or else there wouldn't be space
	     * for a sufficiently long increasing sequence which it
	     * terminated. The second-largest number (not counting
	     * any that are known to be on the far side of a larger
	     * number and hence excluded from this sequence) cannot
	     * occur in the first (clue-2) squares, similarly, and
	     * so on.
	     */

	    if (ctx->dscratch[n-1] < w) {
		for (m = n+1; m < w; m++)
		    if (ctx->dscratch[m] < ctx->dscratch[n-1])
			break;
		if (m < w)
		    continue;	       /* this number doesn't count */
	    }

	    for (j = 0; j < clue - i - 1; j++)
		if (solver->cube[(cstart + j*cstep)*w+n-1]) {
#ifdef STANDALONE_SOLVER
		    if (solver_show_working) {
			int pos = start+j*step;
			printf("%s%*s  ruling out %d at (%d,%d)\n",
			       prefix, solver_recurse_depth*4, "",
			       n, pos%w+1, pos/w+1);
			prefix[0] = '\0';
		    }
#endif
		    solver->cube[(cstart + j*cstep)*w+n-1] = 0;
		    ret = 1;
		}
	    i++;
	}
    }

    if (ret)
	return ret;

    return 0;
}