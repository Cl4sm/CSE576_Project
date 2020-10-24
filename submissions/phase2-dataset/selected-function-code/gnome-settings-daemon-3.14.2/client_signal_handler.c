static void
client_signal_handler (GDBusConnection  *connection,
                       const gchar      *sender_name,
                       const gchar      *object_path,
                       const gchar      *interface_name,
                       const gchar      *signal_name,
                       GVariant         *parameters,
                       gpointer          user_data)
{
        GDBusProxy *proxy;
        GError     *error = NULL;
        GVariant   *output;

        if (g_strcmp0 (signal_name, "QueryEndSession") == 0 ||
            g_strcmp0 (signal_name, "EndSession") == 0) {
                proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
                                                       G_DBUS_PROXY_FLAGS_NONE,
                                                       NULL,
                                                       sender_name,
                                                       object_path,
                                                       interface_name,
                                                       NULL,
                                                       &error);

                if (proxy) {
                        output = g_dbus_proxy_call_sync (proxy,
                                                         "EndSessionResponse",
                                                         g_variant_new ("(bs)", TRUE, ""),
                                                         G_DBUS_CALL_FLAGS_NONE,
                                                         -1,
                                                         NULL,
                                                         &error);

                        if (output) {
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

                if (g_strcmp0 (signal_name, "EndSession") == 0) {
                        g_main_loop_quit (main_loop);
                        g_debug ("Exiting gsd-printer");
                }
        }
}