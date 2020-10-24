void
gsd_ldsm_setup (gboolean check_now)
{
        if (ldsm_notified_hash || ldsm_timeout_id || ldsm_monitor) {
                g_warning ("Low disk space monitor already initialized.");
                return;
        }

        ldsm_notified_hash = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                    g_free,
                                                    ldsm_free_mount_info);

        settings = g_settings_new (SETTINGS_HOUSEKEEPING_DIR);
        privacy_settings = g_settings_new (PRIVACY_SETTINGS);
        gsd_ldsm_get_config ();
        g_signal_connect (G_OBJECT (settings), "changed",
                          G_CALLBACK (gsd_ldsm_update_config), NULL);

        ldsm_monitor = g_unix_mount_monitor_new ();
        g_unix_mount_monitor_set_rate_limit (ldsm_monitor, 1000);
        g_signal_connect (ldsm_monitor, "mounts-changed",
                          G_CALLBACK (ldsm_mounts_changed), NULL);

        if (check_now)
                ldsm_check_all_mounts (NULL);

        ldsm_timeout_id = g_timeout_add_seconds (CHECK_EVERY_X_SECONDS,
                                                 ldsm_check_all_mounts, NULL);
        g_source_set_name_by_id (ldsm_timeout_id, "[gnome-settings-daemon] ldsm_check_all_mounts");

        purge_trash_id = g_timeout_add_seconds (3600, ldsm_purge_trash_and_temp, NULL);
        g_source_set_name_by_id (purge_trash_id, "[gnome-settings-daemon] ldsm_purge_trash_and_temp");
}