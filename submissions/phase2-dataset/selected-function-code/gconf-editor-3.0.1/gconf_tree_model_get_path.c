static GtkTreePath *
gconf_tree_model_get_path (GtkTreeModel *tree_model, GtkTreeIter *iter)
{
	GConfTreeModel *model = (GConfTreeModel *)tree_model;
	Node *tmp_node;
	GtkTreePath *tree_path;
	gint i = 0;

	tmp_node = iter->user_data;

	if (NODE (iter->user_data)->parent == NULL) {
		tree_path = gtk_tree_path_new ();
		tmp_node = model->root;
	}
	else {
		GtkTreeIter tmp_iter = *iter;

		tmp_iter.user_data = NODE (iter->user_data)->parent;

		tree_path = gconf_tree_model_get_path (tree_model, &tmp_iter);

		tmp_node = NODE (iter->user_data)->parent->children;
	}

	for (; tmp_node; tmp_node = tmp_node->next) {
		if (tmp_node == NODE (iter->user_data))
			break;

		i++;
	}

	gtk_tree_path_append_index (tree_path, i);

	return tree_path;
}