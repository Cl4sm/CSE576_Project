static gboolean
gconf_tree_model_iter_nth_child (GtkTreeModel *tree_model, GtkTreeIter *iter, GtkTreeIter *parent, gint n)
{
	GConfTreeModel *model;
	Node *node;
	Node *parent_node = NULL;

	model = (GConfTreeModel *)tree_model;

	g_return_val_if_fail (parent == NULL || parent->user_data != NULL, FALSE);
	g_return_val_if_fail (parent == NULL || parent->stamp == model->stamp, FALSE);

	if (parent == NULL)
		node = model->root;
	else {
		parent_node = (Node *)parent->user_data;
		node = parent_node->children;
	}

	if (!node && parent && gconf_tree_model_build_level (model, parent_node, FALSE)) {
		node = parent_node->children;
	}

	for (; node != NULL; node = node->next)
		if (node->offset == n) {
			iter->stamp = model->stamp;
			iter->user_data = node;

			return TRUE;
		}

	iter->stamp = 0;

	return FALSE;
}