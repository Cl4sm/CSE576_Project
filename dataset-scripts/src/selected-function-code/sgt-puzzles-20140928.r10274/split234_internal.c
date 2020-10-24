static node234 *split234_internal(tree234 *t, int index) {
    node234 *halves[2] = { NULL, NULL }, *n, *sib, *sub;
    node234 *lparent, *rparent;
    int ki, pki, i, half, lcount, rcount;

    n = t->root;
    LOG(("splitting tree %p at point %d\n", t, index));

    /*
     * Easy special cases. After this we have also dealt completely
     * with the empty-tree case and we can assume the root exists.
     */
    if (index == 0)		       /* return nothing */
	return NULL;
    if (index == countnode234(t->root)) {   /* return the whole tree */
	node234 *ret = t->root;
	t->root = NULL;
	return ret;
    }

    /*
     * Search down the tree to find the split point.
     */
    halves[0] = halves[1] = NULL;
    lparent = rparent = NULL;
    pki = -1;
    while (n) {
	LOG(("  node %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d index=%d\n",
	     n,
	     n->kids[0], n->counts[0], n->elems[0],
	     n->kids[1], n->counts[1], n->elems[1],
	     n->kids[2], n->counts[2], n->elems[2],
	     n->kids[3], n->counts[3],
	     index));
	lcount = index;
	rcount = countnode234(n) - lcount;
	if (index <= n->counts[0]) {
	    ki = 0;
	} else if (index -= n->counts[0]+1, index <= n->counts[1]) {
	    ki = 1;
	} else if (index -= n->counts[1]+1, index <= n->counts[2]) {
	    ki = 2;
	} else {
	    index -= n->counts[2]+1;
	    ki = 3;
	}

	LOG(("  splitting at subtree %d\n", ki));
	sub = n->kids[ki];

	LOG(("  splitting at child index %d\n", ki));

	/*
	 * Split the node, put halves[0] on the right of the left
	 * one and halves[1] on the left of the right one, put the
	 * new node pointers in halves[0] and halves[1], and go up
	 * a level.
	 */
	sib = snew(node234);
	for (i = 0; i < 3; i++) {
	    if (i+ki < 3 && n->elems[i+ki]) {
		sib->elems[i] = n->elems[i+ki];
		sib->kids[i+1] = n->kids[i+ki+1];
		if (sib->kids[i+1]) sib->kids[i+1]->parent = sib;
		sib->counts[i+1] = n->counts[i+ki+1];
		n->elems[i+ki] = NULL;
		n->kids[i+ki+1] = NULL;
		n->counts[i+ki+1] = 0;
	    } else {
		sib->elems[i] = NULL;
		sib->kids[i+1] = NULL;
		sib->counts[i+1] = 0;
	    }
	}
	if (lparent) {
	    lparent->kids[pki] = n;
	    lparent->counts[pki] = lcount;
	    n->parent = lparent;
	    rparent->kids[0] = sib;
	    rparent->counts[0] = rcount;
	    sib->parent = rparent;
	} else {
	    halves[0] = n;
	    n->parent = NULL;
	    halves[1] = sib;
	    sib->parent = NULL;
	}
	lparent = n;
	rparent = sib;
	pki = ki;
	LOG(("  left node %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	     n,
	     n->kids[0], n->counts[0], n->elems[0],
	     n->kids[1], n->counts[1], n->elems[1],
	     n->kids[2], n->counts[2], n->elems[2],
	     n->kids[3], n->counts[3]));
	LOG(("  right node %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	     sib,
	     sib->kids[0], sib->counts[0], sib->elems[0],
	     sib->kids[1], sib->counts[1], sib->elems[1],
	     sib->kids[2], sib->counts[2], sib->elems[2],
	     sib->kids[3], sib->counts[3]));

	n = sub;
    }

    /*
     * We've come off the bottom here, so we've successfully split
     * the tree into two equally high subtrees. The only problem is
     * that some of the nodes down the fault line will be smaller
     * than the minimum permitted size. (Since this is a 2-3-4
     * tree, that means they'll be zero-element one-child nodes.)
     */
    LOG(("  fell off bottom, lroot is %p, rroot is %p\n",
	 halves[0], halves[1]));
    assert(halves[0] != NULL);
    assert(halves[1] != NULL);
    lparent->counts[pki] = rparent->counts[0] = 0;
    lparent->kids[pki] = rparent->kids[0] = NULL;

    /*
     * So now we go back down the tree from each of the two roots,
     * fixing up undersize nodes.
     */
    for (half = 0; half < 2; half++) {
	/*
	 * Remove the root if it's undersize (it will contain only
	 * one child pointer, so just throw it away and replace it
	 * with its child). This might happen several times.
	 */
	while (halves[half] && !halves[half]->elems[0]) {
	    LOG(("  root %p is undersize, throwing away\n", halves[half]));
	    halves[half] = halves[half]->kids[0];
	    sfree(halves[half]->parent);
	    halves[half]->parent = NULL;
	    LOG(("  new root is %p\n", halves[half]));
	}

	n = halves[half];
	while (n) {
	    void (*toward)(node234 *n, int ki, int *k, int *index);
	    int ni, merge;

	    /*
	     * Now we have a potentially undersize node on the
	     * right (if half==0) or left (if half==1). Sort it
	     * out, by merging with a neighbour or by transferring
	     * subtrees over. At this time we must also ensure that
	     * nodes are bigger than minimum, in case we need an
	     * element to merge two nodes below.
	     */
	    LOG(("  node %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
		 n,
		 n->kids[0], n->counts[0], n->elems[0],
		 n->kids[1], n->counts[1], n->elems[1],
		 n->kids[2], n->counts[2], n->elems[2],
		 n->kids[3], n->counts[3]));
	    if (half == 1) {
		ki = 0;		       /* the kid we're interested in */
		ni = 1;		       /* the neighbour */
		merge = 0;	       /* for merge: leftmost of the two */
		toward = trans234_subtree_left;
	    } else {
		ki = (n->kids[3] ? 3 : n->kids[2] ? 2 : 1);
		ni = ki-1;
		merge = ni;
		toward = trans234_subtree_right;
	    }

	    sub = n->kids[ki];
	    if (sub && !sub->elems[1]) {
		/*
		 * This node is undersized or minimum-size. If we
		 * can merge it with its neighbour, we do so;
		 * otherwise we must be able to transfer subtrees
		 * over to it until it is greater than minimum
		 * size.
		 */
		int undersized = (!sub->elems[0]);
		LOG(("  child %d is %ssize\n", ki,
		     undersized ? "under" : "minimum-"));
		LOG(("  neighbour is %s\n",
		     n->kids[ni]->elems[2] ? "large" :
		     n->kids[ni]->elems[1] ? "medium" : "small"));
		if (!n->kids[ni]->elems[1] ||
		    (undersized && !n->kids[ni]->elems[2])) {
		    /*
		     * Neighbour is small, or possibly neighbour is
		     * medium and we are undersize.
		     */
		    trans234_subtree_merge(n, merge, NULL, NULL);
		    sub = n->kids[merge];
		    if (!n->elems[0]) {
			/*
			 * n is empty, and hence must have been the
			 * root and needs to be removed.
			 */
			assert(!n->parent);
			LOG(("  shifting root!\n"));
			halves[half] = sub;
			halves[half]->parent = NULL;
			sfree(n);
		    }
		} else {
		    /* Neighbour is big enough to move trees over. */
		    toward(n, ni, NULL, NULL);
		    if (undersized)
			toward(n, ni, NULL, NULL);
		}
	    }
	    n = sub;
	}
    }

    t->root = halves[1];
    return halves[0];
}