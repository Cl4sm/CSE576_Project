static gboolean
start_media_keys_idle_cb (GsdMediaKeysManager *manager)
{
        g_debug ("Starting media_keys manager");
        gnome_settings_profile_start (NULL);

        manager->priv->keys = g_ptr_array_new_with_free_func ((GDestroyNotify) media_key_free);

        initialize_volume_handler (manager);

        manager->priv->settings = g_settings_new (SETTINGS_BINDING_DIR);
        g_signal_connect (G_OBJECT (manager->priv->settings), "changed",
                          G_CALLBACK (gsettings_changed_cb), manager);
        g_signal_connect (G_OBJECT (manager->priv->settings), "changed::custom-keybindings",
                          G_CALLBACK (gsettings_custom_changed_cb), manager);

        manager->priv->custom_settings =
          g_hash_table_new_full (g_str_hash, g_str_equal,
                                 g_free, g_object_unref);

        /* for the power plugin interface code */
        manager->priv->power_settings = g_settings_new (SETTINGS_POWER_DIR);

        /* Logic from http://git.gnome.org/browse/gnome-shell/tree/js/ui/status/accessibility.js#n163 */
        manager->priv->interface_settings = g_settings_new (SETTINGS_INTERFACE_DIR);
        g_signal_connect (G_OBJECT (manager->priv->interface_settings), "changed::gtk-theme",
			  G_CALLBACK (update_theme_settings), manager);
        g_signal_connect (G_OBJECT (manager->priv->interface_settings), "changed::icon-theme",
			  G_CALLBACK (update_theme_settings), manager);
	manager->priv->gtk_theme = g_settings_get_string (manager->priv->interface_settings, "gtk-theme");
	if (g_str_equal (manager->priv->gtk_theme, HIGH_CONTRAST)) {
		g_free (manager->priv->gtk_theme);
		manager->priv->gtk_theme = NULL;
	}
	manager->priv->icon_theme = g_settings_get_string (manager->priv->interface_settings, "icon-theme");

        ensure_cancellable (&manager->priv->grab_cancellable);
        ensure_cancellable (&manager->priv->screencast_cancellable);

        manager->priv->shell_proxy = gnome_settings_bus_get_shell_proxy ();
        g_signal_connect_swapped (manager->priv->shell_proxy, "notify::g-name-owner",
                                  G_CALLBACK (shell_presence_changed), manager);
        shell_presence_changed (manager);

        g_debug ("Starting mpris controller");
        manager->priv->mpris_controller = mpris_controller_new ();

        /* Rotation */
        manager->priv->orientation_watch_id = g_bus_watch_name (G_BUS_TYPE_SESSION,
                                                                "org.gnome.SettingsDaemon.Orientation",
                                                                G_BUS_NAME_WATCHER_FLAGS_NONE,
                                                                orientation_appeared_cb,
                                                                orientation_disappeared_cb,
                                                                manager, NULL);

        gnome_settings_profile_end (NULL);

        manager->priv->start_idle_id = 0;

        return FALSE;
}