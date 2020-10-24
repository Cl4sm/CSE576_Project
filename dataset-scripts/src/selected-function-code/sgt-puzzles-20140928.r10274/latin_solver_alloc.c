void latin_solver_alloc(struct latin_solver *solver, digit *grid, int o)
{
    int x, y;

    solver->o = o;
    solver->cube = snewn(o*o*o, unsigned char);
    solver->grid = grid;		/* write straight back to the input */
    memset(solver->cube, TRUE, o*o*o);

    solver->row = snewn(o*o, unsigned char);
    solver->col = snewn(o*o, unsigned char);
    memset(solver->row, FALSE, o*o);
    memset(solver->col, FALSE, o*o);

    for (x = 0; x < o; x++)
	for (y = 0; y < o; y++)
	    if (grid[y*o+x])
		latin_solver_place(solver, x, y, grid[y*o+x]);

#ifdef STANDALONE_SOLVER
    solver->names = NULL;
#endif
}