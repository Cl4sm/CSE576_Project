static int latin_solver_recurse
    (struct latin_solver *solver, int diff_simple, int diff_set_0,
     int diff_set_1, int diff_forcing, int diff_recursive,
     usersolver_t const *usersolvers, void *ctx,
     ctxnew_t ctxnew, ctxfree_t ctxfree)
{
    int best, bestcount;
    int o = solver->o, x, y, n;
#ifdef STANDALONE_SOLVER
    char **names = solver->names;
#endif

    best = -1;
    bestcount = o+1;

    for (y = 0; y < o; y++)
        for (x = 0; x < o; x++)
            if (!solver->grid[y*o+x]) {
                int count;

                /*
                 * An unfilled square. Count the number of
                 * possible digits in it.
                 */
                count = 0;
                for (n = 1; n <= o; n++)
                    if (cube(x,y,n))
                        count++;

                /*
                 * We should have found any impossibilities
                 * already, so this can safely be an assert.
                 */
                assert(count > 1);

                if (count < bestcount) {
                    bestcount = count;
                    best = y*o+x;
                }
            }

    if (best == -1)
        /* we were complete already. */
        return 0;
    else {
        int i, j;
        digit *list, *ingrid, *outgrid;
        int diff = diff_impossible;    /* no solution found yet */

        /*
         * Attempt recursion.
         */
        y = best / o;
        x = best % o;

        list = snewn(o, digit);
        ingrid = snewn(o*o, digit);
        outgrid = snewn(o*o, digit);
        memcpy(ingrid, solver->grid, o*o);

        /* Make a list of the possible digits. */
        for (j = 0, n = 1; n <= o; n++)
            if (cube(x,y,n))
                list[j++] = n;

#ifdef STANDALONE_SOLVER
        if (solver_show_working) {
            char *sep = "";
            printf("%*srecursing on (%d,%d) [",
                   solver_recurse_depth*4, "", x+1, y+1);
            for (i = 0; i < j; i++) {
                printf("%s%s", sep, names[list[i]-1]);
                sep = " or ";
            }
            printf("]\n");
        }
#endif

        /*
         * And step along the list, recursing back into the
         * main solver at every stage.
         */
        for (i = 0; i < j; i++) {
            int ret;
	    void *newctx;
	    struct latin_solver subsolver;

            memcpy(outgrid, ingrid, o*o);
            outgrid[y*o+x] = list[i];

#ifdef STANDALONE_SOLVER
            if (solver_show_working)
                printf("%*sguessing %s at (%d,%d)\n",
                       solver_recurse_depth*4, "", names[list[i]-1], x+1, y+1);
            solver_recurse_depth++;
#endif

	    if (ctxnew) {
		newctx = ctxnew(ctx);
	    } else {
		newctx = ctx;
	    }
	    latin_solver_alloc(&subsolver, outgrid, o);
#ifdef STANDALONE_SOLVER
	    subsolver.names = solver->names;
#endif
            ret = latin_solver_top(&subsolver, diff_recursive,
				   diff_simple, diff_set_0, diff_set_1,
				   diff_forcing, diff_recursive,
				   usersolvers, newctx, ctxnew, ctxfree);
	    latin_solver_free(&subsolver);
	    if (ctxnew)
		ctxfree(newctx);

#ifdef STANDALONE_SOLVER
            solver_recurse_depth--;
            if (solver_show_working) {
                printf("%*sretracting %s at (%d,%d)\n",
                       solver_recurse_depth*4, "", names[list[i]-1], x+1, y+1);
            }
#endif
            /* we recurse as deep as we can, so we should never find
             * find ourselves giving up on a puzzle without declaring it
             * impossible.  */
            assert(ret != diff_unfinished);

            /*
             * If we have our first solution, copy it into the
             * grid we will return.
             */
            if (diff == diff_impossible && ret != diff_impossible)
                memcpy(solver->grid, outgrid, o*o);

            if (ret == diff_ambiguous)
                diff = diff_ambiguous;
            else if (ret == diff_impossible)
                /* do not change our return value */;
            else {
                /* the recursion turned up exactly one solution */
                if (diff == diff_impossible)
                    diff = diff_recursive;
                else
                    diff = diff_ambiguous;
            }

            /*
             * As soon as we've found more than one solution,
             * give up immediately.
             */
            if (diff == diff_ambiguous)
                break;
        }

        sfree(outgrid);
        sfree(ingrid);
        sfree(list);

        if (diff == diff_impossible)
            return -1;
        else if (diff == diff_ambiguous)
            return 2;
        else {
            assert(diff == diff_recursive);
            return 1;
        }
    }
}