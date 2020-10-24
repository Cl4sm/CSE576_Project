char *
gconf_key_editor_get_full_key_path (GConfKeyEditor *editor)
{
	char *full_key_path;
	const char *key_path;

	key_path = gtk_label_get_text (GTK_LABEL (editor->path_label));

	if (key_path[strlen(key_path) - 1] != '/') {
		full_key_path = g_strdup_printf ("%s/%s",
						 key_path,
						 gtk_entry_get_text (GTK_ENTRY (editor->name_entry)));
	}
	else {
		full_key_path = g_strdup_printf ("%s%s",
						 key_path,
						 gtk_entry_get_text (GTK_ENTRY (editor->name_entry)));

	}

	return full_key_path;
}