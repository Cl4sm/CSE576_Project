static GtkTreePath *
gconf_list_model_get_path (GtkTreeModel *tree_model, GtkTreeIter *iter)
{
	GSList *list;
	GtkTreePath *tree_path;
	gint i = 0;

	g_return_val_if_fail (iter->stamp == GCONF_LIST_MODEL (tree_model)->stamp, NULL);

	for (list = G_SLIST (GCONF_LIST_MODEL (tree_model)->values); list; list = list->next) {
		if (list == G_SLIST (iter->user_data))
			break;
		i++;
	}
	if (list == NULL)
		return NULL;

	tree_path = gtk_tree_path_new ();
	gtk_tree_path_append_index (tree_path, i);

	return tree_path;
}