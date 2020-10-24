static void
gsd_media_keys_manager_finalize (GObject *object)
{
        GsdMediaKeysManager *media_keys_manager;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GSD_IS_MEDIA_KEYS_MANAGER (object));

        media_keys_manager = GSD_MEDIA_KEYS_MANAGER (object);

        g_return_if_fail (media_keys_manager->priv != NULL);

        gsd_media_keys_manager_stop (media_keys_manager);

        if (media_keys_manager->priv->inhibit_keys_fd != -1)
                close (media_keys_manager->priv->inhibit_keys_fd);

        g_clear_object (&media_keys_manager->priv->logind_proxy);
        g_clear_object (&media_keys_manager->priv->screen_saver_proxy);

        G_OBJECT_CLASS (gsd_media_keys_manager_parent_class)->finalize (object);
}