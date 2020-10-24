static void
gconf_editor_popup_window_unset_key (GtkAction *action, GtkWidget *callback_data)
{
	GConfEditorWindow *window = GCONF_EDITOR_WINDOW (callback_data);
	GtkTreeIter iter;
	gchar *key;
	GError *error = NULL;
	
	if (!gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->list_view)),
					 NULL, &iter)) {
		return;
	}

	gtk_tree_model_get (window->sorted_list_model, &iter,
			    GCONF_LIST_MODEL_KEY_PATH_COLUMN, &key,
			    -1);

	gconf_client_unset (window->client, key, &error);
	g_free (key);

	if (error != NULL) {
		gconf_editor_window_popup_error_dialog (GTK_WINDOW (window),
							_("Couldn't unset key. Error was:\n%s"), error);
	}
}