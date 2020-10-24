int latin_solver_main(struct latin_solver *solver, int maxdiff,
		      int diff_simple, int diff_set_0, int diff_set_1,
		      int diff_forcing, int diff_recursive,
		      usersolver_t const *usersolvers, void *ctx,
		      ctxnew_t ctxnew, ctxfree_t ctxfree)
{
    int diff;
#ifdef STANDALONE_SOLVER
    int o = solver->o;
    char *text = NULL, **names = NULL;
#endif

#ifdef STANDALONE_SOLVER
    if (!solver->names) {
	char *p;
	int i;

	text = snewn(40 * o, char);
	p = text;

	solver->names = snewn(o, char *);

	for (i = 0; i < o; i++) {
	    solver->names[i] = p;
	    p += 1 + sprintf(p, "%d", i+1);
	}
    }
#endif

    diff = latin_solver_top(solver, maxdiff,
			    diff_simple, diff_set_0, diff_set_1,
			    diff_forcing, diff_recursive,
			    usersolvers, ctx, ctxnew, ctxfree);

#ifdef STANDALONE_SOLVER
    sfree(names);
    sfree(text);
#endif

    return diff;
}