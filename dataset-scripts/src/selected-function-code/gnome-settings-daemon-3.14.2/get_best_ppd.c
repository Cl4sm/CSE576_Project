static gchar *
get_best_ppd (gchar *device_id,
              gchar *device_make_and_model,
              gchar *device_uri)
{
        GDBusProxy  *proxy;
        GVariant    *output;
        GVariant    *array;
        GVariant    *tuple;
        GError      *error = NULL;
        gchar       *ppd_name = NULL;
        gint         i, j;
        static const char * const match_levels[] = {
                   "exact-cmd",
                   "exact",
                   "close",
                   "generic",
                   "none"};

        proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                               G_DBUS_PROXY_FLAGS_NONE,
                                               NULL,
                                               SCP_BUS,
                                               SCP_PATH,
                                               SCP_IFACE,
                                               NULL,
                                               &error);

        if (!proxy) {
                g_warning ("%s", error->message);
                g_error_free (error);
                return NULL;
        }

        output = g_dbus_proxy_call_sync (proxy,
                                         "GetBestDrivers",
                                         g_variant_new ("(sss)",
                                                 device_id ? device_id : "",
                                                 device_make_and_model ? device_make_and_model : "",
                                                 device_uri ? device_uri : ""),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         DBUS_TIMEOUT,
                                         NULL,
                                         &error);

        if (output && g_variant_n_children (output) >= 1) {
                array = g_variant_get_child_value (output, 0);
                if (array)
                        for (j = 0; j < G_N_ELEMENTS (match_levels) && ppd_name == NULL; j++)
                                for (i = 0; i < g_variant_n_children (array) && ppd_name == NULL; i++) {
                                        tuple = g_variant_get_child_value (array, i);
                                        if (tuple && g_variant_n_children (tuple) == 2) {
                                                if (g_strcmp0 (g_variant_get_string (
                                                                   g_variant_get_child_value (tuple, 1),
                                                                   NULL), match_levels[j]) == 0)
                                                        ppd_name = g_strdup (g_variant_get_string (
                                                                                 g_variant_get_child_value (tuple, 0),
                                                                                 NULL));
                                        }
                                }
        }

        if (output) {
                g_variant_unref (output);
        } else {
                g_warning ("%s", error->message);
                g_error_free (error);
        }

        g_object_unref (proxy);

        return ppd_name;
}