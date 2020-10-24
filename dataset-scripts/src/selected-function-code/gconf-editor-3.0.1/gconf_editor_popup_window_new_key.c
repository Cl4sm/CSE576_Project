static void
gconf_editor_popup_window_new_key (GtkAction *action, GtkWidget *callback_data)
{
	GConfEditorWindow *window = GCONF_EDITOR_WINDOW (callback_data);
	GtkTreeIter iter;
	GtkWidget *editor;
	char *path;
	
	editor = gconf_key_editor_new (GCONF_KEY_EDITOR_NEW_KEY);

	if (gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->tree_view)), NULL, &iter)) {
		GtkTreeIter child_iter;

		gtk_tree_model_sort_convert_iter_to_child_iter (GTK_TREE_MODEL_SORT (window->sorted_tree_model), &child_iter, &iter);
		path = gconf_tree_model_get_gconf_path (GCONF_TREE_MODEL (window->tree_model), &child_iter);
	}
	else {
		path = g_strdup ("/");
	}
	
	gconf_key_editor_set_key_path (GCONF_KEY_EDITOR (editor), path);
	gconf_key_editor_set_writable (GCONF_KEY_EDITOR (editor), TRUE);
	g_free (path);

	g_signal_connect (editor, "response",
			  G_CALLBACK (gconf_editor_new_key_response), window);

	gtk_widget_show (editor);
}