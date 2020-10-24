int latin_solver_diff_simple(struct latin_solver *solver)
{
    int x, y, n, ret, o = solver->o;
#ifdef STANDALONE_SOLVER
    char **names = solver->names;
#endif

    /*
     * Row-wise positional elimination.
     */
    for (y = 0; y < o; y++)
        for (n = 1; n <= o; n++)
            if (!solver->row[y*o+n-1]) {
                ret = latin_solver_elim(solver, cubepos(0,y,n), o*o
#ifdef STANDALONE_SOLVER
					, "positional elimination,"
					" %s in row %d", names[n-1],
					y+1
#endif
					);
                if (ret != 0) return ret;
            }
    /*
     * Column-wise positional elimination.
     */
    for (x = 0; x < o; x++)
        for (n = 1; n <= o; n++)
            if (!solver->col[x*o+n-1]) {
                ret = latin_solver_elim(solver, cubepos(x,0,n), o
#ifdef STANDALONE_SOLVER
					, "positional elimination,"
					" %s in column %d", names[n-1], x+1
#endif
					);
                if (ret != 0) return ret;
            }

    /*
     * Numeric elimination.
     */
    for (x = 0; x < o; x++)
        for (y = 0; y < o; y++)
            if (!solver->grid[y*o+x]) {
                ret = latin_solver_elim(solver, cubepos(x,y,1), 1
#ifdef STANDALONE_SOLVER
					, "numeric elimination at (%d,%d)",
					x+1, y+1
#endif
					);
                if (ret != 0) return ret;
            }
    return 0;
}