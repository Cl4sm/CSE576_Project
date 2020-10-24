static void
on_proxy_method_call (GObject *source,
                      GAsyncResult *res,
                      gpointer user_data)
{
        GConfPKCallbackData *data = user_data;
        GError *error = NULL;

        g_dbus_proxy_call_finish (G_DBUS_PROXY (source), res, &error);

        if (error == NULL) {
		gconf_pk_update_can_set_cache (data->key, data->mandatory);
                if (data->callback)
                        data->callback (data->data, NULL);
        } else {
                if (error->domain == G_DBUS_ERROR &&
                    error->code == G_DBUS_ERROR_NO_REPLY) {
                        /* these errors happen because dbus doesn't
                         * use monotonic clocks
                         */
                        g_warning ("ignoring no-reply error when setting key");
                        g_error_free (error);
                        if (data->callback)
                                data->callback (data->data, NULL);
                }
                else {
                        if (data->callback)
                                data->callback (data->data, error);
                }
        }
}