static gchar *
get_proxy_http (GsdUpdatesManager *manager)
{
        gboolean ret;
        gchar *host = NULL;
        gchar *password = NULL;
        gchar *proxy = NULL;
        gchar *username = NULL;
        GString *string = NULL;
        guint port;
        GDesktopProxyMode proxy_mode;

        proxy_mode = g_settings_get_enum (manager->priv->settings_proxy, "mode");
        if (proxy_mode != G_DESKTOP_PROXY_MODE_MANUAL)
                goto out;

        host = g_settings_get_string (manager->priv->settings_http,
                                      "host");
        if (host == NULL)
                goto out;
        port = g_settings_get_int (manager->priv->settings_http,
                                   "port");

        /* use an HTTP auth string? */
        ret = g_settings_get_boolean (manager->priv->settings_http,
                                      "use-authentication");
        if (ret) {
                username = g_settings_get_string (manager->priv->settings_http,
                                                  "authentication-user");
                password = g_settings_get_string (manager->priv->settings_http,
                                                  "authentication-password");
        }

        /* make PackageKit proxy string */
        string = g_string_new (host);
        if (port > 0)
                g_string_append_printf (string, ":%i", port);
        if (username != NULL && password != NULL)
                g_string_append_printf (string, "@%s:%s", username, password);
        else if (username != NULL)
                g_string_append_printf (string, "@%s", username);
        else if (password != NULL)
                g_string_append_printf (string, "@:%s", password);
        proxy = g_string_free (string, FALSE);
out:
        g_free (host);
        g_free (username);
        g_free (password);
        return proxy;
}