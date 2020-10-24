splay *splay_insert(splay *tree, splay_cmp_t cmp, splay *node) 
{
	int cmpres;
	assert_tree(tree,cmp);
	if (tree == NULL) {
		tree = node;
		node->left = NULL;
		node->right = NULL;
		assert_tree(tree,cmp);
		return tree;
	}
	assert_tree(tree,cmp);
	cmpres=cmp(node,tree);
	if (cmpres<0) {
		tree=splay_insert(tree->left,cmp,node);
	} else if (cmpres>0) {
		tree=splay_insert(tree->right,cmp,node);
	} else {
		/* Replace the root node with the current node */
		node->left = tree->left;
		node->right = tree->right;
		free(tree);
		tree=node;
	}

	assert_tree(tree,cmp);
	return tree;
}