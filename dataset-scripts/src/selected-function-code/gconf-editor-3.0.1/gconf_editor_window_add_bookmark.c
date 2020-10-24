static void
gconf_editor_window_add_bookmark (GtkAction *action, GtkWidget *callback_data)
{
	GConfEditorWindow *window = GCONF_EDITOR_WINDOW (callback_data);
	GtkTreeIter iter;
	char *path;
	
	if (!gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->tree_view)), NULL, &iter)) {
		return;
	}
	else {
		GtkTreeIter child_iter;

		gtk_tree_model_sort_convert_iter_to_child_iter (GTK_TREE_MODEL_SORT (window->sorted_tree_model), &child_iter, &iter);
		path = gconf_tree_model_get_gconf_path (GCONF_TREE_MODEL (window->tree_model), &child_iter);
	}

	gconf_bookmarks_add_bookmark (path);
	
	g_free (path);
}