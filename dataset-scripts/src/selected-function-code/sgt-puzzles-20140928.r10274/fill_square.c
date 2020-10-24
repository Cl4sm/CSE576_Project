static void fill_square(int w, int h, int x, int y, int v,
			signed char *soln,
			int *connected, struct solver_scratch *sc)
{
    int W = w+1 /*, H = h+1 */;

    assert(x >= 0 && x < w && y >= 0 && y < h);

    if (soln[y*w+x] != 0) {
	return;			       /* do nothing */
    }

#ifdef SOLVER_DIAGNOSTICS
    if (verbose)
	printf("  placing %c in %d,%d\n", v == -1 ? '\\' : '/', x, y);
#endif

    soln[y*w+x] = v;

    if (sc) {
	int c = dsf_canonify(sc->equiv, y*w+x);
	sc->slashval[c] = v;
    }

    if (v < 0) {
	merge_vertices(connected, sc, y*W+x, (y+1)*W+(x+1));
	if (sc) {
	    decr_exits(sc, y*W+(x+1));
	    decr_exits(sc, (y+1)*W+x);
	}
    } else {
	merge_vertices(connected, sc, y*W+(x+1), (y+1)*W+x);
	if (sc) {
	    decr_exits(sc, y*W+x);
	    decr_exits(sc, (y+1)*W+(x+1));
	}
    }
}