void
gconf_key_editor_set_value (GConfKeyEditor *editor, GConfValue *value)
{
	if (value == NULL) {
		g_print ("dealing with an unset value\n");
		return;
	}

	gtk_widget_set_sensitive (editor->combobox, FALSE);
	gtk_widget_set_sensitive (editor->list_type_menu, FALSE);

	switch (value->type) {
	case GCONF_VALUE_INT:
		gtk_combo_box_set_active (GTK_COMBO_BOX (editor->combobox), EDIT_INTEGER);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (editor->int_widget), gconf_value_get_int (value));
		break;
	case GCONF_VALUE_STRING:
		gtk_combo_box_set_active (GTK_COMBO_BOX (editor->combobox), EDIT_STRING);
		gtk_entry_set_text (GTK_ENTRY (editor->string_widget), gconf_value_get_string (value));
		break;
	case GCONF_VALUE_BOOL:
		gtk_combo_box_set_active (GTK_COMBO_BOX (editor->combobox), EDIT_BOOLEAN);
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (editor->bool_widget), gconf_value_get_bool (value));
		break;
	case GCONF_VALUE_FLOAT:
		gtk_combo_box_set_active (GTK_COMBO_BOX (editor->combobox), EDIT_FLOAT);
		gtk_spin_button_set_value (GTK_SPIN_BUTTON (editor->float_widget), gconf_value_get_float (value));
		break;
	case GCONF_VALUE_LIST:
		{
			GSList* iter = gconf_value_get_list(value);
			
			switch (gconf_value_get_list_type(value)) {
				case GCONF_VALUE_INT:
					gtk_combo_box_set_active (GTK_COMBO_BOX (editor->list_type_menu), EDIT_INTEGER);
					break;
				case GCONF_VALUE_FLOAT:
					gtk_combo_box_set_active (GTK_COMBO_BOX (editor->list_type_menu), EDIT_FLOAT);
					break;
				case GCONF_VALUE_STRING:
					gtk_combo_box_set_active (GTK_COMBO_BOX (editor->list_type_menu), EDIT_STRING);
					break;
				case GCONF_VALUE_BOOL:
					gtk_combo_box_set_active (GTK_COMBO_BOX (editor->list_type_menu), EDIT_BOOLEAN);
					break;
				default:
					g_assert_not_reached ();
			}

			gtk_combo_box_set_active (GTK_COMBO_BOX (editor->combobox), EDIT_LIST);
				
		        while (iter != NULL) {
				GConfValue* element = (GConfValue*) iter->data;
				GtkTreeIter tree_iter;

			        gtk_list_store_append (editor->list_model, &tree_iter);
			        gtk_list_store_set (editor->list_model, &tree_iter, 0, element, -1);
				iter = g_slist_next(iter);
			}
		}
		break;
	default:
		g_assert_not_reached ();
		break;
	}
	
}