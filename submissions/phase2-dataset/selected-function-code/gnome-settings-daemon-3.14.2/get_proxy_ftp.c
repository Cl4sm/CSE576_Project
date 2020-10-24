static gchar *
get_proxy_ftp (GsdUpdatesManager *manager)
{
        gchar *host = NULL;
        gchar *proxy = NULL;
        GString *string = NULL;
        guint port;
        GDesktopProxyMode proxy_mode;

        proxy_mode = g_settings_get_enum (manager->priv->settings_proxy, "mode");
        if (proxy_mode != G_DESKTOP_PROXY_MODE_MANUAL)
                goto out;

        host = g_settings_get_string (manager->priv->settings_ftp,
                                      "host");
        if (host == NULL)
                goto out;
        port = g_settings_get_int (manager->priv->settings_ftp,
                                   "port");
        if (port == 0)
                goto out;

        /* make PackageKit proxy string */
        string = g_string_new (host);
        if (port > 0)
                g_string_append_printf (string, ":%i", port);
        proxy = g_string_free (string, FALSE);
out:
        g_free (host);
        return proxy;
}