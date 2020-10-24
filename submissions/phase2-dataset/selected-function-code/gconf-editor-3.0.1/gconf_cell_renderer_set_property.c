static void
gconf_cell_renderer_set_property (GObject *object, guint param_id, const GValue *value, GParamSpec *pspec)
{
	GConfCellRenderer *cellvalue;
	GConfValue *gconf_value;
	GtkCellRendererMode new_mode = GTK_CELL_RENDERER_MODE_INERT;
	
	cellvalue = GCONF_CELL_RENDERER (object);

	switch (param_id) {
	case PROP_VALUE:
		if (cellvalue->value) 
			gconf_value_free (cellvalue->value);

		gconf_value = g_value_get_boxed (value);

		if (gconf_value) {
			cellvalue->value = gconf_value_copy (gconf_value);

			switch (gconf_value->type) {
			case GCONF_VALUE_INT:
			case GCONF_VALUE_FLOAT:
			case GCONF_VALUE_STRING:
				new_mode = GTK_CELL_RENDERER_MODE_EDITABLE;
				break;
				
			case GCONF_VALUE_BOOL:
				new_mode = GTK_CELL_RENDERER_MODE_ACTIVATABLE;
				break;

			case GCONF_VALUE_LIST:
			case GCONF_VALUE_SCHEMA:
			case GCONF_VALUE_PAIR:
				new_mode = GTK_CELL_RENDERER_MODE_INERT;
				break;
			default:
				g_warning ("unhandled value type %d", gconf_value->type);
				break;
			}
		}
		else {
			cellvalue->value = NULL;
		}

		g_object_set (object, "mode", new_mode, NULL);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
		break;	
	}
}