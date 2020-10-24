static gboolean
gsd_orientation_manager_idle_cb (GsdOrientationManager *manager)
{
        const char * const subsystems[] = { "input", NULL };

        gnome_settings_profile_start (NULL);

        manager->priv->start_idle_id = 0;
        manager->priv->settings = g_settings_new (CONF_SCHEMA);
        manager->priv->orientation_lock = g_settings_get_boolean (manager->priv->settings, ORIENTATION_LOCK_KEY);
        g_signal_connect (G_OBJECT (manager->priv->settings), "changed::orientation-lock",
                          G_CALLBACK (orientation_lock_changed_cb), manager);

        manager->priv->client = g_udev_client_new (subsystems);
        update_accelerometer (manager);

        /* D-Bus setup */
        g_bus_get (G_BUS_TYPE_SESSION,
                   NULL,
                   (GAsyncReadyCallback) on_bus_gotten,
                   manager);

        g_signal_connect (G_OBJECT (manager->priv->client), "uevent",
                          G_CALLBACK (client_uevent_cb), manager);

        gnome_settings_profile_end (NULL);

        return G_SOURCE_REMOVE;
}