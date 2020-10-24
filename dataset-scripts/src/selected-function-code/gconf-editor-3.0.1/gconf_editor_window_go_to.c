void
gconf_editor_window_go_to (GConfEditorWindow *window,
			   const char        *location)
{
	GtkTreePath *path, *child_path;
	gint depth;
	gint i, nchildren;
	char *key;
	
	g_return_if_fail (location != NULL);
	
	child_path = gconf_tree_model_get_tree_path_from_gconf_path (
				GCONF_TREE_MODEL (window->tree_model), location);
	if (!child_path)
		return;

	path = gtk_tree_model_sort_convert_child_path_to_path (
				GTK_TREE_MODEL_SORT (window->sorted_tree_model),
				child_path);

	gtk_tree_path_free (child_path);
	
	/* kind of hackish, but it works! */
	depth = gtk_tree_path_get_depth (path);
	for (i = 0; i < depth; i++) {
		gint j;
		GtkTreePath *cpath = gtk_tree_path_copy (path);

		for (j = 0; j < (depth - i); j++)
			gtk_tree_path_up (cpath);

		gtk_tree_view_expand_row (GTK_TREE_VIEW (window->tree_view), cpath, FALSE);
		gtk_tree_path_free (cpath);
	}

	gtk_tree_view_expand_row (GTK_TREE_VIEW (window->tree_view), path, FALSE);

	gtk_tree_selection_select_path (
			gtk_tree_view_get_selection (GTK_TREE_VIEW (window->tree_view)),
			path);
	gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (window->tree_view),
				      path, NULL, TRUE, 0.5, 0.5);
	gtk_tree_path_free (path);

	/* check for key name in the list model */
	key = strrchr (location, '/');
	if (!key || strlen (key) < 2)
		return;
	key++;
	nchildren = gtk_tree_model_iter_n_children (window->list_model, NULL);
        for (i = 0; i < nchildren; i++) {
		GtkTreeIter iter;
		char *name;
		gtk_tree_model_iter_nth_child (window->sorted_list_model,
						&iter, NULL, i);

		gtk_tree_model_get (window->sorted_list_model, &iter,
				    GCONF_LIST_MODEL_KEY_NAME_COLUMN, &name, -1);
		if (strcmp (key, name) == 0) {
			path = gtk_tree_model_get_path (window->sorted_list_model, &iter);
			gtk_tree_selection_select_path (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->list_view)),
                        				path);
			gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (window->list_view),
						      path, NULL, TRUE, 0.5, 0.5);
			gtk_tree_path_free (path);
			g_free (name);
			return;
		}
		g_free (name);
	}
}