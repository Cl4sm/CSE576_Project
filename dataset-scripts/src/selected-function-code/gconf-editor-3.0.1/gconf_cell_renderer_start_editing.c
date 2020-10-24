static GtkCellEditable *
gconf_cell_renderer_start_editing (GtkCellRenderer      *cell,
				   GdkEvent             *event,
				   GtkWidget            *widget,
				   const gchar          *path,
				   const GdkRectangle   *background_area,
				   const GdkRectangle   *cell_area,
				   GtkCellRendererState  flags)
{
	GtkWidget *entry;
	GConfCellRenderer *cellvalue;
	gchar *tmp_str;

	cellvalue = GCONF_CELL_RENDERER (cell);

	/* If not writable then we definately can't edit */
	if ( ! gconf_cell_renderer_check_writability (cellvalue, path))
		return NULL;
	
	switch (cellvalue->value->type) {
	case GCONF_VALUE_INT:
	case GCONF_VALUE_FLOAT:
	case GCONF_VALUE_STRING:
		tmp_str = gconf_value_to_string (cellvalue->value);
		entry = g_object_new (GTK_TYPE_ENTRY,
				      "has_frame", FALSE,
				      "text", tmp_str,
				      NULL);
		g_free (tmp_str);
		g_signal_connect (entry, "editing_done",
				  G_CALLBACK (gconf_cell_renderer_text_editing_done), cellvalue);

		g_object_set_data_full (G_OBJECT (entry), GCONF_CELL_RENDERER_TEXT_PATH, g_strdup (path), g_free);
		g_object_set_data_full (G_OBJECT (entry), GCONF_CELL_RENDERER_VALUE, gconf_value_copy (cellvalue->value), (GDestroyNotify)gconf_value_free);
		
		gtk_widget_show (entry);

		return GTK_CELL_EDITABLE (entry);
		break;
	default:
		g_error ("%d shouldn't be handled here", cellvalue->value->type);
		break;
	}

	
	return NULL;
}