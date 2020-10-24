static void
gconf_list_model_get_value (GtkTreeModel *tree_model, GtkTreeIter *iter, gint column, GValue *value)
{
	GConfEntry *entry;
	
	g_return_if_fail (iter->stamp == GCONF_LIST_MODEL (tree_model)->stamp);

	entry = G_SLIST (iter->user_data)->data;
	
	switch (column) {
	case GCONF_LIST_MODEL_KEY_PATH_COLUMN:
		g_value_init (value, G_TYPE_STRING);
		g_value_set_string (value, gconf_entry_get_key (entry));
		break;
		
	case GCONF_LIST_MODEL_KEY_NAME_COLUMN:
		g_value_init (value, G_TYPE_STRING);
		g_value_take_string (value, gconf_get_key_name_from_path (gconf_entry_get_key (entry)));
		break;
		
	case GCONF_LIST_MODEL_ICON_NAME_COLUMN: {
                GConfValue *gconf_value;
                GConfValueType value_type;

                gconf_value = gconf_entry_get_value (entry);
                if (gconf_value) {
                        value_type = gconf_value->type;
                } else {
                        value_type = GCONF_VALUE_INVALID;
                }

                g_value_init (value, G_TYPE_STRING);
                g_value_set_static_string (value, gconf_value_icon_names[value_type]);
		
		break;
        }
		
	case GCONF_LIST_MODEL_VALUE_COLUMN:
		g_value_init (value, GCONF_TYPE_VALUE);
		g_value_set_boxed (value, gconf_entry_get_value (entry));
		break;

	default:
		break;
	}
}