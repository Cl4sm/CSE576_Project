static void cdg_dealloc(cdg_node_t ** root)
{
	cdg_node_t *current = *root;

	while (current) {
		if (current->left) {
			current = current->left;
		} else if (current->right) {
			current = current->right;
		} else {
			if (current->parent == NULL) {
				cdg_node_dealloc(current);
				*root = NULL;
				break;
			}
			if (current->parent->left == current) {
				current = current->parent;
				cdg_node_dealloc(current->left);
				current->left = NULL;
			} else if (current->parent->right == current) {
				current = current->parent;
				cdg_node_dealloc(current->right);
				current->right = NULL;
			}
		}
	}
}