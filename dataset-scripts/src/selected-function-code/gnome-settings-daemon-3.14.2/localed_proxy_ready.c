static void
localed_proxy_ready (GObject      *source,
                     GAsyncResult *res,
                     gpointer      data)
{
        GsdKeyboardManager *manager = data;
        GDBusProxy *proxy;
        GError *error = NULL;

        proxy = g_dbus_proxy_new_finish (res, &error);
        if (!proxy) {
                if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED)) {
                        g_error_free (error);
                        return;
                }
                g_warning ("Failed to contact localed: %s", error->message);
                g_error_free (error);
        }

        manager->priv->localed = proxy;
        maybe_create_initial_settings (manager);
}