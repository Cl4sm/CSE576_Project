static gint
gconf_cell_renderer_activate (GtkCellRenderer      *cell,
			      GdkEvent             *event,
			      GtkWidget            *widget,
			      const gchar          *path,
			      const GdkRectangle   *background_area,
			      const GdkRectangle   *cell_area,
			      GtkCellRendererState  flags)
{
	GConfCellRenderer *cellvalue;
	
	cellvalue = GCONF_CELL_RENDERER (cell);

	if (cellvalue->value == NULL)
		return TRUE;

	switch (cellvalue->value->type) {
	case GCONF_VALUE_BOOL:
		gconf_value_set_bool (cellvalue->value, !gconf_value_get_bool (cellvalue->value));
		g_signal_emit (cell, gconf_cell_signals[CHANGED], 0, path, cellvalue->value);
		
		break;
	default:
		g_error ("%d shouldn't be handled here", cellvalue->value->type);
		break;
	}

	return TRUE;
}