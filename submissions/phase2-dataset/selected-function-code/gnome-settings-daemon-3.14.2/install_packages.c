static void
install_packages (GHashTable *packages)
{
        GVariantBuilder  array_builder;
        GHashTableIter   pkg_iter;
        GDBusProxy      *proxy;
        GVariant        *output;
        gpointer         key, value;
        GError          *error = NULL;

        if (!packages || g_hash_table_size (packages) <= 0)
                return;

        proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                               G_DBUS_PROXY_FLAGS_NONE,
                                               NULL,
                                               PACKAGE_KIT_BUS,
                                               PACKAGE_KIT_PATH,
                                               PACKAGE_KIT_MODIFY_IFACE,
                                               NULL,
                                               &error);

        if (!proxy) {
                g_warning ("%s", error->message);
                g_error_free (error);
                return;
        }

        g_variant_builder_init (&array_builder, G_VARIANT_TYPE ("as"));

        g_hash_table_iter_init (&pkg_iter, packages);
        while (g_hash_table_iter_next (&pkg_iter, &key, &value)) {
                g_variant_builder_add (&array_builder,
                                       "s",
                                       (gchar *) key);
        }

        output = g_dbus_proxy_call_sync (proxy,
                                         "InstallPackageNames",
                                         g_variant_new ("(uass)",
                                                        0,
                                                        &array_builder,
                                                        "hide-finished"),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         DBUS_INSTALL_TIMEOUT,
                                         NULL,
                                         &error);

        if (output) {
                g_variant_unref (output);
        } else {
                g_warning ("%s", error->message);
                g_error_free (error);
        }

        g_object_unref (proxy);
}