static void
on_gconf_proxy_created (GObject *source,
                        GAsyncResult *res,
                        gpointer user_data)
{
        GConfPKCallbackData *data = user_data;
        GDBusProxy *proxy;
	const gchar     *call;
        gchar *keys[2] = { data->key, NULL };
        gchar *dummy[2] = { NULL };
        GError *error = NULL;

        proxy = g_dbus_proxy_new_finish (res, &error);

        if (error != NULL) {
                if (data->callback)
                        data->callback (data->data, error);

                return;
        }

	call = data->mandatory ? "SetMandatory" : "SetSystem";
        g_dbus_proxy_call (proxy,
                           call, 
                           g_variant_new ("(^as^as)",
                                          keys, dummy),
                           G_DBUS_CALL_FLAGS_NONE,
                           -1, NULL,
                           on_proxy_method_call, data);
}