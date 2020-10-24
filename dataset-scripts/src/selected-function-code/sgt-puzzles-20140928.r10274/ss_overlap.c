static struct set **ss_overlap(struct setstore *ss, int x, int y, int mask)
{
    struct set **ret = NULL;
    int nret = 0, retsize = 0;
    int xx, yy;

    for (xx = x-3; xx < x+3; xx++)
	for (yy = y-3; yy < y+3; yy++) {
	    struct set stmp, *s;
	    int pos;

	    /*
	     * Find the first set with these top left coordinates.
	     */
	    stmp.x = xx;
	    stmp.y = yy;
	    stmp.mask = 0;

	    if (findrelpos234(ss->sets, &stmp, NULL, REL234_GE, &pos)) {
		while ((s = index234(ss->sets, pos)) != NULL &&
		       s->x == xx && s->y == yy) {
		    /*
		     * This set potentially overlaps the input one.
		     * Compute the intersection to see if they
		     * really overlap, and add it to the list if
		     * so.
		     */
		    if (setmunge(x, y, mask, s->x, s->y, s->mask, FALSE)) {
			/*
			 * There's an overlap.
			 */
			if (nret >= retsize) {
			    retsize = nret + 32;
			    ret = sresize(ret, retsize, struct set *);
			}
			ret[nret++] = s;
		    }

		    pos++;
		}
	    }
	}

    ret = sresize(ret, nret+1, struct set *);
    ret[nret] = NULL;

    return ret;
}