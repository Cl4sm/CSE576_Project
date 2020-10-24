static void
gconf_cell_renderer_render (GtkCellRenderer *cell,
			    cairo_t *cr,
			    GtkWidget *widget,
			    const GdkRectangle *background_area,
			    const GdkRectangle *cell_area,
			    GtkCellRendererState flags)
{
	GConfCellRenderer *cellvalue;
	char *tmp_str;
	
	cellvalue = GCONF_CELL_RENDERER (cell);

	if (cellvalue->value == NULL) {
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", _("<no value>"),
			      NULL);

		gtk_cell_renderer_render (cellvalue->text_renderer, cr, widget,
					  background_area, cell_area, flags);
		return;
	}

	switch (cellvalue->value->type) {
	case GCONF_VALUE_FLOAT:
	case GCONF_VALUE_INT:
		tmp_str = gconf_value_to_string (cellvalue->value);
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", tmp_str,
			      NULL);
		gtk_cell_renderer_render (cellvalue->text_renderer, cr, widget,
					  background_area, cell_area, flags);
		g_free (tmp_str);
		break;
	case GCONF_VALUE_STRING:
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", gconf_value_get_string (cellvalue->value),
			      NULL);
		gtk_cell_renderer_render (cellvalue->text_renderer, cr, widget,
					  background_area, cell_area, flags);
		break;
	case GCONF_VALUE_BOOL:
		g_object_set (G_OBJECT (cellvalue->toggle_renderer),
			      "xalign", 0.0,
			      "active", gconf_value_get_bool (cellvalue->value),
			      NULL);
		
		gtk_cell_renderer_render (cellvalue->toggle_renderer, cr, widget,
					  background_area, cell_area, flags);
		break;

	case GCONF_VALUE_SCHEMA:
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", SCHEMA_TEXT,
			      NULL);
		
		gtk_cell_renderer_render (cellvalue->text_renderer, cr, widget,
					  background_area, cell_area, flags);
		break;
		
	case GCONF_VALUE_LIST:
		tmp_str = gconf_value_to_string (cellvalue->value);
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", tmp_str,
			      NULL);
		
		gtk_cell_renderer_render (cellvalue->text_renderer, cr, widget,
					  background_area, cell_area, flags);
		g_free (tmp_str);
		break;
	case GCONF_VALUE_PAIR:
		tmp_str = gconf_value_to_string (cellvalue->value);
		g_object_set (G_OBJECT (cellvalue->text_renderer),
			      "text", tmp_str,
			      NULL);
		gtk_cell_renderer_render (cellvalue->text_renderer, cr, widget,
					  background_area, cell_area, flags);
		g_free (tmp_str);
		break;

	default:
		g_print ("render: Unknown type: %d\n", cellvalue->value->type);
		break;
	}
}