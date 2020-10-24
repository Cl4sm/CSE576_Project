GtkWidget *
gconf_key_editor_new (GConfKeyEditorAction action)
{
	GConfKeyEditor *dialog;

	dialog = g_object_new (GCONF_TYPE_KEY_EDITOR, NULL);

	switch (action) {
	case GCONF_KEY_EDITOR_NEW_KEY:
		gtk_window_set_title (GTK_WINDOW (dialog), _("New Key"));
		gtk_widget_grab_focus (dialog->name_entry);
		break;
	case GCONF_KEY_EDITOR_EDIT_KEY:
		gtk_window_set_title (GTK_WINDOW (dialog), _("Edit Key"));

		gtk_widget_set_sensitive (dialog->name_entry, FALSE);
		gtk_widget_hide (dialog->path_box);
		break;
	default:
		break;
	}
	
	return GTK_WIDGET (dialog);
}