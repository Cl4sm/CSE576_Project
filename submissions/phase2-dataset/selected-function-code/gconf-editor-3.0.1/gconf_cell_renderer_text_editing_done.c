static void
gconf_cell_renderer_text_editing_done (GtkCellEditable *entry, GConfCellRenderer *cell)
{
	const gchar *path;
	const gchar *new_text;
	gboolean editing_canceled = FALSE;
	GConfValue *value;

	g_object_get (entry, "editing-canceled", &editing_canceled, NULL);
	if (editing_canceled != FALSE)
		return;

	path = g_object_get_data (G_OBJECT (entry), GCONF_CELL_RENDERER_TEXT_PATH);
	value = g_object_get_data (G_OBJECT (entry), GCONF_CELL_RENDERER_VALUE);
	new_text = gtk_entry_get_text (GTK_ENTRY (entry));

	switch (value->type) {
	case GCONF_VALUE_STRING:
		gconf_value_set_string (value, new_text);
		break;
	case GCONF_VALUE_FLOAT:
		gconf_value_set_float (value, (gdouble)(g_ascii_strtod (new_text, NULL)));
		break;
	case GCONF_VALUE_INT:
		gconf_value_set_int (value, (gint)(g_ascii_strtod (new_text, NULL)));
		break;
	default:
		g_error ("editing done, unknown value %d", value->type);
		break;
	}
	
	g_signal_emit (cell, gconf_cell_signals[CHANGED], 0, path, value);
}