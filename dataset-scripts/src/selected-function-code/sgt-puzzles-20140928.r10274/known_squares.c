static void known_squares(int w, int h, struct squaretodo *std,
                          signed char *grid,
			  open_cb open, void *openctx,
			  int x, int y, int mask, int mine)
{
    int xx, yy, bit;

    bit = 1;

    for (yy = 0; yy < 3; yy++)
	for (xx = 0; xx < 3; xx++) {
	    if (mask & bit) {
		int i = (y + yy) * w + (x + xx);

		/*
		 * It's possible that this square is _already_
		 * known, in which case we don't try to add it to
		 * the list twice.
		 */
		if (grid[i] == -2) {

		    if (mine) {
			grid[i] = -1;   /* and don't open it! */
		    } else {
			grid[i] = open(openctx, x + xx, y + yy);
			assert(grid[i] != -1);   /* *bang* */
		    }
		    std_add(std, i);

		}
	    }
	    bit <<= 1;
	}
}