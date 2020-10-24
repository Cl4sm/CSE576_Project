splay *splay_search_tree(splay *tree, splay_cmp_t cmp, splay *node) {

	if (tree == NULL) {
		return NULL;
	}

	assert_tree(tree,cmp);

	for (;;) {
		int cmpres = cmp(node,tree);

		if (cmpres<0) {
			splay *y;
			if (tree->left == NULL)
				break;
			/* Rotate Right */
			y = tree->left;
			tree->left=y->right;
			y->right=tree;
			tree=y;
			/* Not found? */
			if (cmp(node,tree)>0) {
				break;
			}
		} else if (cmpres>0) {
			splay *y;
			if (tree->right == NULL)
				break;
			/* Rotate Left */
			y = tree->right;
			tree->right=y->left;
			y->left=tree;
			tree=y;
			/* Not found? */
			if (cmp(node,tree)<0) {
				break;
			}
		} else {
			/* Found it */
			break;
		}
	}

	assert_tree(tree,cmp);

	return tree;
}