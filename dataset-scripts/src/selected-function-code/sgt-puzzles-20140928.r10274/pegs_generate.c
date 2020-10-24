static void pegs_generate(unsigned char *grid, int w, int h, random_state *rs)
{
    while (1) {
	int x, y, extremes;

	memset(grid, GRID_OBST, w*h);
	grid[(h/2) * w + (w/2)] = GRID_PEG;
#ifdef GENERATION_DIAGNOSTICS
	printf("beginning move selection\n");
#endif
	pegs_genmoves(grid, w, h, rs);
#ifdef GENERATION_DIAGNOSTICS
	printf("finished move selection\n");
#endif

	extremes = 0;
	for (y = 0; y < h; y++) {
	    if (grid[y*w+0] != GRID_OBST)
		extremes |= 1;
	    if (grid[y*w+w-1] != GRID_OBST)
		extremes |= 2;
	}
	for (x = 0; x < w; x++) {
	    if (grid[0*w+x] != GRID_OBST)
		extremes |= 4;
	    if (grid[(h-1)*w+x] != GRID_OBST)
		extremes |= 8;
	}

	if (extremes == 15)
	    break;
#ifdef GENERATION_DIAGNOSTICS
	printf("insufficient extent; trying again\n");
#endif
    }
#ifdef GENERATION_DIAGNOSTICS
    fflush(stdout);
#endif
}