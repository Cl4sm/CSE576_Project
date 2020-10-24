static void
gconf_editor_popup_window_edit_key (GtkAction *action, GtkWidget *callback_data)
{
	GConfEditorWindow *window = GCONF_EDITOR_WINDOW (callback_data);
	GtkTreeIter iter;
	GtkWidget *editor, *dialog;
	GConfValue *value; 
	char *path = NULL;
	
	if (!gtk_tree_selection_get_selected (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->list_view)),
					      NULL, &iter))
		return;
	
	gtk_tree_model_get (window->sorted_list_model, &iter,
			    GCONF_LIST_MODEL_KEY_PATH_COLUMN, &path,
			    GCONF_LIST_MODEL_VALUE_COLUMN, &value,
			    -1);

	if (value && (value->type == GCONF_VALUE_SCHEMA ||
		      value->type == GCONF_VALUE_PAIR)) {
		dialog = gtk_message_dialog_new (GTK_WINDOW (window), 0,
						 GTK_MESSAGE_INFO,
						 GTK_BUTTONS_OK,
						 _("Currently pairs and schemas can't "
						   "be edited. This will be changed in a later "
						   "version."));
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		gconf_value_free (value);
		g_free (path);
		return;
	}
	
	editor = gconf_key_editor_new (GCONF_KEY_EDITOR_EDIT_KEY);

	if(value) {
	  gconf_key_editor_set_value (GCONF_KEY_EDITOR (editor), value);
	  gconf_value_free (value);
	}

	gconf_key_editor_set_key_name (GCONF_KEY_EDITOR (editor), path);

	gconf_key_editor_set_writable (GCONF_KEY_EDITOR (editor),
				       gconf_client_key_is_writable (window->client, path, NULL));

	g_free (path);

	g_signal_connect (editor, "response",
			  G_CALLBACK (gconf_editor_edit_key_response), window);

	gtk_widget_show (editor);
}