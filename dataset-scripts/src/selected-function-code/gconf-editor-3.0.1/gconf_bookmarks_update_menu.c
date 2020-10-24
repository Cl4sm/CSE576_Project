static void
gconf_bookmarks_update_menu (GtkWidget *menu)
{
	GSList *list, *tmp;
	GtkWidget *menuitem, *window;
	GConfClient *client;

	window = g_object_get_data (G_OBJECT (menu), "editor-window");
	client = gconf_client_get_default ();
	
	/* Get the old list and then set it */
	list = gconf_client_get_list (client,
				     "/apps/gconf-editor/bookmarks", GCONF_VALUE_STRING, NULL);

	if (list != NULL) {
		menuitem = gtk_separator_menu_item_new ();
		gtk_widget_show (menuitem);
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
	}

	for (tmp = list; tmp; tmp = tmp->next) {
		menuitem = gtk_image_menu_item_new_with_label (tmp->data);
		g_signal_connect (menuitem, "activate",
				  G_CALLBACK (gconf_bookmarks_bookmark_activated), window);
		g_object_set_data_full (G_OBJECT (menuitem), "gconf-key", g_strdup (tmp->data), g_free);
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menuitem), gtk_image_new_from_stock (STOCK_BOOKMARK,
													 GTK_ICON_SIZE_MENU));
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
		gtk_widget_show_all (menuitem);

		g_free (tmp->data);
	}

	g_object_unref (client);
	g_slist_free (list);
}