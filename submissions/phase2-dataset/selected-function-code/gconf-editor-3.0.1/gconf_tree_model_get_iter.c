static gboolean
gconf_tree_model_get_iter (GtkTreeModel *tree_model, GtkTreeIter *iter, GtkTreePath *path)
{
	gint *indices;
	gint depth, i;
	GtkTreeIter parent;
	GConfTreeModel *model;

	model = (GConfTreeModel *)tree_model;

	indices = gtk_tree_path_get_indices (path);
	depth = gtk_tree_path_get_depth (path);

	parent.stamp = model->stamp;
	parent.user_data = NULL;

	if (!gtk_tree_model_iter_nth_child (tree_model, iter, NULL, indices[0]))
		return FALSE;

	for (i = 1; i < depth; i++) {
		parent = *iter;

		if (!gtk_tree_model_iter_nth_child (tree_model, iter, &parent, indices[i]))
			return FALSE;
	}

	return TRUE;
}