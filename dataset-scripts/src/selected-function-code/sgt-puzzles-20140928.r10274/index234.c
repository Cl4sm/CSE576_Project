void *index234(tree234 *t, int index) {
    node234 *n;

    if (!t->root)
	return NULL;		       /* tree is empty */

    if (index < 0 || index >= countnode234(t->root))
	return NULL;		       /* out of range */

    n = t->root;
    
    while (n) {
	if (index < n->counts[0])
	    n = n->kids[0];
	else if (index -= n->counts[0] + 1, index < 0)
	    return n->elems[0];
	else if (index < n->counts[1])
	    n = n->kids[1];
	else if (index -= n->counts[1] + 1, index < 0)
	    return n->elems[1];
	else if (index < n->counts[2])
	    n = n->kids[2];
	else if (index -= n->counts[2] + 1, index < 0)
	    return n->elems[2];
	else
	    n = n->kids[3];
    }

    /* We shouldn't ever get here. I wonder how we did. */
    return NULL;
}