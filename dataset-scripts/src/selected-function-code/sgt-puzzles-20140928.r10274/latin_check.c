int latin_check(digit *sq, int order)
{
    tree234 *dict = newtree234(latin_check_cmp);
    int c, r;
    int ret = 0;
    lcparams *lcp, lc, *aret;

    /* Use a tree234 as a simple hash table, go through the square
     * adding elements as we go or incrementing their counts. */
    for (c = 0; c < order; c++) {
	for (r = 0; r < order; r++) {
	    lc.elt = ELT(sq, c, r); lc.count = 0;
	    lcp = find234(dict, &lc, NULL);
	    if (!lcp) {
		lcp = snew(lcparams);
		lcp->elt = ELT(sq, c, r);
		lcp->count = 1;
                aret = add234(dict, lcp);
		assert(aret == lcp);
	    } else {
		lcp->count++;
	    }
	}
    }

    /* There should be precisely 'order' letters in the alphabet,
     * each occurring 'order' times (making the OxO tree) */
    if (count234(dict) != order) ret = 1;
    else {
	for (c = 0; (lcp = index234(dict, c)) != NULL; c++) {
	    if (lcp->count != order) ret = 1;
	}
    }
    for (c = 0; (lcp = index234(dict, c)) != NULL; c++)
	sfree(lcp);
    freetree234(dict);

    return ret;
}