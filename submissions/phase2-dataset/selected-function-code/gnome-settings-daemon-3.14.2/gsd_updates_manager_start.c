gboolean
gsd_updates_manager_start (GsdUpdatesManager *manager,
                           GError **error)
{
        gboolean ret = FALSE;

        g_debug ("Starting updates manager");

        /* use PackageKit */
        manager->priv->cancellable = g_cancellable_new ();
        manager->priv->control = pk_control_new ();
        g_signal_connect (manager->priv->control, "notify::locked",
                          G_CALLBACK (notify_locked_cb), manager);
        manager->priv->task = pk_task_new ();
        g_object_set (manager->priv->task,
                      "background", TRUE,
                      "interactive", FALSE,
                      "only-download", TRUE,
                      NULL);

        /* watch UDev for missing firmware */
        manager->priv->firmware = gsd_updates_firmware_new ();

        /* get automatic callbacks about when we should check for
         * updates, refresh-caches and upgrades */
        manager->priv->refresh = gsd_updates_refresh_new ();
        g_signal_connect (manager->priv->refresh, "get-upgrades",
                          G_CALLBACK (due_get_upgrades_cb), manager);
        g_signal_connect (manager->priv->refresh, "refresh-cache",
                          G_CALLBACK (due_refresh_cache_cb), manager);
        /* gnome-software takes over software updates if it's installed */
        if (!does_gnome_software_exist ()) {
                g_signal_connect (manager->priv->refresh, "get-updates",
                                  G_CALLBACK (due_get_updates_cb), manager);
        }

        /* get proxy settings */
        manager->priv->settings_proxy = g_settings_new ("org.gnome.system.proxy");
        g_signal_connect (manager->priv->settings_proxy, "changed",
                          G_CALLBACK (settings_changed_cb), manager);

        /* get http settings */
        manager->priv->settings_http = g_settings_new ("org.gnome.system.proxy.http");
        g_signal_connect (manager->priv->settings_http, "changed",
                          G_CALLBACK (settings_changed_cb), manager);

        /* get ftp settings */
        manager->priv->settings_ftp = g_settings_new ("org.gnome.system.proxy.ftp");
        g_signal_connect (manager->priv->settings_ftp, "changed",
                          G_CALLBACK (settings_changed_cb), manager);

        /* get ftp settings */
        manager->priv->settings_gsd = g_settings_new ("org.gnome.settings-daemon.plugins.updates");
        g_signal_connect (manager->priv->settings_gsd, "changed",
                          G_CALLBACK (settings_gsd_changed_cb), manager);

        /* use gnome-session for the idle detection */
        manager->priv->proxy_session =
                gnome_settings_bus_get_session_proxy ();
        if (manager->priv->proxy_session == NULL)
                goto out;

        /* if the update viewer is started, then hide the notification */
        manager->priv->update_viewer_watcher_id =
                g_bus_watch_name (G_BUS_TYPE_SESSION,
                                  "org.freedesktop.PackageKit.UpdateViewer",
                                  G_BUS_NAME_WATCHER_FLAGS_NONE,
                                  update_viewer_appeared_cb,
                                  NULL,
                                  manager,
                                  NULL);

        /* get a volume monitor so we can watch media */
        manager->priv->volume_monitor = g_volume_monitor_get ();
        g_signal_connect (manager->priv->volume_monitor, "mount-added",
                          G_CALLBACK (mount_added_cb), manager);

        /* coldplug */
        reload_proxy_settings (manager);

        /* check for offline update */
        if (!does_gnome_software_exist ()) {
                manager->priv->offline_update_id =
                        g_timeout_add_seconds (GSD_UPDATES_CHECK_OFFLINE_TIMEOUT,
                                               check_offline_update_cb,
                                               manager);
        }

        /* success */
        ret = TRUE;
        g_debug ("Started updates manager");
out:
        return ret;
}