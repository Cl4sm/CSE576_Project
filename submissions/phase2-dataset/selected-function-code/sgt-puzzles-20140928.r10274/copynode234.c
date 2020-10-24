static node234 *copynode234(node234 *n, copyfn234 copyfn, void *copyfnstate) {
    int i;
    node234 *n2 = snew(node234);

    for (i = 0; i < 3; i++) {
	if (n->elems[i] && copyfn)
	    n2->elems[i] = copyfn(copyfnstate, n->elems[i]);
	else
	    n2->elems[i] = n->elems[i];
    }

    for (i = 0; i < 4; i++) {
	if (n->kids[i]) {
	    n2->kids[i] = copynode234(n->kids[i], copyfn, copyfnstate);
	    n2->kids[i]->parent = n2;
	} else {
	    n2->kids[i] = NULL;
	}
	n2->counts[i] = n->counts[i];
    }

    return n2;
}