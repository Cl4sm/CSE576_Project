static gboolean
gconf_tree_model_iter_children (GtkTreeModel *tree_model, GtkTreeIter *iter, GtkTreeIter *parent)
{
	GConfTreeModel *model = (GConfTreeModel *)tree_model;
	Node *parent_node = parent->user_data;

	if (parent_node->children != NULL) {
		iter->stamp = model->stamp;
		iter->user_data = parent_node->children;

		return TRUE;
	}

	if (!gconf_tree_model_build_level (model, parent_node, TRUE)) {
		iter->stamp = 0;
		iter->user_data = NULL;

		return FALSE;
	}

	iter->stamp = model->stamp;
	iter->user_data = parent_node->children;

	return TRUE;
}