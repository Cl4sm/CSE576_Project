static void *add234_internal(tree234 *t, void *e, int index) {
    node234 *n;
    int ki;
    void *orig_e = e;
    int c;

    LOG(("adding element \"%s\" to tree %p\n", e, t));
    if (t->root == NULL) {
	t->root = snew(node234);
	t->root->elems[1] = t->root->elems[2] = NULL;
	t->root->kids[0] = t->root->kids[1] = NULL;
	t->root->kids[2] = t->root->kids[3] = NULL;
	t->root->counts[0] = t->root->counts[1] = 0;
	t->root->counts[2] = t->root->counts[3] = 0;
	t->root->parent = NULL;
	t->root->elems[0] = e;
	LOG(("  created root %p\n", t->root));
	return orig_e;
    }

    n = t->root;
    while (n) {
	LOG(("  node %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	     n,
	     n->kids[0], n->counts[0], n->elems[0],
	     n->kids[1], n->counts[1], n->elems[1],
	     n->kids[2], n->counts[2], n->elems[2],
	     n->kids[3], n->counts[3]));
	if (index >= 0) {
	    if (!n->kids[0]) {
		/*
		 * Leaf node. We want to insert at kid position
		 * equal to the index:
		 * 
		 *   0 A 1 B 2 C 3
		 */
		ki = index;
	    } else {
		/*
		 * Internal node. We always descend through it (add
		 * always starts at the bottom, never in the
		 * middle).
		 */
		if (index <= n->counts[0]) {
		    ki = 0;
		} else if (index -= n->counts[0] + 1, index <= n->counts[1]) {
		    ki = 1;
		} else if (index -= n->counts[1] + 1, index <= n->counts[2]) {
		    ki = 2;
		} else if (index -= n->counts[2] + 1, index <= n->counts[3]) {
		    ki = 3;
		} else
		    return NULL;       /* error: index out of range */
	    }
	} else {
	    if ((c = t->cmp(e, n->elems[0])) < 0)
		ki = 0;
	    else if (c == 0)
		return n->elems[0];	       /* already exists */
	    else if (n->elems[1] == NULL || (c = t->cmp(e, n->elems[1])) < 0)
		ki = 1;
	    else if (c == 0)
		return n->elems[1];	       /* already exists */
	    else if (n->elems[2] == NULL || (c = t->cmp(e, n->elems[2])) < 0)
		ki = 2;
	    else if (c == 0)
		return n->elems[2];	       /* already exists */
	    else
		ki = 3;
	}
	LOG(("  moving to child %d (%p)\n", ki, n->kids[ki]));
	if (!n->kids[ki])
	    break;
	n = n->kids[ki];
    }

    add234_insert(NULL, e, NULL, &t->root, n, ki);

    return orig_e;
}