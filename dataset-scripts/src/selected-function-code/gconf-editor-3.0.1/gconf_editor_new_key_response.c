static void
gconf_editor_new_key_response (GtkDialog *editor,
			       int        response,
			       GConfEditorWindow *window)
{
	if (response == GTK_RESPONSE_OK) {
		GConfValue *value;
		char       *full_path;
		char       *why_not_valid = NULL;
		
		full_path = gconf_key_editor_get_full_key_path (GCONF_KEY_EDITOR (editor));
		
		if (!gconf_valid_key (full_path, &why_not_valid)) {
			GtkWidget *message_dialog;

			g_assert (why_not_valid != NULL);

			message_dialog = gtk_message_dialog_new (GTK_WINDOW (editor),
								 GTK_DIALOG_MODAL,
								 GTK_MESSAGE_ERROR,
								 GTK_BUTTONS_OK,
								 _("Could not create key. The error is:\n%s"),
								 why_not_valid);
			gtk_dialog_set_default_response (GTK_DIALOG (message_dialog), GTK_RESPONSE_OK);
			gtk_dialog_run (GTK_DIALOG (message_dialog));
			gtk_widget_destroy (message_dialog);

			g_free (full_path);
			g_free (why_not_valid);

			/* leave the key editor in place */
			return;
		}
			
		/* Create the key */
		value = gconf_key_editor_get_value (GCONF_KEY_EDITOR (editor));

		gconf_client_set (window->client, full_path, value, NULL);
		gconf_add_recent_key (window, full_path);

		if(value)
		  gconf_value_free (value);
		g_free (full_path);
	}

	gtk_widget_destroy (GTK_WIDGET (editor));
}