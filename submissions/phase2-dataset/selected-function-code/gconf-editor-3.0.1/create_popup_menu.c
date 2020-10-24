static GtkWidget *
create_popup_menu (GeditOutputWindow  *output_window)
{
	GtkWidget *menu;
	GtkWidget *menu_item;
	
	menu = gtk_menu_new ();

	/* Add the clear button */
	output_window->priv->clear_menu_item = gtk_image_menu_item_new_from_stock (GTK_STOCK_CLEAR, NULL);
	gtk_widget_show (output_window->priv->clear_menu_item);
	g_signal_connect (G_OBJECT (output_window->priv->clear_menu_item), "activate",
		      	  G_CALLBACK (clear_clicked_callback), output_window);
	gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), output_window->priv->clear_menu_item);

	/* Add the copy button */
	output_window->priv->copy_menu_item = gtk_image_menu_item_new_from_stock (GTK_STOCK_COPY, NULL);
	gtk_widget_show (output_window->priv->copy_menu_item);
	g_signal_connect (G_OBJECT (output_window->priv->copy_menu_item), "activate",
		      	  G_CALLBACK (copy_clicked_callback), output_window);
	gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), output_window->priv->copy_menu_item);

	/* Add the separator */
	menu_item = gtk_separator_menu_item_new ();
	gtk_widget_show (menu_item);
	gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), menu_item);

	/* Add the close button */
	output_window->priv->close_menu_item = gtk_image_menu_item_new_from_stock (GTK_STOCK_CLOSE, NULL);
	gtk_widget_show (output_window->priv->close_menu_item);
	g_signal_connect (G_OBJECT (output_window->priv->close_menu_item), "activate",
		      	  G_CALLBACK (close_clicked_callback), output_window);
	gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), output_window->priv->close_menu_item);
	
	gtk_widget_set_sensitive (output_window->priv->copy_menu_item, FALSE);
	gtk_widget_set_sensitive (output_window->priv->clear_menu_item, FALSE);

	return menu;
}