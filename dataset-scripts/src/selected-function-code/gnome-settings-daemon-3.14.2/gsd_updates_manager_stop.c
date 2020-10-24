void
gsd_updates_manager_stop (GsdUpdatesManager *manager)
{
        g_debug ("Stopping updates manager");

        g_clear_object (&manager->priv->settings_proxy);
        g_clear_object (&manager->priv->settings_http);
        g_clear_object (&manager->priv->settings_ftp);
        g_clear_object (&manager->priv->settings_gsd);
        g_clear_object (&manager->priv->control);
        g_clear_object (&manager->priv->task);
        g_clear_object (&manager->priv->refresh);
        g_clear_object (&manager->priv->firmware);
        g_clear_object (&manager->priv->proxy_session);
        g_clear_object (&manager->priv->volume_monitor);
        if (manager->priv->cancellable) {
                g_cancellable_cancel (manager->priv->cancellable);
                g_clear_object (&manager->priv->cancellable);
        }
        if (manager->priv->update_viewer_watcher_id != 0) {
                g_bus_unwatch_name (manager->priv->update_viewer_watcher_id);
                manager->priv->update_viewer_watcher_id = 0;
        }
        if (manager->priv->offline_update_id) {
                g_source_remove (manager->priv->offline_update_id);
                manager->priv->offline_update_id = 0;
        }
        if (manager->priv->update_packages != NULL) {
                g_ptr_array_unref (manager->priv->update_packages);
                manager->priv->update_packages = NULL;
        }
        g_clear_object (&manager->priv->offline_update_error);
}