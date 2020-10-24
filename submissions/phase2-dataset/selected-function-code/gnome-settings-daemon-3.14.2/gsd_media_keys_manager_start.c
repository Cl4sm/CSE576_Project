gboolean
gsd_media_keys_manager_start (GsdMediaKeysManager *manager,
                              GError             **error)
{
        const char * const subsystems[] = { "input", "usb", "sound", NULL };

        gnome_settings_profile_start (NULL);

#ifdef HAVE_GUDEV
        manager->priv->streams = g_hash_table_new (g_direct_hash, g_direct_equal);
        manager->priv->udev_client = g_udev_client_new (subsystems);
#endif

        manager->priv->start_idle_id = g_idle_add ((GSourceFunc) start_media_keys_idle_cb, manager);
        g_source_set_name_by_id (manager->priv->start_idle_id, "[gnome-settings-daemon] start_media_keys_idle_cb");

        register_manager (manager_object);

        gnome_settings_profile_end (NULL);

        return TRUE;
}