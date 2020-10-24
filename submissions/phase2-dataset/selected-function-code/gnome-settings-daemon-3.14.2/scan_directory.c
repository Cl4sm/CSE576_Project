static void
scan_directory (GsdUpdatesFirmware *firmware)
{
        gboolean ret;
        GError *error = NULL;
        GDir *dir;
        const gchar *filename;
        gchar *filename_decoded;
        guint i;
        GPtrArray *array;
        const GsdUpdatesFirmwareRequest *req;
        guint scan_id = 0;

        /* should we check and show the user */
        ret = g_settings_get_boolean (firmware->priv->settings,
                                      GSD_SETTINGS_ENABLE_CHECK_FIRMWARE);
        if (!ret) {
                g_debug ("not showing thanks to GSettings");
                return;
        }

        /* open the directory of requests */
        dir = g_dir_open (GSD_UPDATES_FIRMWARE_MISSING_DIR, 0, &error);
        if (dir == NULL) {
                if (error->code != G_FILE_ERROR_NOENT) {
                        g_warning ("failed to open directory: %s",
                                   error->message);
                }
                g_error_free (error);
                return;
        }

        /* find all the firmware requests */
        filename = g_dir_read_name (dir);
        while (filename != NULL) {

                filename_decoded = udev_text_decode (filename);
                add_filename (firmware, filename_decoded);
                g_free (filename_decoded);

                /* next file */
                filename = g_dir_read_name (dir);
        }
        g_dir_close (dir);

        /* debugging */
        array = firmware->priv->array_requested;
        for (i=0; i<array->len; i++) {
                req = g_ptr_array_index (array, i);
                g_debug ("requested: %s", req->filename);
        }

        /* remove banned files */
        remove_banned (firmware, array);

        /* remove ignored devices */
        remove_ignored (firmware, array);

        /* debugging */
        array = firmware->priv->array_requested;
        for (i=0; i<array->len; i++) {
                req = g_ptr_array_index (array, i);
                g_debug ("searching for: %s", req->filename);
        }

        /* don't spam the user at startup, so wait a little delay */
        if (array->len > 0) {
                scan_id = g_timeout_add_seconds (GSD_UPDATES_FIRMWARE_PROCESS_DELAY,
                                                 delay_timeout_cb,
                                                 firmware);
                g_source_set_name_by_id (scan_id, "[GsdUpdatesFirmware] process");
        }
}