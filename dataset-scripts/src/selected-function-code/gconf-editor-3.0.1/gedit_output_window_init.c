static void 
gedit_output_window_init (GeditOutputWindow  *output_window)
{
	GtkSettings 		*settings;
	gint 			 w, h;
	GtkWidget 		*vbox1;
	GtkWidget 		*image;
	GtkWidget 		*hbox2;
	GtkWidget		*vseparator;
	GtkWidget 		*vbox2;
	GtkWidget		*scrolledwindow;
	GtkTreeViewColumn	*column;
	GtkCellRenderer 	*cell;
	GtkTreeSelection 	*selection;
	GtkWidget 		*popup_menu;

	GList			*focusable_widgets = NULL;

	output_window->priv = g_new0 (GeditOutputWindowPrivate, 1);

	settings = gtk_widget_get_settings (GTK_WIDGET (output_window));

	gtk_icon_size_lookup_for_settings (settings,
					   GTK_ICON_SIZE_MENU,
					   &w, &h);

	vbox1 = gtk_vbox_new (FALSE, 2);
	gtk_container_set_border_width (GTK_CONTAINER (vbox1), 2);

	/* Create the close button */
	output_window->priv->close_button = gtk_button_new ();
	gtk_box_pack_start (GTK_BOX (vbox1), output_window->priv->close_button, FALSE, FALSE, 0);

	gtk_widget_set_tooltip_text (output_window->priv->close_button,
				     _("Close the output window"));

	gedit_output_window_format_button (GTK_BUTTON (output_window->priv->close_button), w, h);

	image = gtk_image_new_from_stock (GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  	gtk_container_add (GTK_CONTAINER (output_window->priv->close_button), image);
	
	g_signal_connect (output_window->priv->close_button, 
			  "clicked",
			  G_CALLBACK (close_clicked_callback),
			  output_window);

	/* Create the 3 vertical separators */
	hbox2 = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox2, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox2), 4);

	vseparator = gtk_vseparator_new ();
	gtk_box_pack_start (GTK_BOX (hbox2), vseparator, FALSE, FALSE, 0);

	vseparator = gtk_vseparator_new ();
	gtk_box_pack_start (GTK_BOX (hbox2), vseparator, FALSE, FALSE, 0);

	vseparator = gtk_vseparator_new ();
  	gtk_box_pack_start (GTK_BOX (hbox2), vseparator, FALSE, TRUE, 0);

	/* Create the vbox for the copy and clear buttons */
	vbox2 = gtk_vbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox1), vbox2, FALSE, TRUE, 0);

	/* Create the copy button */
	output_window->priv->copy_button = gtk_button_new ();
	gtk_box_pack_start (GTK_BOX (vbox2), output_window->priv->copy_button, FALSE, FALSE, 0);

	gtk_widget_set_tooltip_text (output_window->priv->copy_button, 
				     _("Copy selected lines"));

	gedit_output_window_format_button (GTK_BUTTON (output_window->priv->copy_button), w, h);

	image = gtk_image_new_from_stock (GTK_STOCK_COPY, GTK_ICON_SIZE_MENU);
  	gtk_container_add (GTK_CONTAINER (output_window->priv->copy_button), image);

	g_signal_connect (output_window->priv->copy_button, 
			  "clicked",
			  G_CALLBACK (copy_clicked_callback),
			  output_window);

	/* Create the clear button */
	output_window->priv->clear_button = gtk_button_new ();
	gtk_box_pack_start (GTK_BOX (vbox2), output_window->priv->clear_button, FALSE, FALSE, 0);

	gtk_widget_set_tooltip_text (output_window->priv->clear_button, 
				     _("Clear the output window"));
	
	gedit_output_window_format_button (GTK_BUTTON (output_window->priv->clear_button), w, h);

	image = gtk_image_new_from_stock (GTK_STOCK_CLEAR, GTK_ICON_SIZE_MENU);
	gtk_container_add (GTK_CONTAINER (output_window->priv->clear_button), image);

	g_signal_connect (output_window->priv->clear_button, 
			  "clicked",
			  G_CALLBACK (clear_clicked_callback),
			  output_window);

	gtk_widget_set_sensitive (output_window->priv->copy_button, FALSE);
	gtk_widget_set_sensitive (output_window->priv->clear_button, FALSE);

  	/* Create the scrolled window */
	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
  	
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), 
					GTK_POLICY_AUTOMATIC, 
					GTK_POLICY_AUTOMATIC);
	
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), 
					     GTK_SHADOW_ETCHED_IN);

	output_window->priv->treeview = gtk_tree_view_new ();
  	gtk_container_add (GTK_CONTAINER (scrolledwindow), output_window->priv->treeview);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (output_window->priv->treeview), FALSE);


	/* List */
	output_window->priv->model = GTK_TREE_MODEL (
			gtk_list_store_new (NUM_COLUMNS, G_TYPE_STRING));

	gtk_tree_view_set_model (GTK_TREE_VIEW (output_window->priv->treeview), 
				 output_window->priv->model);

	/* Add the suggestions column */
	cell = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (_("Output Lines"), cell, 
			"markup", COLUMN_LINES, NULL);

	gtk_tree_view_append_column (GTK_TREE_VIEW (output_window->priv->treeview), column);

	gtk_tree_view_set_search_column (GTK_TREE_VIEW (output_window->priv->treeview),
			COLUMN_LINES);

	selection = gtk_tree_view_get_selection (
			GTK_TREE_VIEW (output_window->priv->treeview));

	gtk_tree_selection_set_mode (selection, GTK_SELECTION_MULTIPLE);
	
	gtk_box_pack_end (GTK_BOX (output_window), scrolledwindow, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (output_window), vbox1, FALSE, FALSE, 0);

	gtk_widget_set_size_request (GTK_WIDGET (output_window), 3 * w, 5 * (h + 2));

	g_signal_connect (G_OBJECT (output_window->priv->treeview), "key_press_event",
			  G_CALLBACK (gedit_output_window_key_press_event_cb), output_window);

	g_signal_connect (G_OBJECT (selection), "changed", 
			  G_CALLBACK (gedit_output_window_treeview_selection_changed), 
			  output_window);

	focusable_widgets = g_list_append (focusable_widgets, output_window->priv->treeview);
	focusable_widgets = g_list_append (focusable_widgets, output_window->priv->close_button);
	focusable_widgets = g_list_append (focusable_widgets, output_window->priv->copy_button);
	focusable_widgets = g_list_append (focusable_widgets, output_window->priv->clear_button);

	gtk_container_set_focus_chain (GTK_CONTAINER (output_window), focusable_widgets);

	g_list_free (focusable_widgets);

	popup_menu = create_popup_menu (output_window);
		
	gtk_menu_attach_to_widget(GTK_MENU(popup_menu), output_window->priv->treeview, NULL);
	
	g_signal_connect_swapped (output_window->priv->treeview, "button_press_event",	G_CALLBACK (my_popup_handler), popup_menu);
	
			  
}