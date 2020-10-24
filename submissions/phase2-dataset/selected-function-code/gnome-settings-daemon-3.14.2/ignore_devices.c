static void
ignore_devices (GsdUpdatesFirmware *firmware)
{
        gchar *existing = NULL;
        GsdUpdatesFirmwareRequest *req;
        GPtrArray *array;
        GString *string;
        guint i;

        /* get from settings */
        existing = g_settings_get_string (firmware->priv->settings,
                                          GSD_SETTINGS_IGNORED_DEVICES);

        /* get existing string */
        string = g_string_new (existing);
        if (string->len > 0)
                g_string_append (string, ",");

        /* add all listed devices */
        array = firmware->priv->array_requested;
        for (i=0; i<array->len; i++) {
                req = g_ptr_array_index (array, i);
                g_string_append_printf (string, "%s,", req->id);
        }

        /* remove final ',' */
        if (string->len > 2)
                g_string_set_size (string, string->len - 1);

        /* set new string */
        g_settings_set_string (firmware->priv->settings,
                               GSD_SETTINGS_IGNORED_DEVICES,
                               string->str);

        g_free (existing);
        g_string_free (string, TRUE);
}