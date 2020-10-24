static void
session_presence_signal_cb (GDBusProxy *proxy,
                            gchar *sender_name,
                            gchar *signal_name,
                            GVariant *parameters,
                            GsdUpdatesRefresh *refresh)
{
        guint status;

        g_return_if_fail (GSD_IS_UPDATES_REFRESH (refresh));

        if (g_strcmp0 (signal_name, "StatusChanged") != 0)
                return;

        /* map status code into boolean */
        g_variant_get (parameters, "(u)", &status);
        refresh->priv->session_idle = (status == PRESENCE_STATUS_IDLE);
        g_debug ("setting is_idle %i",
                 refresh->priv->session_idle);
        if (refresh->priv->session_idle)
                change_state (refresh);

}