static void
gconf_cell_renderer_get_size (GtkCellRenderer *cell,
			      GtkWidget *widget,
			      const GdkRectangle *cell_area,
			      gint *x_offset,
			      gint *y_offset,
			      gint *width,
			      gint *height)
{
	GConfCellRenderer *cellvalue;
	gchar *tmp_str;
	
	cellvalue = GCONF_CELL_RENDERER (cell);

	if (cellvalue->value == NULL) {
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", _("<no value>"),
			      NULL);
		gtk_cell_renderer_get_size (cellvalue->text_renderer, widget, cell_area,
					    x_offset, y_offset, width, height);
		return;
	}
	
	switch (cellvalue->value->type) {
	case GCONF_VALUE_FLOAT:
	case GCONF_VALUE_INT:
		tmp_str = gconf_value_to_string (cellvalue->value);
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", tmp_str,
			      NULL);
		gtk_cell_renderer_get_size (cellvalue->text_renderer, widget, cell_area,
					    x_offset, y_offset, width, height);
		g_free (tmp_str);
		break;
	case GCONF_VALUE_STRING:
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", gconf_value_get_string (cellvalue->value),
			      NULL);
		gtk_cell_renderer_get_size (cellvalue->text_renderer, widget, cell_area,
					    x_offset, y_offset, width, height);
		break;
	case GCONF_VALUE_BOOL:
		g_object_set (G_OBJECT (cellvalue->toggle_renderer),
			      "xalign", 0.0,
			      "active", gconf_value_get_bool (cellvalue->value),
			      NULL);
		gtk_cell_renderer_get_size (cellvalue->toggle_renderer, widget, cell_area,
					    x_offset, y_offset, width, height);
		break;
        case GCONF_VALUE_SCHEMA:
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", SCHEMA_TEXT,
			      NULL);
		gtk_cell_renderer_get_size (cellvalue->text_renderer,
					    widget, cell_area,
					    x_offset, y_offset, width, height);
		break;
	case GCONF_VALUE_LIST:
		tmp_str = gconf_value_to_string (cellvalue->value);
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", tmp_str,
			      NULL);
		gtk_cell_renderer_get_size (cellvalue->text_renderer,
					    widget, cell_area,
					    x_offset, y_offset, width, height);
		g_free (tmp_str);
		break;
	case GCONF_VALUE_PAIR:
		tmp_str = gconf_value_to_string (cellvalue->value);
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", tmp_str,
			      NULL);
		gtk_cell_renderer_get_size (cellvalue->text_renderer,
					    widget, cell_area,
					    x_offset, y_offset, width, height);
		g_free (tmp_str);
		break;
	default:
		g_print ("get_size: Unknown type: %d\n", cellvalue->value->type);
		break;
	}
}