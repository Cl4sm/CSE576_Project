static void
gconf_key_editor_init (GConfKeyEditor *editor)
{
	GtkWidget *content_area;
	GtkWidget *hbox, *vbox, *framebox;
	GtkWidget *label, *image;
	GtkWidget *value_box;
	GtkWidget *button_box, *button;
	GtkSizeGroup *size_group;
	GtkCellRenderer *cell_renderer;
	GtkTreeSelection *list_selection;
	GtkWidget *sw;

	gtk_dialog_add_buttons (GTK_DIALOG (editor),
				GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, 
				GTK_STOCK_OK, GTK_RESPONSE_OK,
				NULL);
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (editor));
	gtk_dialog_set_default_response (GTK_DIALOG (editor), GTK_RESPONSE_OK);

	gtk_container_set_border_width (GTK_CONTAINER (editor), 5);
	gtk_window_set_resizable (GTK_WINDOW (editor), FALSE);
	gtk_box_set_spacing (GTK_BOX (content_area), 2);

	vbox = gtk_vbox_new (FALSE, 6);
	gtk_box_pack_start (GTK_BOX (content_area), vbox, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
	gtk_widget_show_all (vbox);

	size_group = gtk_size_group_new (GTK_SIZE_GROUP_BOTH);
	
	editor->path_box = gtk_hbox_new (FALSE, 12);
	label = gtk_label_new (_("Path:"));
	gtk_size_group_add_widget (size_group, label);
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
	gtk_box_pack_start (GTK_BOX (editor->path_box), label, FALSE, FALSE, 0);
	editor->path_label = gtk_label_new (NULL);
	gtk_misc_set_alignment (GTK_MISC (editor->path_label), 0.0, 0.5);
	gtk_box_pack_start (GTK_BOX (editor->path_box), editor->path_label, TRUE, TRUE, 0);
	gtk_widget_show_all (editor->path_box);
	gtk_box_pack_start (GTK_BOX (vbox), editor->path_box, FALSE, FALSE, 0);

	hbox = gtk_hbox_new (FALSE, 12);
	label = gtk_label_new_with_mnemonic (_("_Name:"));
	gtk_size_group_add_widget (size_group, label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	editor->name_entry = gtk_entry_new ();
	gtk_widget_set_size_request (editor->name_entry, 250, -1);
	gtk_entry_set_activates_default (GTK_ENTRY (editor->name_entry), TRUE);
	gtk_box_pack_start (GTK_BOX (hbox), editor->name_entry, TRUE, TRUE, 0);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), editor->name_entry);
	gtk_widget_show_all (hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

	hbox = gtk_hbox_new (FALSE, 12);
	label = gtk_label_new_with_mnemonic (_("_Type:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
	gtk_size_group_add_widget (size_group, label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	editor->combobox = gconf_key_editor_create_combo_box (editor);
	gtk_box_pack_start (GTK_BOX (hbox),
			    editor->combobox,
			    TRUE, TRUE, 0);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), editor->combobox);
	gtk_widget_show_all (hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

	g_object_unref (size_group);

	framebox = gtk_vbox_new (FALSE, 6);
	gtk_widget_show (framebox);
	gtk_box_pack_start (GTK_BOX (vbox), framebox, FALSE, FALSE, 0);

	editor->non_writable_label = gtk_hbox_new (FALSE, 12);
	gtk_box_pack_start (GTK_BOX (framebox), editor->non_writable_label, FALSE, FALSE, 0);

	image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (editor->non_writable_label), image, FALSE, FALSE, 0);

	label = gtk_label_new (_("This key is not writable"));
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (editor->non_writable_label), label, FALSE, FALSE, 0);

	editor->active_type = EDIT_INTEGER;

	/* EDIT_INTEGER */
	editor->int_box = gtk_hbox_new (FALSE, 0);
	hbox = gtk_hbox_new (FALSE, 12);
	label = gtk_label_new ("    ");
	gtk_box_pack_start (GTK_BOX (editor->int_box), label, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic (_("_Value:"));
	editor->int_widget = gtk_spin_button_new_with_range (G_MININT, G_MAXINT, 1);

	/* Set a nicer default value */
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (editor->int_widget), 0);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), editor->int_widget);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), editor->int_widget, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (editor->int_box), hbox, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), editor->int_box, FALSE, FALSE, 0);
	gtk_widget_show_all (editor->int_box);
	
	/* EDIT_BOOLEAN */
	editor->bool_box = gtk_hbox_new (FALSE, 0);
	hbox = gtk_hbox_new (FALSE, 12);
	label = gtk_label_new ("    ");
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (editor->bool_box), label, FALSE, FALSE, 0);
	label = gtk_label_new_with_mnemonic (_("_Value:"));
	editor->bool_widget = gtk_toggle_button_new_with_mnemonic (_("_False"));
	g_signal_connect (editor->bool_widget, "toggled",
			  G_CALLBACK (bool_button_toggled),
			  editor);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), editor->bool_widget);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), editor->bool_widget, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (editor->bool_box), hbox, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), editor->bool_box, FALSE, FALSE, 0);
	gtk_widget_show_all (hbox);
	gtk_widget_hide (editor->bool_box);

	/* EDIT_STRING */
	editor->string_box = gtk_hbox_new (FALSE, 0);
	hbox = gtk_hbox_new (FALSE, 12);
	label = gtk_label_new ("    ");
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (editor->string_box), label, FALSE, FALSE, 0);
	label = gtk_label_new_with_mnemonic (_("_Value:"));
	editor->string_widget = gtk_entry_new ();
	gtk_entry_set_activates_default (GTK_ENTRY (editor->string_widget), TRUE);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), editor->string_widget);	
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), editor->string_widget, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (editor->string_box), hbox, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), editor->string_box, FALSE, FALSE, 0);
	gtk_widget_show_all (hbox);
	gtk_widget_hide (editor->string_box);

	/* EDIT_FLOAT */
	editor->float_box = gtk_hbox_new (FALSE, 0);
	hbox = gtk_hbox_new (FALSE, 12);
	label = gtk_label_new ("    ");
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (editor->float_box), label, FALSE, FALSE, 0);
	label = gtk_label_new_with_mnemonic (_("_Value:"));
	editor->float_widget = gtk_spin_button_new_with_range (G_MINFLOAT, G_MAXFLOAT, 0.1);

	/* Set a nicer default value */
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (editor->float_widget), 0.0);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), editor->float_widget);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), editor->float_widget, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (editor->float_box), hbox, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), editor->float_box, FALSE, FALSE, 0);
	gtk_widget_show_all (hbox);
	gtk_widget_hide (editor->float_box);

        /* EDIT_LIST */
	editor->list_box = gtk_hbox_new (FALSE, 0);
	label = gtk_label_new ("    ");
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (editor->list_box), label, FALSE, FALSE, 0);

        value_box = gtk_vbox_new (FALSE, 6);

	hbox = gtk_hbox_new (FALSE, 12);
	label = gtk_label_new_with_mnemonic (_("List _type:"));
	editor->list_type_menu = gconf_key_editor_create_list_type_menu (editor);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), editor->list_type_menu);
	gtk_box_pack_start (GTK_BOX (value_box), hbox, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (hbox),
                            editor->list_type_menu, TRUE, TRUE, 0);

	label = gtk_label_new_with_mnemonic (_("_Values:"));
	gtk_misc_set_alignment(GTK_MISC (label), 0.0, 0.5);
  
	hbox = gtk_hbox_new (FALSE, 12);

	sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
					     GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
					GTK_POLICY_AUTOMATIC,			      					GTK_POLICY_AUTOMATIC);

        editor->list_model = gtk_list_store_new (1, g_type_from_name("GConfValue"));
	editor->list_widget = gtk_tree_view_new_with_model (GTK_TREE_MODEL (editor->list_model));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (editor->list_widget), FALSE);
	list_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (editor->list_widget));
	g_signal_connect (G_OBJECT (editor->list_widget), "row_activated",
			  G_CALLBACK (list_view_row_activated), editor);
	g_signal_connect (G_OBJECT (list_selection), "changed",
			  G_CALLBACK (list_selection_changed), editor);
 	
	
	cell_renderer = gconf_cell_renderer_new ();
	g_signal_connect (G_OBJECT (cell_renderer), "changed", G_CALLBACK (gconf_key_editor_list_entry_changed), editor);

	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (editor->list_widget), -1, NULL, cell_renderer, "value", 0, NULL);

	button_box = gtk_vbutton_box_new ();
	gtk_button_box_set_layout (GTK_BUTTON_BOX (button_box), GTK_BUTTONBOX_START);
	gtk_box_set_spacing (GTK_BOX (button_box), 6);
	button = gtk_button_new_from_stock (GTK_STOCK_ADD);
	fix_button_align (button);
	editor->remove_button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
	fix_button_align (editor->remove_button);
	editor->edit_button = gtk_button_new_from_stock (GTK_STOCK_EDIT);
	fix_button_align (editor->edit_button);
	editor->go_up_button = gtk_button_new_from_stock (GTK_STOCK_GO_UP);
	fix_button_align (editor->go_up_button);
	editor->go_down_button = gtk_button_new_from_stock (GTK_STOCK_GO_DOWN);
	fix_button_align (editor->go_down_button);
	g_signal_connect (G_OBJECT (button), "clicked",
			  G_CALLBACK (list_add_clicked), editor);
	g_signal_connect (G_OBJECT (editor->edit_button), "clicked",
			  G_CALLBACK (list_edit_clicked), editor);
	g_signal_connect (G_OBJECT (editor->remove_button), "clicked",
			  G_CALLBACK (list_remove_clicked), editor);
	g_signal_connect (G_OBJECT (editor->go_up_button), "clicked",
			  G_CALLBACK (list_go_up_clicked), editor);
	g_signal_connect (G_OBJECT (editor->go_down_button), "clicked",
			  G_CALLBACK (list_go_down_clicked), editor);
	gtk_box_pack_start (GTK_BOX (button_box), button, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (button_box), editor->remove_button, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (button_box), editor->edit_button, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (button_box), editor->go_up_button, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (button_box), editor->go_down_button, FALSE, FALSE, 0);

	update_list_buttons (editor);

        gtk_label_set_mnemonic_widget (GTK_LABEL (label), editor->list_widget); 
        gtk_box_pack_start (GTK_BOX (value_box), label, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (value_box), hbox, TRUE, TRUE, 0);
        gtk_box_pack_start (GTK_BOX (hbox), sw, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), button_box, FALSE, FALSE, 0);
	gtk_container_add (GTK_CONTAINER (sw), editor->list_widget);
	gtk_box_pack_start (GTK_BOX (editor->list_box), value_box, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), editor->list_box, TRUE, TRUE, 0);
	gtk_widget_show_all (value_box);
	gtk_widget_hide (editor->list_box);
}