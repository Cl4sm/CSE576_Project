static gboolean
device_rebind (GsdUpdatesFirmware *firmware)
{
        gboolean ret;
        gchar *argv[4];
        gchar *rebind_stderr = NULL;
        gchar *rebind_stdout = NULL;
        GError *error = NULL;
        gint exit_status = 0;
        guint i;
        GPtrArray *array;
        const GsdUpdatesFirmwareRequest *req;
        GString *string;

        string = g_string_new ("");

        /* make a string array of all the devices to replug */
        array = firmware->priv->array_requested;
        for (i=0; i<array->len; i++) {
                req = g_ptr_array_index (array, i);
                g_string_append_printf (string, "%s ", req->sysfs_path);
        }

        /* remove trailing space */
        if (string->len > 0)
                g_string_set_size (string, string->len-1);

        /* use PolicyKit to do this as root */
        argv[0] = "pkexec";
        argv[1] = GSD_UPDATES_FIRMWARE_DEVICE_REBIND_PROGRAM;
        argv[2] = string->str;
        argv[3] = NULL;
        ret = g_spawn_sync (NULL,
                            argv,
                            NULL,
                            G_SPAWN_SEARCH_PATH,
                            NULL, NULL,
                            &rebind_stdout,
                            &rebind_stderr,
                            &exit_status,
                            &error);
        if (!ret) {
                g_warning ("failed to spawn '%s': %s",
                           argv[1], error->message);
                g_error_free (error);
                goto out;
        }

        /* if we failed to rebind the device */
        if (exit_status != 0) {
                g_warning ("failed to rebind: %s, %s",
                           rebind_stdout, rebind_stderr);
                ret = FALSE;
                goto out;
        }
out:
        g_free (rebind_stdout);
        g_free (rebind_stderr);
        g_string_free (string, TRUE);
        return ret;
}