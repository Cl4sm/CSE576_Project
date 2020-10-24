static void
on_system_bus_got (GObject *source,
                   GAsyncResult *res,
                   gpointer user_data)
{
        GConfPKCallbackData *data = user_data;
        GDBusConnection *bus;
        GError *error = NULL;

        bus = g_bus_get_finish (res, &error);

        if (error != NULL) {
                if (data->callback)
                        data->callback (data->data, error);

                return;
        }

        g_dbus_proxy_new (bus,
                          G_DBUS_PROXY_FLAGS_NONE,
                          NULL,
                          "org.gnome.GConf.Defaults",
                          "/",
                          "org.gnome.GConf.Defaults",
                          NULL,
                          on_gconf_proxy_created,
                          data);
}