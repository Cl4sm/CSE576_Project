static gboolean
gsd_mouse_manager_idle_cb (GsdMouseManager *manager)
{
        GList *devices, *l;

        gnome_settings_profile_start (NULL);

        set_devicepresence_handler (manager);

        manager->priv->mouse_settings = g_settings_new (SETTINGS_MOUSE_DIR);
        g_signal_connect (manager->priv->mouse_settings, "changed",
                          G_CALLBACK (mouse_callback), manager);

        manager->priv->mouse_a11y_settings = g_settings_new ("org.gnome.desktop.a11y.mouse");
        g_signal_connect (manager->priv->mouse_a11y_settings, "changed",
                          G_CALLBACK (mouse_callback), manager);

        manager->priv->touchpad_settings = g_settings_new (SETTINGS_TOUCHPAD_DIR);
        g_signal_connect (manager->priv->touchpad_settings, "changed",
                          G_CALLBACK (touchpad_callback), manager);

        manager->priv->syndaemon_spawned = FALSE;

        set_locate_pointer (manager, g_settings_get_boolean (manager->priv->mouse_settings, KEY_LOCATE_POINTER));
        set_mousetweaks_daemon (manager,
                                g_settings_get_boolean (manager->priv->mouse_a11y_settings, KEY_DWELL_CLICK_ENABLED),
                                g_settings_get_boolean (manager->priv->mouse_a11y_settings, KEY_SECONDARY_CLICK_ENABLED));
        set_disable_w_typing (manager, g_settings_get_boolean (manager->priv->touchpad_settings, KEY_TOUCHPAD_DISABLE_W_TYPING));

        devices = gdk_device_manager_list_devices (manager->priv->device_manager, GDK_DEVICE_TYPE_SLAVE);
        for (l = devices; l != NULL; l = l->next) {
                GdkDevice *device = l->data;

                if (device_is_ignored (manager, device))
                        continue;

                if (run_custom_command (device, COMMAND_DEVICE_PRESENT) == FALSE) {
                        set_mouse_settings (manager, device);
                } else {
                        int id;
                        g_object_get (G_OBJECT (device), "device-id", &id, NULL);
                        g_hash_table_insert (manager->priv->blacklist,
                                             GINT_TO_POINTER (id), GINT_TO_POINTER (1));
                }
        }
        g_list_free (devices);

        ensure_touchpad_active (manager);

        if (g_settings_get_boolean (manager->priv->touchpad_settings, KEY_TOUCHPAD_ENABLED)) {
                devices = get_disabled_devices (manager->priv->device_manager);
                for (l = devices; l != NULL; l = l->next) {
                        int device_id;

                        device_id = GPOINTER_TO_INT (l->data);
                        set_touchpad_enabled (device_id);
                }
                g_list_free (devices);
        }

        gnome_settings_profile_end (NULL);

        manager->priv->start_idle_id = 0;

        return FALSE;
}