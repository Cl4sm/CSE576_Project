static void
gsd_media_keys_manager_init (GsdMediaKeysManager *manager)
{
        GError *error;
        GDBusConnection *bus;

        error = NULL;
        manager->priv = GSD_MEDIA_KEYS_MANAGER_GET_PRIVATE (manager);

        bus = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, &error);
        if (bus == NULL) {
                g_warning ("Failed to connect to system bus: %s",
                           error->message);
                g_error_free (error);
                return;
        }

        manager->priv->logind_proxy =
                g_dbus_proxy_new_sync (bus,
                                       0,
                                       NULL,
                                       SYSTEMD_DBUS_NAME,
                                       SYSTEMD_DBUS_PATH,
                                       SYSTEMD_DBUS_INTERFACE,
                                       NULL,
                                       &error);

        if (manager->priv->logind_proxy == NULL) {
                g_warning ("Failed to connect to systemd: %s",
                           error->message);
                g_error_free (error);
        }

        g_object_unref (bus);

        g_debug ("Adding system inhibitors for power keys");
        manager->priv->inhibit_keys_fd = -1;
        g_dbus_proxy_call_with_unix_fd_list (manager->priv->logind_proxy,
                                             "Inhibit",
                                             g_variant_new ("(ssss)",
                                                            "handle-power-key:handle-suspend-key:handle-hibernate-key",
                                                            g_get_user_name (),
                                                            "GNOME handling keypresses",
                                                            "block"),
                                             0,
                                             G_MAXINT,
                                             NULL,
                                             NULL,
                                             inhibit_done,
                                             manager);

}