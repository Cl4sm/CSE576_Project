static void trans234_subtree_right(node234 *n, int ki, int *k, int *index) {
    node234 *src, *dest;
    int i, srclen, adjust;

    src = n->kids[ki];
    dest = n->kids[ki+1];

    LOG(("  trans234_subtree_right(%p, %d):\n", n, ki));
    LOG(("    parent %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 n,
	 n->kids[0], n->counts[0], n->elems[0],
	 n->kids[1], n->counts[1], n->elems[1],
	 n->kids[2], n->counts[2], n->elems[2],
	 n->kids[3], n->counts[3]));
    LOG(("    src %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 src,
	 src->kids[0], src->counts[0], src->elems[0],
	 src->kids[1], src->counts[1], src->elems[1],
	 src->kids[2], src->counts[2], src->elems[2],
	 src->kids[3], src->counts[3]));
    LOG(("    dest %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 dest,
	 dest->kids[0], dest->counts[0], dest->elems[0],
	 dest->kids[1], dest->counts[1], dest->elems[1],
	 dest->kids[2], dest->counts[2], dest->elems[2],
	 dest->kids[3], dest->counts[3]));
    /*
     * Move over the rest of the destination node to make space.
     */
    dest->kids[3] = dest->kids[2];    dest->counts[3] = dest->counts[2];
    dest->elems[2] = dest->elems[1];
    dest->kids[2] = dest->kids[1];    dest->counts[2] = dest->counts[1];
    dest->elems[1] = dest->elems[0];
    dest->kids[1] = dest->kids[0];    dest->counts[1] = dest->counts[0];

    /* which element to move over */
    i = (src->elems[2] ? 2 : src->elems[1] ? 1 : 0);

    dest->elems[0] = n->elems[ki];
    n->elems[ki] = src->elems[i];
    src->elems[i] = NULL;

    dest->kids[0] = src->kids[i+1];   dest->counts[0] = src->counts[i+1];
    src->kids[i+1] = NULL;            src->counts[i+1] = 0;

    if (dest->kids[0]) dest->kids[0]->parent = dest;

    adjust = dest->counts[0] + 1;

    n->counts[ki] -= adjust;
    n->counts[ki+1] += adjust;

    srclen = n->counts[ki];

    if (k) {
	LOG(("    before: k,index = %d,%d\n", (*k), (*index)));
	if ((*k) == ki && (*index) > srclen) {
	    (*index) -= srclen + 1;
	    (*k)++;
	} else if ((*k) == ki+1) {
	    (*index) += adjust;
	}
	LOG(("    after: k,index = %d,%d\n", (*k), (*index)));
    }

    LOG(("    parent %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 n,
	 n->kids[0], n->counts[0], n->elems[0],
	 n->kids[1], n->counts[1], n->elems[1],
	 n->kids[2], n->counts[2], n->elems[2],
	 n->kids[3], n->counts[3]));
    LOG(("    src %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 src,
	 src->kids[0], src->counts[0], src->elems[0],
	 src->kids[1], src->counts[1], src->elems[1],
	 src->kids[2], src->counts[2], src->elems[2],
	 src->kids[3], src->counts[3]));
    LOG(("    dest %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 dest,
	 dest->kids[0], dest->counts[0], dest->elems[0],
	 dest->kids[1], dest->counts[1], dest->elems[1],
	 dest->kids[2], dest->counts[2], dest->elems[2],
	 dest->kids[3], dest->counts[3]));
}