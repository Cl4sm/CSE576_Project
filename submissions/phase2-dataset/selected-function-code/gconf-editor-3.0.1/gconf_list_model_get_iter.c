static gboolean
gconf_list_model_get_iter (GtkTreeModel *tree_model, GtkTreeIter *iter, GtkTreePath *path)
{
	GConfListModel *list_model = (GConfListModel *)tree_model;
	GSList *list;
	gint i;

	g_return_val_if_fail (gtk_tree_path_get_depth (path) > 0, FALSE);

	i = gtk_tree_path_get_indices (path)[0];

	if (i >= list_model->length)
		return FALSE;

	list = g_slist_nth (list_model->values, i);

	iter->stamp = list_model->stamp;
	iter->user_data = list;

	return TRUE;
}