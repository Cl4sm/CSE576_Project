static cdg_node_t *get_next_cdg_node(cdg_node_t * root)
{
	cdg_node_t *current = root, *res = NULL;

	while (current) {
		current->visited = 1;
		if (current->status == UNKNOWN) {
			res = current;
			break;
		}
		if (current->left && !current->left->visited) {
			current = current->left;
		} else if (current->right && !current->right->visited) {
			current = current->right;
		} else {
			if (current->left)
				current->left->visited = 0;
			if (current->right)
				current->right->visited = 0;
			if (current->parent == NULL)
				break;
			else
				current = current->parent;
		}
	}

	/* Clean up */
	while (current) {
		current->visited = 0;
		if (current->left)
			current->left->visited = 0;
		if (current->right)
			current->right->visited = 0;
		current = current->parent;
	}

	return res;
}