void
gsd_media_keys_manager_stop (GsdMediaKeysManager *manager)
{
        GsdMediaKeysManagerPrivate *priv = manager->priv;
        int i;

        g_debug ("Stopping media_keys manager");

        if (priv->start_idle_id != 0) {
                g_source_remove (priv->start_idle_id);
                priv->start_idle_id = 0;
        }

        if (priv->bus_cancellable != NULL) {
                g_cancellable_cancel (priv->bus_cancellable);
                g_object_unref (priv->bus_cancellable);
                priv->bus_cancellable = NULL;
        }

        if (manager->priv->gtksettings != NULL) {
                g_signal_handlers_disconnect_by_func (manager->priv->gtksettings, sound_theme_changed, manager);
                manager->priv->gtksettings = NULL;
        }

        if (manager->priv->orientation_watch_id > 0) {
                g_bus_unwatch_name (manager->priv->orientation_watch_id);
                manager->priv->orientation_watch_id = 0;
        }

        g_clear_pointer (&manager->priv->ca, ca_context_destroy);

#ifdef HAVE_GUDEV
        g_clear_pointer (&priv->streams, g_hash_table_destroy);
        g_clear_object (&priv->udev_client);
#endif /* HAVE_GUDEV */

        g_clear_object (&priv->logind_proxy);
        g_clear_object (&priv->settings);
        g_clear_object (&priv->power_settings);
        g_clear_object (&priv->power_proxy);
        g_clear_object (&priv->power_screen_proxy);
        g_clear_object (&priv->power_keyboard_proxy);
        g_clear_object (&priv->composite_device);
        g_clear_object (&priv->mpris_controller);
        g_clear_object (&priv->screencast_proxy);

        if (priv->cancellable != NULL) {
                g_cancellable_cancel (priv->cancellable);
                g_clear_object (&priv->cancellable);
        }

        g_clear_pointer (&priv->introspection_data, g_dbus_node_info_unref);
        g_clear_object (&priv->connection);

        if (priv->keys != NULL) {
                for (i = 0; i < priv->keys->len; ++i) {
                        MediaKey *key;

                        key = g_ptr_array_index (manager->priv->keys, i);
                        ungrab_media_key (key, manager);
                }
                g_ptr_array_free (priv->keys, TRUE);
                priv->keys = NULL;
        }

        g_clear_object (&priv->key_grabber);

        if (priv->grab_cancellable != NULL) {
                g_cancellable_cancel (priv->grab_cancellable);
                g_clear_object (&priv->grab_cancellable);
        }

        if (priv->screencast_cancellable != NULL) {
                g_cancellable_cancel (priv->screencast_cancellable);
                g_clear_object (&priv->screencast_cancellable);
        }

        g_clear_object (&priv->sink);
        g_clear_object (&priv->source);
        g_clear_object (&priv->volume);

        if (priv->media_players != NULL) {
                g_list_free_full (priv->media_players, (GDestroyNotify) free_media_player);
                priv->media_players = NULL;
        }

        g_clear_object (&priv->shell_proxy);
}