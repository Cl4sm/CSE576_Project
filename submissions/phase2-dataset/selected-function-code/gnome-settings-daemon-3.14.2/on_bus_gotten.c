static void
on_bus_gotten (GObject             *source_object,
               GAsyncResult        *res,
               GsdMediaKeysManager *manager)
{
        GDBusConnection *connection;
        GError *error = NULL;
        UpClient *up_client;

        if (manager->priv->bus_cancellable == NULL ||
            g_cancellable_is_cancelled (manager->priv->bus_cancellable)) {
                g_warning ("Operation has been cancelled, so not retrieving session bus");
                return;
        }

        connection = g_bus_get_finish (res, &error);
        if (connection == NULL) {
                g_warning ("Could not get session bus: %s", error->message);
                g_error_free (error);
                return;
        }
        manager->priv->connection = connection;

        g_dbus_connection_register_object (connection,
                                           GSD_MEDIA_KEYS_DBUS_PATH,
                                           manager->priv->introspection_data->interfaces[0],
                                           &interface_vtable,
                                           manager,
                                           NULL,
                                           NULL);

        g_dbus_proxy_new (manager->priv->connection,
                          G_DBUS_PROXY_FLAGS_NONE,
                          NULL,
                          GSD_DBUS_NAME ".XRANDR",
                          GSD_DBUS_PATH "/XRANDR",
                          GSD_DBUS_BASE_INTERFACE ".XRANDR_2",
                          NULL,
                          (GAsyncReadyCallback) xrandr_ready_cb,
                          manager);

        g_dbus_proxy_new (manager->priv->connection,
                          G_DBUS_PROXY_FLAGS_NONE,
                          NULL,
                          GSD_DBUS_NAME ".Power",
                          GSD_DBUS_PATH "/Power",
                          GSD_DBUS_BASE_INTERFACE ".Power",
                          NULL,
                          (GAsyncReadyCallback) power_ready_cb,
                          manager);

        g_dbus_proxy_new (manager->priv->connection,
                          G_DBUS_PROXY_FLAGS_NONE,
                          NULL,
                          GSD_DBUS_NAME ".Power",
                          GSD_DBUS_PATH "/Power",
                          GSD_DBUS_BASE_INTERFACE ".Power.Screen",
                          NULL,
                          (GAsyncReadyCallback) power_screen_ready_cb,
                          manager);

        g_dbus_proxy_new (manager->priv->connection,
                          G_DBUS_PROXY_FLAGS_NONE,
                          NULL,
                          GSD_DBUS_NAME ".Power",
                          GSD_DBUS_PATH "/Power",
                          GSD_DBUS_BASE_INTERFACE ".Power.Keyboard",
                          NULL,
                          (GAsyncReadyCallback) power_keyboard_ready_cb,
                          manager);

        up_client = up_client_new ();
        manager->priv->composite_device = up_client_get_display_device (up_client);
        g_object_unref (up_client);
}