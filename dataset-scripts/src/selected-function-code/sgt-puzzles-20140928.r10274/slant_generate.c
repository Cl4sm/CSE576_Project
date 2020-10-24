static void slant_generate(int w, int h, signed char *soln, random_state *rs)
{
    int W = w+1, H = h+1;
    int x, y, i;
    int *connected, *indices;

    /*
     * Clear the output.
     */
    memset(soln, 0, w*h);

    /*
     * Establish a disjoint set forest for tracking connectedness
     * between grid points.
     */
    connected = snew_dsf(W*H);

    /*
     * Prepare a list of the squares in the grid, and fill them in
     * in a random order.
     */
    indices = snewn(w*h, int);
    for (i = 0; i < w*h; i++)
	indices[i] = i;
    shuffle(indices, w*h, sizeof(*indices), rs);

    /*
     * Fill in each one in turn.
     */
    for (i = 0; i < w*h; i++) {
	int fs, bs, v;

	y = indices[i] / w;
	x = indices[i] % w;

	fs = (dsf_canonify(connected, y*W+x) ==
	      dsf_canonify(connected, (y+1)*W+(x+1)));
	bs = (dsf_canonify(connected, (y+1)*W+x) ==
	      dsf_canonify(connected, y*W+(x+1)));

	/*
	 * It isn't possible to get into a situation where we
	 * aren't allowed to place _either_ type of slash in a
	 * square. Thus, filled-grid generation never has to
	 * backtrack.
	 * 
	 * Proof (thanks to Gareth Taylor):
	 * 
	 * If it were possible, it would have to be because there
	 * was an existing path (not using this square) between the
	 * top-left and bottom-right corners of this square, and
	 * another between the other two. These two paths would
	 * have to cross at some point.
	 * 
	 * Obviously they can't cross in the middle of a square, so
	 * they must cross by sharing a point in common. But this
	 * isn't possible either: if you chessboard-colour all the
	 * points on the grid, you find that any continuous
	 * diagonal path is entirely composed of points of the same
	 * colour. And one of our two hypothetical paths is between
	 * two black points, and the other is between two white
	 * points - therefore they can have no point in common. []
	 */
	assert(!(fs && bs));

	v = fs ? +1 : bs ? -1 : 2 * random_upto(rs, 2) - 1;
	fill_square(w, h, x, y, v, soln, connected, NULL);
    }

    sfree(indices);
    sfree(connected);
}