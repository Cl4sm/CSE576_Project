static void
gconf_editor_window_init (GConfEditorWindow *window)
{
	GtkWidget *hpaned, *vpaned, *scrolled_window, *vbox;
	GtkTreeViewColumn *column;
	GtkCellRenderer *cell;
	GtkAccelGroup *accel_group;
	GtkActionGroup *action_group;
	GtkWidget *menubar;
	GtkWidget *details_frame, *alignment, *table, *label, *text_view, *image;
	gchar *markup;
	GError *error;
        GdkRGBA bg;
	
	gtk_window_set_default_size (GTK_WINDOW (window), 700, 550);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox);

	action_group = gtk_action_group_new ("GConfEditorMenuActions");
	gtk_action_group_set_translation_domain (action_group, NULL);
	gtk_action_group_add_actions (action_group, entries, G_N_ELEMENTS (entries), window);
	
	window->ui_manager = gtk_ui_manager_new ();
	gtk_ui_manager_set_add_tearoffs (window->ui_manager, TRUE);
	gtk_ui_manager_insert_action_group (window->ui_manager, action_group, 0);

	accel_group = gtk_ui_manager_get_accel_group (window->ui_manager);
	gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

	error = NULL;
	if (!gtk_ui_manager_add_ui_from_string (window->ui_manager, ui_description, -1, &error)) {
      		g_message ("Building menus failed: %s", error->message);
	      	g_error_free (error);
      		exit (EXIT_FAILURE);
    	}

	menubar = gtk_ui_manager_get_widget (window->ui_manager, "/GConfEditorMenu");
	gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);

	window->popup_menu = gtk_ui_manager_get_widget (window->ui_manager, "/GConfKeyPopupMenu");

	/* Hook up bookmarks */
	gconf_bookmarks_hook_up_menu (window,
				      gtk_menu_item_get_submenu (GTK_MENU_ITEM (gtk_ui_manager_get_widget (window->ui_manager,
				      						     "/GConfEditorMenu/BookmarksMenu/"))),
				      gtk_ui_manager_get_widget (window->ui_manager, "/GConfEditorMenu/BookmarksMenu/AddBookmark"),
				      gtk_ui_manager_get_widget (window->ui_manager, "/GConfEditorMenu/BookmarksMenu/EditBookmarks"));

	/* Create content area */
	vpaned = gtk_vpaned_new ();
	gtk_paned_set_position (GTK_PANED (vpaned), 400);
	gtk_box_pack_start (GTK_BOX (vbox), vpaned, TRUE, TRUE, 0);

	hpaned = gtk_hpaned_new ();
	gtk_paned_set_position (GTK_PANED (hpaned), 250);

	gtk_paned_pack1 (GTK_PANED (vpaned), hpaned, FALSE, FALSE);

	/* Create ouput window */
	window->output_window = gedit_output_window_new ();
	window->output_window_type = GCONF_EDITOR_WINDOW_OUTPUT_WINDOW_NONE;
	gedit_output_window_set_select_multiple (GEDIT_OUTPUT_WINDOW (window->output_window),
						 GTK_SELECTION_SINGLE);
	gtk_paned_pack2 (GTK_PANED (vpaned), window->output_window, FALSE, FALSE);
	g_signal_connect (G_OBJECT (window->output_window), "close_requested",
			  G_CALLBACK (gconf_editor_window_close_output_window), window);

	g_signal_connect (G_OBJECT (window->output_window), "selection_changed",
			  G_CALLBACK (gconf_editor_window_output_window_changed), window);

	gconf_editor_window_recents_init ();
	
	/* Create status bar */
	window->statusbar = gtk_statusbar_new ();
	gtk_box_pack_start (GTK_BOX (vbox), window->statusbar, FALSE, FALSE, 0);
	
	/* Create tree model and tree view */
	window->tree_model = gconf_tree_model_new ();
	window->sorted_tree_model = gtk_tree_model_sort_new_with_model (window->tree_model);
	window->tree_view = gtk_tree_view_new_with_model (window->sorted_tree_model);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (window->tree_view), FALSE);
	g_signal_connect (G_OBJECT (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->tree_view))), "changed",
			  G_CALLBACK (gconf_editor_window_selection_changed), window);
	g_signal_connect (G_OBJECT (window->tree_view), "row-activated", 
			  G_CALLBACK (gconf_editor_window_row_activated),
			  window);
	g_signal_connect (window->tree_view, "test-expand-row",
			  G_CALLBACK (gconf_editor_window_test_expand_row),
			  window);
	g_signal_connect (window->tree_view, "row-expanded",
			  G_CALLBACK (gconf_editor_window_row_expanded),
			  window);
	g_object_unref (G_OBJECT (window->tree_model));

	column = gtk_tree_view_column_new ();

	cell = gtk_cell_renderer_pixbuf_new ();
        g_signal_connect (window->tree_view, "style-updated",
                          G_CALLBACK (tree_view_style_updated_cb), cell);
	gtk_tree_view_column_pack_start (column, cell, FALSE);
	cell = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, cell, TRUE);
	gtk_tree_view_column_set_attributes (column, cell,
					     "text", GCONF_TREE_MODEL_NAME_COLUMN,
					     NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (window->tree_view), column);

	gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (window->sorted_tree_model), 0, GTK_SORT_ASCENDING);
	
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
					     GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (scrolled_window), window->tree_view);
	gtk_paned_add1 (GTK_PANED (hpaned), scrolled_window);

	/* Create list model and list view */
	window->list_model = gconf_list_model_new ();
	window->sorted_list_model = gtk_tree_model_sort_new_with_model (window->list_model);
	window->list_view = gtk_tree_view_new_with_model (window->sorted_list_model);
	g_object_unref (G_OBJECT (window->list_model));
	g_object_unref (G_OBJECT (window->sorted_list_model));

	gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (window->sorted_list_model),
					      GCONF_LIST_MODEL_KEY_NAME_COLUMN, GTK_SORT_ASCENDING);

	g_signal_connect (window->list_view, "popup_menu",
			  G_CALLBACK (gconf_editor_window_list_view_popup_menu), window);
	g_signal_connect (window->list_view, "row_activated",
			  G_CALLBACK (gconf_editor_window_list_view_row_activated), window);

	g_signal_connect (gtk_tree_view_get_selection (GTK_TREE_VIEW (window->list_view)), "changed",
			  G_CALLBACK (gconf_editor_window_update_list_selection), window);
	
	column = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_reorderable (column, TRUE);
	gtk_tree_view_column_set_title (column, _("Name"));

	cell = gtk_cell_renderer_pixbuf_new ();
        g_object_set (cell, "stock-size", GTK_ICON_SIZE_MENU, NULL);
	gtk_tree_view_column_pack_start (column, cell, FALSE);
	gtk_tree_view_column_set_attributes (column, cell,
					     "icon-name", GCONF_LIST_MODEL_ICON_NAME_COLUMN,
					     NULL);

	gtk_tree_view_column_set_resizable (column, TRUE);
	cell = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, cell, TRUE);
	gtk_tree_view_column_set_attributes (column, cell,
					     "text", GCONF_LIST_MODEL_KEY_NAME_COLUMN,
					     NULL);
	gtk_tree_view_column_set_sort_column_id (column, GCONF_LIST_MODEL_KEY_NAME_COLUMN);
	gtk_tree_view_append_column (GTK_TREE_VIEW (window->list_view), column);

	cell = gconf_cell_renderer_new ();
	g_signal_connect (cell, "changed",
			  G_CALLBACK (gconf_editor_gconf_value_changed), window);
	g_signal_connect (cell, "check_writable",
			  G_CALLBACK (gconf_editor_check_writable), window);
	
	window->value_column = column = gtk_tree_view_column_new_with_attributes (_("Value"),
										  cell,
										  "value", GCONF_LIST_MODEL_VALUE_COLUMN,
										  NULL);
	g_signal_connect (window->list_view, "button_press_event",
			  G_CALLBACK (list_view_button_press_event), window);

	gtk_tree_view_column_set_reorderable (column, TRUE);
	gtk_tree_view_column_set_resizable (column, TRUE);

	gtk_tree_view_append_column (GTK_TREE_VIEW (window->list_view), column);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
					     GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (scrolled_window), window->list_view);

	vpaned = gtk_vpaned_new ();
	gtk_paned_add2 (GTK_PANED (hpaned), vpaned);

	gtk_paned_pack1 (GTK_PANED (vpaned), scrolled_window, TRUE, FALSE);

	/* Create details area */
	label = gtk_label_new (NULL);
	markup = g_strdup_printf ("<span weight=\"bold\">%s</span>", _("Key Documentation"));
	gtk_label_set_markup (GTK_LABEL (label), markup);
	g_free (markup);

	details_frame = gtk_frame_new (NULL);
	gtk_frame_set_label_widget (GTK_FRAME (details_frame), label);
	gtk_frame_set_shadow_type (GTK_FRAME (details_frame), GTK_SHADOW_NONE);
	gtk_paned_pack2 (GTK_PANED (vpaned), details_frame, FALSE, FALSE);

	alignment = gtk_alignment_new (0.0, 0.0, 1.0, 1.0);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 12, 0, 24, 0);
	gtk_container_add (GTK_CONTAINER (details_frame), alignment);

	table = gtk_table_new (6, 2, FALSE);
	gtk_table_set_row_spacings (GTK_TABLE (table), 6);
	gtk_table_set_col_spacings (GTK_TABLE (table), 6);
	
	gtk_container_add (GTK_CONTAINER (alignment), table);


	window->non_writable_label = gtk_hbox_new (FALSE, 6);
	gtk_table_attach (GTK_TABLE (table), window->non_writable_label,
			  0, 2, 0, 1,
			  GTK_FILL, 0, 0, 0);

	image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (window->non_writable_label), image, FALSE, FALSE, 0);

	label = gtk_label_new (_("This key is not writable"));
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (window->non_writable_label), label, FALSE, FALSE, 0);
	

	window->no_schema_label = gtk_hbox_new (FALSE, 6);
	gtk_table_attach (GTK_TABLE (table), window->no_schema_label,
			  0, 2, 1, 2,
			  GTK_FILL, 0, 0, 0);
	image = gtk_image_new_from_stock (GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (window->no_schema_label), image, FALSE, FALSE, 0);

	label = gtk_label_new (_("This key has no schema"));
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (window->no_schema_label), label, FALSE, FALSE, 0);


	label = gtk_label_new (_("Key name:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
	
	gtk_table_attach (GTK_TABLE (table), label,
			  0, 1, 2, 3,
			  GTK_FILL, 0, 0, 0);
	window->key_name_label = gtk_label_new (_("(None)"));
	gtk_label_set_selectable (GTK_LABEL (window->key_name_label), TRUE);
	gtk_misc_set_alignment (GTK_MISC (window->key_name_label), 0.0, 0.5);	
	gtk_table_attach (GTK_TABLE (table), window->key_name_label,
			  1, 2, 2, 3,
			  GTK_FILL, 0, 0, 0);
	
	label = gtk_label_new (_("Key owner:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);	
	gtk_table_attach (GTK_TABLE (table), label,
			  0, 1, 3, 4,
			  GTK_FILL, 0, 0, 0);
	window->owner_label= gtk_label_new (_("(None)"));
	gtk_label_set_selectable (GTK_LABEL (window->owner_label), TRUE);	
	gtk_misc_set_alignment (GTK_MISC (window->owner_label), 0.0, 0.5);		
	gtk_table_attach (GTK_TABLE (table), window->owner_label,
			  1, 2, 3, 4,
			  GTK_FILL, 0, 0, 0);

	label = gtk_label_new (_("Short description:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);	
	gtk_table_attach (GTK_TABLE (table), label,
			  0, 1, 4, 5,
			  GTK_FILL, 0, 0, 0);
	window->short_desc_label= gtk_label_new (_("(None)"));
	gtk_label_set_line_wrap (GTK_LABEL (window->short_desc_label), TRUE);
	gtk_label_set_selectable (GTK_LABEL (window->short_desc_label), TRUE);	
	gtk_misc_set_alignment (GTK_MISC (window->short_desc_label), 0.0, 0.5);		
	gtk_table_attach (GTK_TABLE (table), window->short_desc_label,
			  1, 2, 4, 5,
			  GTK_FILL, 0, 0, 0);

	label = gtk_label_new (_("Long description:"));
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.0);	
	gtk_table_attach (GTK_TABLE (table), label,
			  0, 1, 5, 6,
			  GTK_FILL, GTK_FILL, 0, 0);

	window->long_desc_buffer = gtk_text_buffer_new (NULL);
	gtk_text_buffer_set_text (window->long_desc_buffer, _("(None)"), -1);	
	text_view = gtk_text_view_new_with_buffer (window->long_desc_buffer);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (text_view), FALSE);

        gtk_style_context_get_background_color (gtk_widget_get_style_context (GTK_WIDGET (window)),
                                                0, &bg);
        gtk_widget_override_background_color (text_view, 0, &bg);

        gtk_style_context_get_background_color (gtk_widget_get_style_context (GTK_WIDGET (window)),
                                                GTK_STATE_FLAG_SELECTED, &bg);
        gtk_widget_override_background_color (text_view, GTK_STATE_FLAG_SELECTED, &bg);

        scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (scrolled_window), text_view);
	
	gtk_table_attach (GTK_TABLE (table), scrolled_window, 
			  1, 2, 5, 6,
			  GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);

	gtk_widget_show_all (vbox);
	gtk_widget_hide (window->non_writable_label);
	gtk_widget_hide (window->no_schema_label);
	gtk_widget_hide (window->output_window);
	
	gtk_window_set_default_icon_name ("gconf-editor");

	gconf_editor_window_setup_ui_prefs_handler (window);
}