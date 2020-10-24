void
gconf_bookmarks_hook_up_menu (GConfEditorWindow *window,
			      GtkWidget *menu,
			      GtkWidget *add_bookmark,
			      GtkWidget *edit_bookmarks)
{
	GConfClient *client;
	guint notify_id;

	g_object_set_data (G_OBJECT (menu), "editor-window", window);

	client = gconf_client_get_default ();
	
	/* Add a notify function */
	gconf_client_add_dir (client, "/apps/gconf-editor/bookmarks",
			      GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);
	notify_id = gconf_client_notify_add (client, "/apps/gconf-editor/bookmarks",
					     gconf_bookmarks_key_changed, menu, NULL, NULL);
	g_object_set_data_full (G_OBJECT (menu), "notify-id", GINT_TO_POINTER (notify_id),
				remove_notify_id);


	notify_id = gconf_client_notify_add (client, "/desktop/gnome/interface/menus_have_icons",
					     gconf_bookmarks_have_icons_notify, menu, NULL, NULL); 
	g_object_set_data_full (G_OBJECT (menu), "notify-id-x", GINT_TO_POINTER (notify_id),
				remove_notify_id);

	gconf_bookmarks_update_menu (menu);

        {
                gboolean have_icons;
                GConfValue *value;
                GError *err;

                err = NULL;
                value = gconf_client_get (client, "/desktop/gnome/interface/menus_have_icons", &err);

                if (err != NULL || value == NULL || value->type != GCONF_VALUE_BOOL)
                        return;

                have_icons = gconf_value_get_bool (value);
                gconf_bookmarks_set_have_icons (menu, have_icons);

                gconf_value_free (value);
        }

	if ( ! gconf_client_key_is_writable (client, "/apps/gconf-editor/bookmarks", NULL)) {
		gtk_widget_set_sensitive (add_bookmark, FALSE);
		gtk_widget_set_sensitive (edit_bookmarks, FALSE);
	}

	g_object_unref (client);
}