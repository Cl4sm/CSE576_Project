static void
gconf_bookmarks_key_changed (GConfClient *client, guint cnxn_id, GConfEntry *entry, gpointer user_data)
{
	GList *child_list, *tmp;
	GtkWidget *menu_item;
	
	child_list = gtk_container_get_children (GTK_CONTAINER (user_data));

	for (tmp = child_list; tmp; tmp = tmp->next) {
		menu_item = tmp->data;

		if (g_object_get_data (G_OBJECT (menu_item), "gconf-key") != NULL ||
			GTK_IS_SEPARATOR_MENU_ITEM (menu_item)) {
			gtk_widget_destroy (menu_item);
		}
	}

	gconf_bookmarks_update_menu (GTK_WIDGET (user_data));
	
	g_list_free (child_list);
}