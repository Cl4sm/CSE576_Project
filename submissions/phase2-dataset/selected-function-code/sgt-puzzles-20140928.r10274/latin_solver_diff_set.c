int latin_solver_diff_set(struct latin_solver *solver,
                          struct latin_solver_scratch *scratch,
                          int extreme)
{
    int x, y, n, ret, o = solver->o;
#ifdef STANDALONE_SOLVER
    char **names = solver->names;
#endif

    if (!extreme) {
        /*
         * Row-wise set elimination.
         */
        for (y = 0; y < o; y++) {
            ret = latin_solver_set(solver, scratch, cubepos(0,y,1), o*o, 1
#ifdef STANDALONE_SOLVER
                                   , "set elimination, row %d", y+1
#endif
                                  );
            if (ret != 0) return ret;
        }
        /*
         * Column-wise set elimination.
         */
        for (x = 0; x < o; x++) {
            ret = latin_solver_set(solver, scratch, cubepos(x,0,1), o, 1
#ifdef STANDALONE_SOLVER
                                   , "set elimination, column %d", x+1
#endif
                                  );
            if (ret != 0) return ret;
        }
    } else {
        /*
         * Row-vs-column set elimination on a single number
         * (much tricker for a human to do!)
         */
        for (n = 1; n <= o; n++) {
            ret = latin_solver_set(solver, scratch, cubepos(0,0,n), o*o, o
#ifdef STANDALONE_SOLVER
                                   , "positional set elimination on %s",
				   names[n-1]
#endif
                                  );
            if (ret != 0) return ret;
        }
    }
    return 0;
}