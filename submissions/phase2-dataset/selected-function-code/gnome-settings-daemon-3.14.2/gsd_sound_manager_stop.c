void
gsd_sound_manager_stop (GsdSoundManager *manager)
{
        g_debug ("Stopping sound manager");

        if (manager->priv->settings != NULL) {
                g_object_unref (manager->priv->settings);
                manager->priv->settings = NULL;
        }

        if (manager->priv->timeout) {
                g_source_remove (manager->priv->timeout);
                manager->priv->timeout = 0;
        }

        while (manager->priv->monitors) {
                g_file_monitor_cancel (G_FILE_MONITOR (manager->priv->monitors->data));
                g_object_unref (manager->priv->monitors->data);
                manager->priv->monitors = g_list_delete_link (manager->priv->monitors, manager->priv->monitors);
        }
}