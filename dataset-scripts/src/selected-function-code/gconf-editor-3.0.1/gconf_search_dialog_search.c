static void
gconf_search_dialog_search (GtkWidget *button, GConfSearchDialog *dialog)
{
	GConfEditorWindow *window;
	GdkCursor *cursor;
	GdkWindow *dialog_window;

	gchar *pattern;
	int res;

	window = g_object_get_data (G_OBJECT (dialog), "editor-window");
	gedit_output_window_clear (GEDIT_OUTPUT_WINDOW (window->output_window));
	window->output_window_type = GCONF_EDITOR_WINDOW_OUTPUT_WINDOW_SEARCH;

	pattern = g_strdup (gtk_entry_get_text (GTK_ENTRY (dialog->entry)));
	dialog_window = gtk_widget_get_window (GTK_WIDGET (dialog));

	cursor = gdk_cursor_new (GDK_WATCH);
	gdk_window_set_cursor (dialog_window, cursor);
	gdk_cursor_unref (cursor);
	gdk_display_flush (gtk_widget_get_display (GTK_WIDGET (dialog)));

	res = gconf_tree_model_build_match_list  (GCONF_TREE_MODEL (window->tree_model),
						  GEDIT_OUTPUT_WINDOW (window->output_window), 
						  pattern,
						  gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (dialog->search_in_keys)),
						  gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (dialog->search_in_values)),
						  G_OBJECT (dialog));

	g_free (pattern);

	if (dialog != NULL && GTK_IS_WIDGET (dialog)) {
		gdk_window_set_cursor (dialog_window, NULL);
		gdk_display_flush (gtk_widget_get_display (GTK_WIDGET (dialog)));

		if (res != 0) {
			gtk_widget_destroy (GTK_WIDGET (dialog));
		} else {
			gconf_search_not_found_dialog (dialog);
		}
	}
}