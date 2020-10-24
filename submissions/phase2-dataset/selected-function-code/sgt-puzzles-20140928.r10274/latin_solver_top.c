static int latin_solver_top(struct latin_solver *solver, int maxdiff,
			    int diff_simple, int diff_set_0, int diff_set_1,
			    int diff_forcing, int diff_recursive,
			    usersolver_t const *usersolvers, void *ctx,
			    ctxnew_t ctxnew, ctxfree_t ctxfree)
{
    struct latin_solver_scratch *scratch = latin_solver_new_scratch(solver);
    int ret, diff = diff_simple;

    assert(maxdiff <= diff_recursive);
    /*
     * Now loop over the grid repeatedly trying all permitted modes
     * of reasoning. The loop terminates if we complete an
     * iteration without making any progress; we then return
     * failure or success depending on whether the grid is full or
     * not.
     */
    while (1) {
	int i;

	cont:

        latin_solver_debug(solver->cube, solver->o);

	for (i = 0; i <= maxdiff; i++) {
	    if (usersolvers[i])
		ret = usersolvers[i](solver, ctx);
	    else
		ret = 0;
	    if (ret == 0 && i == diff_simple)
		ret = latin_solver_diff_simple(solver);
	    if (ret == 0 && i == diff_set_0)
		ret = latin_solver_diff_set(solver, scratch, 0);
	    if (ret == 0 && i == diff_set_1)
		ret = latin_solver_diff_set(solver, scratch, 1);
	    if (ret == 0 && i == diff_forcing)
		ret = latin_solver_forcing(solver, scratch);

	    if (ret < 0) {
		diff = diff_impossible;
		goto got_result;
	    } else if (ret > 0) {
		diff = max(diff, i);
		goto cont;
	    }
	}

        /*
         * If we reach here, we have made no deductions in this
         * iteration, so the algorithm terminates.
         */
        break;
    }

    /*
     * Last chance: if we haven't fully solved the puzzle yet, try
     * recursing based on guesses for a particular square. We pick
     * one of the most constrained empty squares we can find, which
     * has the effect of pruning the search tree as much as
     * possible.
     */
    if (maxdiff == diff_recursive) {
        int nsol = latin_solver_recurse(solver,
					diff_simple, diff_set_0, diff_set_1,
					diff_forcing, diff_recursive,
					usersolvers, ctx, ctxnew, ctxfree);
        if (nsol < 0) diff = diff_impossible;
        else if (nsol == 1) diff = diff_recursive;
        else if (nsol > 1) diff = diff_ambiguous;
        /* if nsol == 0 then we were complete anyway
         * (and thus don't need to change diff) */
    } else {
        /*
         * We're forbidden to use recursion, so we just see whether
         * our grid is fully solved, and return diff_unfinished
         * otherwise.
         */
        int x, y, o = solver->o;

        for (y = 0; y < o; y++)
            for (x = 0; x < o; x++)
                if (!solver->grid[y*o+x])
                    diff = diff_unfinished;
    }

    got_result:

#ifdef STANDALONE_SOLVER
    if (solver_show_working)
        printf("%*s%s found\n",
               solver_recurse_depth*4, "",
               diff == diff_impossible ? "no solution (impossible)" :
               diff == diff_unfinished ? "no solution (unfinished)" :
               diff == diff_ambiguous ? "multiple solutions" :
               "one solution");
#endif

    latin_solver_free_scratch(scratch);

    return diff;
}