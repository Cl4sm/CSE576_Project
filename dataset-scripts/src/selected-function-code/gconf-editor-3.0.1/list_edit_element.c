static void
list_edit_element (GConfKeyEditor *editor)
{
	GtkWidget *content_area;
	GtkWidget *dialog;
	GtkWidget *hbox;
	GtkWidget *value_widget;
	GtkWidget *label;
	gint response;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GConfValue *value = NULL;

	dialog = gtk_dialog_new_with_buttons (_("Edit List Entry"),
                                              GTK_WINDOW (editor),
					      GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,
					      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					      GTK_STOCK_OK, GTK_RESPONSE_OK,
					      NULL);
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);

	gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
	gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
	gtk_box_set_spacing (GTK_BOX (content_area), 2);
	
	hbox = gtk_hbox_new (FALSE, 12);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
	gtk_box_pack_start (GTK_BOX (content_area), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic (_("_Edit list value:"));
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->list_widget));
	gtk_tree_selection_get_selected (selection, &model, &iter);
	gtk_tree_model_get (GTK_TREE_MODEL (model), &iter, 0, &value, -1);
	switch (gtk_combo_box_get_active (GTK_COMBO_BOX (editor->list_type_menu))) {
		case EDIT_INTEGER:
			value_widget = gtk_spin_button_new_with_range (G_MININT, G_MAXINT, 1);
			gtk_spin_button_set_value (GTK_SPIN_BUTTON (value_widget), gconf_value_get_int (value));
			break;
		case EDIT_BOOLEAN:
			value_widget = gtk_toggle_button_new_with_mnemonic (_("_False"));
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (value_widget), gconf_value_get_bool (value));
			break;
		case EDIT_STRING:
			value_widget = gtk_entry_new ();
			gtk_entry_set_text (GTK_ENTRY (value_widget), gconf_value_get_string (value));
			break;
		default:
			value_widget = NULL;
			g_assert_not_reached ();
	}
	gconf_value_free (value);

	gtk_label_set_mnemonic_widget (GTK_LABEL (label), value_widget);
	gtk_box_pack_start (GTK_BOX (hbox), value_widget, TRUE, TRUE, 0);

	gtk_widget_show_all (hbox);

	response = gtk_dialog_run (GTK_DIALOG (dialog));

	if (response == GTK_RESPONSE_OK) {
		GConfValue *value;
		
	        value = NULL;

		switch (gtk_combo_box_get_active (GTK_COMBO_BOX (editor->list_type_menu))) {
			case EDIT_INTEGER:
				value = gconf_value_new (GCONF_VALUE_INT);
				gconf_value_set_int (value,
						     gtk_spin_button_get_value (GTK_SPIN_BUTTON (value_widget)));
				break;

			case EDIT_BOOLEAN:
				value = gconf_value_new (GCONF_VALUE_BOOL);
				gconf_value_set_bool (value,
						      gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (value_widget)));
				break;

			case EDIT_STRING:
		                {
		                        char *text;
				
		                        text = gtk_editable_get_chars (GTK_EDITABLE (value_widget), 0, -1);
		                        value = gconf_value_new (GCONF_VALUE_STRING);
		                        gconf_value_set_string (value, text);
		                        g_free (text);
		                }
	                	break;
			default:
				g_assert_not_reached ();
				
		}
                gtk_list_store_set (editor->list_model, &iter, 0, value, -1);

	}
	
	gtk_widget_destroy (dialog);
}