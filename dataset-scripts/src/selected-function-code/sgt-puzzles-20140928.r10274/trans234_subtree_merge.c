static void trans234_subtree_merge(node234 *n, int ki, int *k, int *index) {
    node234 *left, *right;
    int i, leftlen, rightlen, lsize, rsize;

    left = n->kids[ki];               leftlen = n->counts[ki];
    right = n->kids[ki+1];            rightlen = n->counts[ki+1];

    LOG(("  trans234_subtree_merge(%p, %d):\n", n, ki));
    LOG(("    parent %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 n,
	 n->kids[0], n->counts[0], n->elems[0],
	 n->kids[1], n->counts[1], n->elems[1],
	 n->kids[2], n->counts[2], n->elems[2],
	 n->kids[3], n->counts[3]));
    LOG(("    left %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 left,
	 left->kids[0], left->counts[0], left->elems[0],
	 left->kids[1], left->counts[1], left->elems[1],
	 left->kids[2], left->counts[2], left->elems[2],
	 left->kids[3], left->counts[3]));
    LOG(("    right %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 right,
	 right->kids[0], right->counts[0], right->elems[0],
	 right->kids[1], right->counts[1], right->elems[1],
	 right->kids[2], right->counts[2], right->elems[2],
	 right->kids[3], right->counts[3]));

    assert(!left->elems[2] && !right->elems[2]);   /* neither is large! */
    lsize = (left->elems[1] ? 2 : left->elems[0] ? 1 : 0);
    rsize = (right->elems[1] ? 2 : right->elems[0] ? 1 : 0);

    left->elems[lsize] = n->elems[ki];

    for (i = 0; i < rsize+1; i++) {
	left->kids[lsize+1+i] = right->kids[i];
	left->counts[lsize+1+i] = right->counts[i];
	if (left->kids[lsize+1+i])
	    left->kids[lsize+1+i]->parent = left;
	if (i < rsize)
	    left->elems[lsize+1+i] = right->elems[i];
    }

    n->counts[ki] += rightlen + 1;

    sfree(right);

    /*
     * Move the rest of n up by one.
     */
    for (i = ki+1; i < 3; i++) {
	n->kids[i] = n->kids[i+1];
	n->counts[i] = n->counts[i+1];
    }
    for (i = ki; i < 2; i++) {
	n->elems[i] = n->elems[i+1];
    }
    n->kids[3] = NULL;
    n->counts[3] = 0;
    n->elems[2] = NULL;

    if (k) {
	LOG(("    before: k,index = %d,%d\n", (*k), (*index)));
	if ((*k) == ki+1) {
	    (*k)--;
	    (*index) += leftlen + 1;
	} else if ((*k) > ki+1) {
	    (*k)--;
	}
	LOG(("    after: k,index = %d,%d\n", (*k), (*index)));
    }

    LOG(("    parent %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 n,
	 n->kids[0], n->counts[0], n->elems[0],
	 n->kids[1], n->counts[1], n->elems[1],
	 n->kids[2], n->counts[2], n->elems[2],
	 n->kids[3], n->counts[3]));
    LOG(("    merged %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	 left,
	 left->kids[0], left->counts[0], left->elems[0],
	 left->kids[1], left->counts[1], left->elems[1],
	 left->kids[2], left->counts[2], left->elems[2],
	 left->kids[3], left->counts[3]));

}