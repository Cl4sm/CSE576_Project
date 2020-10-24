static int add234_insert(node234 *left, void *e, node234 *right,
			 node234 **root, node234 *n, int ki) {
    int lcount, rcount;
    /*
     * We need to insert the new left/element/right set in n at
     * child position ki.
     */
    lcount = countnode234(left);
    rcount = countnode234(right);
    while (n) {
	LOG(("  at %p: %p/%d \"%s\" %p/%d \"%s\" %p/%d \"%s\" %p/%d\n",
	     n,
	     n->kids[0], n->counts[0], n->elems[0],
	     n->kids[1], n->counts[1], n->elems[1],
	     n->kids[2], n->counts[2], n->elems[2],
	     n->kids[3], n->counts[3]));
	LOG(("  need to insert %p/%d \"%s\" %p/%d at position %d\n",
	     left, lcount, e, right, rcount, ki));
	if (n->elems[1] == NULL) {
	    /*
	     * Insert in a 2-node; simple.
	     */
	    if (ki == 0) {
		LOG(("  inserting on left of 2-node\n"));
		n->kids[2] = n->kids[1];     n->counts[2] = n->counts[1];
		n->elems[1] = n->elems[0];
		n->kids[1] = right;          n->counts[1] = rcount;
		n->elems[0] = e;
		n->kids[0] = left;           n->counts[0] = lcount;
	    } else { /* ki == 1 */
		LOG(("  inserting on right of 2-node\n"));
		n->kids[2] = right;          n->counts[2] = rcount;
		n->elems[1] = e;
		n->kids[1] = left;           n->counts[1] = lcount;
	    }
	    if (n->kids[0]) n->kids[0]->parent = n;
	    if (n->kids[1]) n->kids[1]->parent = n;
	    if (n->kids[2]) n->kids[2]->parent = n;
	    LOG(("  done\n"));
	    break;
	} else if (n->elems[2] == NULL) {
	    /*
	     * Insert in a 3-node; simple.
	     */
	    if (ki == 0) {
		LOG(("  inserting on left of 3-node\n"));
		n->kids[3] = n->kids[2];    n->counts[3] = n->counts[2];
		n->elems[2] = n->elems[1];
		n->kids[2] = n->kids[1];    n->counts[2] = n->counts[1];
		n->elems[1] = n->elems[0];
		n->kids[1] = right;         n->counts[1] = rcount;
		n->elems[0] = e;
		n->kids[0] = left;          n->counts[0] = lcount;
	    } else if (ki == 1) {
		LOG(("  inserting in middle of 3-node\n"));
		n->kids[3] = n->kids[2];    n->counts[3] = n->counts[2];
		n->elems[2] = n->elems[1];
		n->kids[2] = right;         n->counts[2] = rcount;
		n->elems[1] = e;
		n->kids[1] = left;          n->counts[1] = lcount;
	    } else { /* ki == 2 */
		LOG(("  inserting on right of 3-node\n"));
		n->kids[3] = right;         n->counts[3] = rcount;
		n->elems[2] = e;
		n->kids[2] = left;          n->counts[2] = lcount;
	    }
	    if (n->kids[0]) n->kids[0]->parent = n;
	    if (n->kids[1]) n->kids[1]->parent = n;
	    if (n->kids[2]) n->kids[2]->parent = n;
	    if (n->kids[3]) n->kids[3]->parent = n;
	    LOG(("  done\n"));
	    break;
	} else {
	    node234 *m = snew(node234);
	    m->parent = n->parent;
	    LOG(("  splitting a 4-node; created new node %p\n", m));
	    /*
	     * Insert in a 4-node; split into a 2-node and a
	     * 3-node, and move focus up a level.
	     * 
	     * I don't think it matters which way round we put the
	     * 2 and the 3. For simplicity, we'll put the 3 first
	     * always.
	     */
	    if (ki == 0) {
		m->kids[0] = left;          m->counts[0] = lcount;
		m->elems[0] = e;
		m->kids[1] = right;         m->counts[1] = rcount;
		m->elems[1] = n->elems[0];
		m->kids[2] = n->kids[1];    m->counts[2] = n->counts[1];
		e = n->elems[1];
		n->kids[0] = n->kids[2];    n->counts[0] = n->counts[2];
		n->elems[0] = n->elems[2];
		n->kids[1] = n->kids[3];    n->counts[1] = n->counts[3];
	    } else if (ki == 1) {
		m->kids[0] = n->kids[0];    m->counts[0] = n->counts[0];
		m->elems[0] = n->elems[0];
		m->kids[1] = left;          m->counts[1] = lcount;
		m->elems[1] = e;
		m->kids[2] = right;         m->counts[2] = rcount;
		e = n->elems[1];
		n->kids[0] = n->kids[2];    n->counts[0] = n->counts[2];
		n->elems[0] = n->elems[2];
		n->kids[1] = n->kids[3];    n->counts[1] = n->counts[3];
	    } else if (ki == 2) {
		m->kids[0] = n->kids[0];    m->counts[0] = n->counts[0];
		m->elems[0] = n->elems[0];
		m->kids[1] = n->kids[1];    m->counts[1] = n->counts[1];
		m->elems[1] = n->elems[1];
		m->kids[2] = left;          m->counts[2] = lcount;
		/* e = e; */
		n->kids[0] = right;         n->counts[0] = rcount;
		n->elems[0] = n->elems[2];
		n->kids[1] = n->kids[3];    n->counts[1] = n->counts[3];
	    } else { /* ki == 3 */
		m->kids[0] = n->kids[0];    m->counts[0] = n->counts[0];
		m->elems[0] = n->elems[0];
		m->kids[1] = n->kids[1];    m->counts[1] = n->counts[1];
		m->elems[1] = n->elems[1];
		m->kids[2] = n->kids[2];    m->counts[2] = n->counts[2];
		n->kids[0] = left;          n->counts[0] = lcount;
		n->elems[0] = e;
		n->kids[1] = right;         n->counts[1] = rcount;
		e = n->elems[2];
	    }
	    m->kids[3] = n->kids[3] = n->kids[2] = NULL;
	    m->counts[3] = n->counts[3] = n->counts[2] = 0;
	    m->elems[2] = n->elems[2] = n->elems[1] = NULL;
	    if (m->kids[0]) m->kids[0]->parent = m;
	    if (m->kids[1]) m->kids[1]->parent = m;
	    if (m->kids[2]) m->kids[2]->parent = m;
	    if (n->kids[0]) n->kids[0]->parent = n;
	    if (n->kids[1]) n->kids[1]->parent = n;
	    LOG(("  left (%p): %p/%d \"%s\" %p/%d \"%s\" %p/%d\n", m,
		 m->kids[0], m->counts[0], m->elems[0],
		 m->kids[1], m->counts[1], m->elems[1],
		 m->kids[2], m->counts[2]));
	    LOG(("  right (%p): %p/%d \"%s\" %p/%d\n", n,
		 n->kids[0], n->counts[0], n->elems[0],
		 n->kids[1], n->counts[1]));
	    left = m;  lcount = countnode234(left);
	    right = n; rcount = countnode234(right);
	}
	if (n->parent)
	    ki = (n->parent->kids[0] == n ? 0 :
		  n->parent->kids[1] == n ? 1 :
		  n->parent->kids[2] == n ? 2 : 3);
	n = n->parent;
    }

    /*
     * If we've come out of here by `break', n will still be
     * non-NULL and all we need to do is go back up the tree
     * updating counts. If we've come here because n is NULL, we
     * need to create a new root for the tree because the old one
     * has just split into two. */
    if (n) {
	while (n->parent) {
	    int count = countnode234(n);
	    int childnum;
	    childnum = (n->parent->kids[0] == n ? 0 :
			n->parent->kids[1] == n ? 1 :
			n->parent->kids[2] == n ? 2 : 3);
	    n->parent->counts[childnum] = count;
	    n = n->parent;
	}
	return 0;		       /* root unchanged */
    } else {
	LOG(("  root is overloaded, split into two\n"));
	(*root) = snew(node234);
	(*root)->kids[0] = left;     (*root)->counts[0] = lcount;
	(*root)->elems[0] = e;
	(*root)->kids[1] = right;    (*root)->counts[1] = rcount;
	(*root)->elems[1] = NULL;
	(*root)->kids[2] = NULL;     (*root)->counts[2] = 0;
	(*root)->elems[2] = NULL;
	(*root)->kids[3] = NULL;     (*root)->counts[3] = 0;
	(*root)->parent = NULL;
	if ((*root)->kids[0]) (*root)->kids[0]->parent = (*root);
	if ((*root)->kids[1]) (*root)->kids[1]->parent = (*root);
	LOG(("  new root is %p/%d \"%s\" %p/%d\n",
	     (*root)->kids[0], (*root)->counts[0],
	     (*root)->elems[0],
	     (*root)->kids[1], (*root)->counts[1]));
	return 1;		       /* root moved */
    }
}