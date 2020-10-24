void *findrelpos234(tree234 *t, void *e, cmpfn234 cmp,
		    int relation, int *index) {
    node234 *n;
    void *ret;
    int c;
    int idx, ecount, kcount, cmpret;

    if (t->root == NULL)
	return NULL;

    if (cmp == NULL)
	cmp = t->cmp;

    n = t->root;
    /*
     * Attempt to find the element itself.
     */
    idx = 0;
    ecount = -1;
    /*
     * Prepare a fake `cmp' result if e is NULL.
     */
    cmpret = 0;
    if (e == NULL) {
	assert(relation == REL234_LT || relation == REL234_GT);
	if (relation == REL234_LT)
	    cmpret = +1;	       /* e is a max: always greater */
	else if (relation == REL234_GT)
	    cmpret = -1;	       /* e is a min: always smaller */
    }
    while (1) {
	for (kcount = 0; kcount < 4; kcount++) {
	    if (kcount >= 3 || n->elems[kcount] == NULL ||
		(c = cmpret ? cmpret : cmp(e, n->elems[kcount])) < 0) {
		break;
	    }
	    if (n->kids[kcount]) idx += n->counts[kcount];
	    if (c == 0) {
		ecount = kcount;
		break;
	    }
	    idx++;
	}
	if (ecount >= 0)
	    break;
	if (n->kids[kcount])
	    n = n->kids[kcount];
	else
	    break;
    }

    if (ecount >= 0) {
	/*
	 * We have found the element we're looking for. It's
	 * n->elems[ecount], at tree index idx. If our search
	 * relation is EQ, LE or GE we can now go home.
	 */
	if (relation != REL234_LT && relation != REL234_GT) {
	    if (index) *index = idx;
	    return n->elems[ecount];
	}

	/*
	 * Otherwise, we'll do an indexed lookup for the previous
	 * or next element. (It would be perfectly possible to
	 * implement these search types in a non-counted tree by
	 * going back up from where we are, but far more fiddly.)
	 */
	if (relation == REL234_LT)
	    idx--;
	else
	    idx++;
    } else {
	/*
	 * We've found our way to the bottom of the tree and we
	 * know where we would insert this node if we wanted to:
	 * we'd put it in in place of the (empty) subtree
	 * n->kids[kcount], and it would have index idx
	 * 
	 * But the actual element isn't there. So if our search
	 * relation is EQ, we're doomed.
	 */
	if (relation == REL234_EQ)
	    return NULL;

	/*
	 * Otherwise, we must do an index lookup for index idx-1
	 * (if we're going left - LE or LT) or index idx (if we're
	 * going right - GE or GT).
	 */
	if (relation == REL234_LT || relation == REL234_LE) {
	    idx--;
	}
    }

    /*
     * We know the index of the element we want; just call index234
     * to do the rest. This will return NULL if the index is out of
     * bounds, which is exactly what we want.
     */
    ret = index234(t, idx);
    if (ret && index) *index = idx;
    return ret;
}