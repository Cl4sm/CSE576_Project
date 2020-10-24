static gchar *
register_gnome_session_client (const gchar *app_id,
                               const gchar *client_startup_id)
{
        GDBusProxy  *proxy;
        GVariant    *output = NULL;
        GError      *error = NULL;
        const gchar *client_id = NULL;
        gchar       *result = NULL;

        proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                               G_DBUS_PROXY_FLAGS_NONE,
                                               NULL,
                                               GNOME_SESSION_DBUS_NAME,
                                               GNOME_SESSION_DBUS_PATH,
                                               GNOME_SESSION_DBUS_IFACE,
                                               NULL,
                                               &error);

        if (proxy) {
                output = g_dbus_proxy_call_sync (proxy,
                                                 "RegisterClient",
                                                 g_variant_new ("(ss)", app_id, client_startup_id),
                                                 G_DBUS_CALL_FLAGS_NONE,
                                                 -1,
                                                 NULL,
                                                 &error);

                if (output) {
                        g_variant_get (output, "(o)", &client_id);
                        if (client_id)
                                result = g_strdup (client_id);
                        g_variant_unref (output);
                }
                else {
                        g_warning ("%s", error->message);
                        g_error_free (error);
                }

                g_object_unref (proxy);
        }
        else {
                g_warning ("%s", error->message);
                g_error_free (error);
        }

        return result;
}