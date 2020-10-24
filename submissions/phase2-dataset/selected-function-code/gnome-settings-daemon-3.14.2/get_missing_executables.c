static GHashTable *
get_missing_executables (const gchar *ppd_file_name)
{
        GHashTable *executables = NULL;
        GDBusProxy *proxy;
        GVariant   *output;
        GVariant   *array;
        GError     *error = NULL;
        gint        i;

        if (!ppd_file_name)
                return NULL;

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
                                         "MissingExecutables",
                                         g_variant_new ("(s)",
                                                        ppd_file_name),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         DBUS_TIMEOUT,
                                         NULL,
                                         &error);

        if (output && g_variant_n_children (output) == 1) {
                array = g_variant_get_child_value (output, 0);
                if (array) {
                        executables = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                             g_free, NULL);
                        for (i = 0; i < g_variant_n_children (array); i++) {
                                g_hash_table_insert (executables,
                                                     g_strdup (g_variant_get_string (
                                                       g_variant_get_child_value (array, i),
                                                       NULL)),
                                                     NULL);
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

        return executables;
}