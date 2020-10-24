static void
gconf_editor_window_copy_key_name (GtkAction *action, GtkWidget *callback_data)
{
	GConfEditorWindow *window = GCONF_EDITOR_WINDOW (callback_data);
	char *path;
	gchar *key;
	GtkTreeIter iter;
	GtkTreeIter child_iter;
	
	if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection
					     (GTK_TREE_VIEW (window->list_view)),
					     NULL, &iter) == TRUE) {
		gtk_tree_model_get (window->sorted_list_model, &iter,
                	            GCONF_LIST_MODEL_KEY_PATH_COLUMN, &key,
                        	    -1);
		gtk_clipboard_set_text (gtk_clipboard_get
					(GDK_SELECTION_CLIPBOARD), key, -1);
		g_free (key);
		return;
	}
		
	gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->tree_view)), NULL, &iter);

	gtk_tree_model_sort_convert_iter_to_child_iter (GTK_TREE_MODEL_SORT (window->sorted_tree_model), &child_iter, &iter);
	path = gconf_tree_model_get_gconf_path (GCONF_TREE_MODEL (window->tree_model), &child_iter);

	gtk_clipboard_set_text (gtk_clipboard_get (GDK_SELECTION_CLIPBOARD), path, -1);

	g_free (path);
}