static gboolean
start_keyboard_idle_cb (GsdKeyboardManager *manager)
{
        gnome_settings_profile_start (NULL);

        g_debug ("Starting keyboard manager");

        manager->priv->settings = g_settings_new (GSD_KEYBOARD_DIR);

	xkb_init (manager);

	set_devicepresence_handler (manager);

        manager->priv->input_sources_settings = g_settings_new (GNOME_DESKTOP_INPUT_SOURCES_DIR);
        g_signal_connect (manager->priv->input_sources_settings, "changed::"KEY_INPUT_SOURCES,
                          G_CALLBACK (input_sources_changed), manager);

        manager->priv->cancellable = g_cancellable_new ();

        g_dbus_proxy_new_for_bus (G_BUS_TYPE_SYSTEM,
                                  G_DBUS_PROXY_FLAGS_NONE,
                                  NULL,
                                  "org.freedesktop.locale1",
                                  "/org/freedesktop/locale1",
                                  "org.freedesktop.locale1",
                                  manager->priv->cancellable,
                                  localed_proxy_ready,
                                  manager);

        /* apply current settings before we install the callback */
        g_debug ("Started the keyboard plugin, applying all settings");
        apply_all_settings (manager);

        g_signal_connect (G_OBJECT (manager->priv->settings), "changed",
                          G_CALLBACK (settings_changed), manager);

	install_xkb_filter (manager);

        gnome_settings_profile_end (NULL);

        manager->priv->start_idle_id = 0;

        return FALSE;
}