static guint
can_set (const gchar *key, gboolean mandatory)
{
        GDBusConnection *bus = NULL;
        GDBusProxy *proxy = NULL;
	const gchar *keys[2];
	const gchar *func;
	GError *error = NULL;
        GVariant *res;
        guint retval = 0;

        bus = g_bus_get_sync (G_BUS_TYPE_SYSTEM,
                              NULL, NULL);
        if (bus == NULL)
                goto out;

        proxy = g_dbus_proxy_new_sync (bus,
                                       G_DBUS_PROXY_FLAGS_NONE,
                                       NULL,
                                       "org.gnome.GConf.Defaults",
                                       "/",
                                       "org.gnome.GConf.Defaults",
                                       NULL, NULL);
        if (proxy == NULL)
                goto out;

	keys[0] = key;
	keys[1] = NULL;
	func = mandatory ? "CanSetMandatory" : "CanSetSystem";

        res = g_dbus_proxy_call_sync (proxy, func,
                                      g_variant_new ("(^as)", keys),
                                      G_DBUS_CALL_FLAGS_NONE,
                                      -1, NULL, &error);

        if (error != NULL) {
    		g_warning ("error calling %s: %s\n", func, error->message);
    		g_error_free (error);
  	} else {
                g_variant_get (res, "(u)", &retval);
        }

out:
        if (bus)
                g_object_unref (bus);

	if (proxy)
		g_object_unref (proxy);

        return retval;
}