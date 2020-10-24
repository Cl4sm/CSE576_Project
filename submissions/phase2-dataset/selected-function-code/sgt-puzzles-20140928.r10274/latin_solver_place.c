void latin_solver_place(struct latin_solver *solver, int x, int y, int n)
{
    int i, o = solver->o;

    assert(n <= o);
    assert(cube(x,y,n));

    /*
     * Rule out all other numbers in this square.
     */
    for (i = 1; i <= o; i++)
	if (i != n)
            cube(x,y,i) = FALSE;

    /*
     * Rule out this number in all other positions in the row.
     */
    for (i = 0; i < o; i++)
	if (i != y)
            cube(x,i,n) = FALSE;

    /*
     * Rule out this number in all other positions in the column.
     */
    for (i = 0; i < o; i++)
	if (i != x)
            cube(i,y,n) = FALSE;

    /*
     * Enter the number in the result grid.
     */
    solver->grid[y*o+x] = n;

    /*
     * Cross out this number from the list of numbers left to place
     * in its row, its column and its block.
     */
    solver->row[y*o+n-1] = solver->col[x*o+n-1] = TRUE;
}