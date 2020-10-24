GConfValue*
gconf_key_editor_get_value (GConfKeyEditor *editor)
{
	GConfValue *value;

	value = NULL;
	
	switch (editor->active_type) {

	case EDIT_INTEGER:
		value = gconf_value_new (GCONF_VALUE_INT);
		gconf_value_set_int (value,
				     gtk_spin_button_get_value (GTK_SPIN_BUTTON (editor->int_widget)));
		break;

	case EDIT_BOOLEAN:
		value = gconf_value_new (GCONF_VALUE_BOOL);
		gconf_value_set_bool (value,
				      gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (editor->bool_widget)));
		break;

	case EDIT_STRING:
		{
			char *text;
			
			text = gtk_editable_get_chars (GTK_EDITABLE (editor->string_widget), 0, -1);
			value = gconf_value_new (GCONF_VALUE_STRING);
			gconf_value_set_string (value, text);
			g_free (text);
		}
		break;
	case EDIT_FLOAT:
		value = gconf_value_new (GCONF_VALUE_FLOAT);
		gconf_value_set_float (value,
				       gtk_spin_button_get_value (GTK_SPIN_BUTTON (editor->float_widget)));
		break;

	case EDIT_LIST:
		{
			GSList* list = NULL;
			GtkTreeIter iter;
			GtkTreeModel* model = GTK_TREE_MODEL (editor->list_model);

			if (gtk_tree_model_get_iter_first (model, &iter)) {
				do {
					GConfValue *element;

					gtk_tree_model_get (model, &iter, 0, &element, -1);
					list = g_slist_append (list, element);
				} while (gtk_tree_model_iter_next (model, &iter));
			}

			value = gconf_value_new (GCONF_VALUE_LIST);

			switch (gtk_combo_box_get_active (GTK_COMBO_BOX (editor->list_type_menu))) {
			        case EDIT_INTEGER:
					gconf_value_set_list_type (value, GCONF_VALUE_INT);
					break;
				case EDIT_BOOLEAN:
					gconf_value_set_list_type (value, GCONF_VALUE_BOOL);
					break;
				case EDIT_STRING:
					gconf_value_set_list_type (value, GCONF_VALUE_STRING);
					break;
				default:
					g_assert_not_reached ();
			}
			gconf_value_set_list_nocopy (value, list);
		}
		break;
	}

	return value;
}