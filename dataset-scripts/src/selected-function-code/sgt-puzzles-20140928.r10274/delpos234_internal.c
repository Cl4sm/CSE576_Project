static void *delpos234_internal(tree234 *t, int index) {
    node234 *n;
    void *retval;
    int ki, i;

    retval = NULL;

    n = t->root;		       /* by assumption this is non-NULL */
    LOG(("deleting item %d from tree %p\n", index, t));
    while (1) {
	node234 *sub;

	LOG(("  node %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d index=%d\n",
	     n,
	     n->kids[0], n->counts[0], n->elems[0],
	     n->kids[1], n->counts[1], n->elems[1],
	     n->kids[2], n->counts[2], n->elems[2],
	     n->kids[3], n->counts[3],
	     index));
	if (index <= n->counts[0]) {
	    ki = 0;
	} else if (index -= n->counts[0]+1, index <= n->counts[1]) {
	    ki = 1;
	} else if (index -= n->counts[1]+1, index <= n->counts[2]) {
	    ki = 2;
	} else if (index -= n->counts[2]+1, index <= n->counts[3]) {
	    ki = 3;
	} else {
	    assert(0);		       /* can't happen */
	}

	if (!n->kids[0])
	    break;		       /* n is a leaf node; we're here! */

	/*
	 * Check to see if we've found our target element. If so,
	 * we must choose a new target (we'll use the old target's
	 * successor, which will be in a leaf), move it into the
	 * place of the old one, continue down to the leaf and
	 * delete the old copy of the new target.
	 */
	if (index == n->counts[ki]) {
	    node234 *m;
	    LOG(("  found element in internal node, index %d\n", ki));
	    assert(n->elems[ki]);      /* must be a kid _before_ an element */
	    ki++; index = 0;
	    for (m = n->kids[ki]; m->kids[0]; m = m->kids[0])
		continue;
	    LOG(("  replacing with element \"%s\" from leaf node %p\n",
		 m->elems[0], m));
	    retval = n->elems[ki-1];
	    n->elems[ki-1] = m->elems[0];
	}

	/*
	 * Recurse down to subtree ki. If it has only one element,
	 * we have to do some transformation to start with.
	 */
	LOG(("  moving to subtree %d\n", ki));
	sub = n->kids[ki];
	if (!sub->elems[1]) {
	    LOG(("  subtree has only one element!\n"));
	    if (ki > 0 && n->kids[ki-1]->elems[1]) {
		/*
		 * Child ki has only one element, but child
		 * ki-1 has two or more. So we need to move a
		 * subtree from ki-1 to ki.
		 */
		trans234_subtree_right(n, ki-1, &ki, &index);
	    } else if (ki < 3 && n->kids[ki+1] &&
		       n->kids[ki+1]->elems[1]) {
		/*
		 * Child ki has only one element, but ki+1 has
		 * two or more. Move a subtree from ki+1 to ki.
		 */
		trans234_subtree_left(n, ki+1, &ki, &index);
	    } else {
		/*
		 * ki is small with only small neighbours. Pick a
		 * neighbour and merge with it.
		 */
		trans234_subtree_merge(n, ki>0 ? ki-1 : ki, &ki, &index);
		sub = n->kids[ki];

		if (!n->elems[0]) {
		    /*
		     * The root is empty and needs to be
		     * removed.
		     */
		    LOG(("  shifting root!\n"));
		    t->root = sub;
		    sub->parent = NULL;
		    sfree(n);
		    n = NULL;
		}
	    }
	}

	if (n)
	    n->counts[ki]--;
	n = sub;
    }

    /*
     * Now n is a leaf node, and ki marks the element number we
     * want to delete. We've already arranged for the leaf to be
     * bigger than minimum size, so let's just go to it.
     */
    assert(!n->kids[0]);
    if (!retval)
	retval = n->elems[ki];

    for (i = ki; i < 2 && n->elems[i+1]; i++)
	n->elems[i] = n->elems[i+1];
    n->elems[i] = NULL;

    /*
     * It's just possible that we have reduced the leaf to zero
     * size. This can only happen if it was the root - so destroy
     * it and make the tree empty.
     */
    if (!n->elems[0]) {
	LOG(("  removed last element in tree, destroying empty root\n"));
	assert(n == t->root);
	sfree(n);
	t->root = NULL;
    }

    return retval;		       /* finished! */
}