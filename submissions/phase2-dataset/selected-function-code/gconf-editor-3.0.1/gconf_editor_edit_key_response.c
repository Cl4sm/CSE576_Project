static void
gconf_editor_edit_key_response (GtkDialog *editor,
				int        response,
				GConfEditorWindow *window)
{
	if (response == GTK_RESPONSE_OK) {
		GConfValue *value;
		GError     *error = NULL;
		const char *path;

		value = gconf_key_editor_get_value (GCONF_KEY_EDITOR (editor));

		path = gconf_key_editor_get_key_name (GCONF_KEY_EDITOR (editor));
		g_assert (gconf_valid_key (path, NULL));

		/* if not writable we weren't allowed to change things anyway */
		if (gconf_client_key_is_writable (window->client, path, &error)) {
			gconf_client_set (window->client, path, value, &error);
			gconf_add_recent_key (window, path);
		}

		gconf_value_free (value);

		if (error != NULL) {
			gconf_editor_window_popup_error_dialog (GTK_WINDOW (editor),
								_("Could not change key value. Error message:\n%s"),
								error);
			return;
		}
	}

	gtk_widget_destroy (GTK_WIDGET (editor));
}