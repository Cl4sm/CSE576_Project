static void
gsd_updates_firmware_init (GsdUpdatesFirmware *firmware)
{
        GFile *file;
        GError *error = NULL;

        firmware->priv = GSD_UPDATES_FIRMWARE_GET_PRIVATE (firmware);
        firmware->priv->timeout_id = 0;
        firmware->priv->packages_found = g_ptr_array_new_with_free_func ((GDestroyNotify) g_object_unref);
        firmware->priv->array_requested = g_ptr_array_new_with_free_func ((GDestroyNotify) request_free);
        firmware->priv->settings = g_settings_new (GSD_SETTINGS_SCHEMA);
        firmware->priv->task = pk_task_new ();
        g_object_set (firmware->priv->task,
                      "background", TRUE,
                      NULL);

        /* setup watch for new hardware */
        file = g_file_new_for_path (GSD_UPDATES_FIRMWARE_MISSING_DIR);
        firmware->priv->monitor = g_file_monitor (file,
                                                  G_FILE_MONITOR_NONE,
                                                  NULL,
                                                  &error);
        if (firmware->priv->monitor == NULL) {
                g_warning ("failed to setup monitor: %s", error->message);
                g_error_free (error);
                goto out;
        }

        /* limit to one per second */
        g_file_monitor_set_rate_limit (firmware->priv->monitor, 1000);

        /* get notified of changes */
        g_signal_connect (firmware->priv->monitor, "changed",
                          G_CALLBACK (monitor_changed_cb), firmware);
out:
        g_object_unref (file);
        firmware->priv->timeout_id =
                g_timeout_add_seconds (GSD_UPDATES_FIRMWARE_LOGIN_DELAY,
                                       (GSourceFunc) scan_directory_cb,
                                       firmware);
        g_source_set_name_by_id (firmware->priv->timeout_id,
                                 "[GsdUpdatesFirmware] login coldplug");
}