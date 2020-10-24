static GHashTable *
find_packages_for_executables (GHashTable *executables)
{
        GHashTableIter  exec_iter;
        GHashTable     *packages = NULL;
        GDBusProxy     *proxy;
        GVariant       *output;
        gpointer        key, value;
        GError         *error = NULL;

        if (!executables || g_hash_table_size (executables) <= 0)
                return NULL;

        proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                               G_DBUS_PROXY_FLAGS_NONE,
                                               NULL,
                                               PACKAGE_KIT_BUS,
                                               PACKAGE_KIT_PATH,
                                               PACKAGE_KIT_QUERY_IFACE,
                                               NULL,
                                               &error);

        if (!proxy) {
                g_warning ("%s", error->message);
                g_error_free (error);
                return NULL;
        }

        packages = g_hash_table_new_full (g_str_hash, g_str_equal,
                                          g_free, NULL);

        g_hash_table_iter_init (&exec_iter, executables);
        while (g_hash_table_iter_next (&exec_iter, &key, &value)) {
                output = g_dbus_proxy_call_sync (proxy,
                                                 "SearchFile",
                                                 g_variant_new ("(ss)",
                                                                (gchar *) key,
                                                                ""),
                                                 G_DBUS_CALL_FLAGS_NONE,
                                                 DBUS_TIMEOUT,
                                                 NULL,
                                                 &error);

                if (output) {
                        gboolean  installed;
                        gchar    *package;

                        g_variant_get (output,
                                       "(bs)",
                                       &installed,
                                       &package);
                        if (!installed)
                                g_hash_table_insert (packages, g_strdup (package), NULL);

                        g_variant_unref (output);
                } else {
                        g_warning ("%s", error->message);
                        g_error_free (error);
                }
        }

        g_object_unref (proxy);

        return packages;
}