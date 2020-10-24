static node234 *join234_internal(node234 *left, void *sep,
				 node234 *right, int *height) {
    node234 *root, *node;
    int relht = *height;
    int ki;

    LOG(("  join: joining %p \"%s\" %p, relative height is %d\n",
	 left, sep, right, relht));
    if (relht == 0) {
	/*
	 * The trees are the same height. Create a new one-element
	 * root containing the separator and pointers to the two
	 * nodes.
	 */
	node234 *newroot;
	newroot = snew(node234);
	newroot->kids[0] = left;     newroot->counts[0] = countnode234(left);
	newroot->elems[0] = sep;
	newroot->kids[1] = right;    newroot->counts[1] = countnode234(right);
	newroot->elems[1] = NULL;
	newroot->kids[2] = NULL;     newroot->counts[2] = 0;
	newroot->elems[2] = NULL;
	newroot->kids[3] = NULL;     newroot->counts[3] = 0;
	newroot->parent = NULL;
	if (left) left->parent = newroot;
	if (right) right->parent = newroot;
	*height = 1;
	LOG(("  join: same height, brand new root\n"));
	return newroot;
    }

    /*
     * This now works like the addition algorithm on the larger
     * tree. We're replacing a single kid pointer with two kid
     * pointers separated by an element; if that causes the node to
     * overload, we split it in two, move a separator element up to
     * the next node, and repeat.
     */
    if (relht < 0) {
	/*
	 * Left tree is shorter. Search down the right tree to find
	 * the pointer we're inserting at.
	 */
	node = root = right;
	while (++relht < 0) {
	    node = node->kids[0];
	}
	ki = 0;
	right = node->kids[ki];
    } else {
	/*
	 * Right tree is shorter; search down the left to find the
	 * pointer we're inserting at.
	 */
	node = root = left;
	while (--relht > 0) {
	    if (node->elems[2])
		node = node->kids[3];
	    else if (node->elems[1])
		node = node->kids[2];
	    else
		node = node->kids[1];
	}
	if (node->elems[2])
	    ki = 3;
	else if (node->elems[1])
	    ki = 2;
	else
	    ki = 1;
	left = node->kids[ki];
    }

    /*
     * Now proceed as for addition.
     */
    *height = add234_insert(left, sep, right, &root, node, ki);

    return root;
}