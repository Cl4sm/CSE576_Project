static void
gconf_editor_window_update_list_selection (GtkTreeSelection *selection, GConfEditorWindow *window)
{
	GtkTreeIter iter;
	GConfSchema *schema;
	char *path;
	
	if (!gtk_tree_selection_get_selected (selection, NULL, &iter)) {
		gtk_label_set_text (GTK_LABEL (window->key_name_label), _("(None)"));
		gtk_label_set_text (GTK_LABEL (window->owner_label), _("(None)"));
		gtk_label_set_text (GTK_LABEL (window->short_desc_label), _("(None)"));
		gtk_text_buffer_set_text (window->long_desc_buffer, _("(None)"), -1);
		gtk_widget_hide (window->non_writable_label);
		gtk_widget_hide (window->no_schema_label);
		
		return;
	}
	
	gtk_tree_model_get (window->sorted_list_model, &iter,
			    GCONF_LIST_MODEL_KEY_PATH_COLUMN, &path,
			    -1);
	
	gtk_label_set_text (GTK_LABEL (window->key_name_label), path);

	if (gconf_client_key_is_writable (window->client, path, NULL))
		gtk_widget_hide (window->non_writable_label);
	else
		gtk_widget_show (window->non_writable_label);

	schema = gconf_client_get_schema_for_key (window->client, path);

	if (schema != NULL && gconf_schema_get_long_desc (schema) != NULL) {
		char *long_desc;

		long_desc = strip_whitespace (gconf_schema_get_long_desc (schema));
		
		gtk_text_buffer_set_text (window->long_desc_buffer, long_desc, -1);
		g_free (long_desc);
	}
	else {
		gtk_text_buffer_set_text (window->long_desc_buffer, _("(None)"), -1);
	}

	if (schema != NULL && gconf_schema_get_short_desc (schema) != NULL) {
		set_label_and_strip_whitespace (GTK_LABEL (window->short_desc_label),
						gconf_schema_get_short_desc (schema));
	}
	else {
		gtk_label_set_text (GTK_LABEL (window->short_desc_label), _("(None)"));

	}

	if (schema != NULL && gconf_schema_get_owner (schema) != NULL) {
		set_label_and_strip_whitespace (GTK_LABEL (window->owner_label),
						gconf_schema_get_owner (schema));
	}
	else {
		gtk_label_set_text (GTK_LABEL (window->owner_label), _("(None)"));

	}

	if (schema == NULL) {
		GConfValue *value = gconf_client_get (window->client, path, NULL);
		if (value != NULL) {
			if (value->type == GCONF_VALUE_SCHEMA) {
				gtk_widget_hide (window->no_schema_label);
			}
			else {
				gtk_widget_show (window->no_schema_label);
			}
			gconf_value_free (value);
		}
	}
	else {
		gtk_widget_hide (window->no_schema_label);
	}

	if (schema != NULL)
		gconf_schema_free (schema);

	gtk_statusbar_pop (GTK_STATUSBAR (window->statusbar), 0);
	gtk_statusbar_push (GTK_STATUSBAR (window->statusbar), 0, path);

	g_free (path);
}