static void cdg_insert(cdg_node_t ** root, cdg_node_t * new_node)
{
	cdg_node_t *current = *root;

	if (!current) {
		current = new_node;
		*root = current;
		return;
	}

	while (current) {
		if (new_node->channelID < current->channelID) {
			if (current->left) {
				current = current->left;
			} else {
				current->left = new_node;
				new_node->parent = current;
				break;
			}
		} else if (new_node->channelID > current->channelID) {
			if (current->right) {
				current = current->right;
			} else {
				current->right = new_node;
				new_node->parent = current;
				break;
			}
		} else if (new_node->channelID == current->channelID) {
			/* not really possible, maybe programming error */
			break;
		}
	}
}