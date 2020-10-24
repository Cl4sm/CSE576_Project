static void
gsd_updates_refresh_finalize (GObject *object)
{
        GsdUpdatesRefresh *refresh;

        g_return_if_fail (GSD_IS_UPDATES_REFRESH (object));

        refresh = GSD_UPDATES_REFRESH (object);
        g_return_if_fail (refresh->priv != NULL);

        if (refresh->priv->timeout_id != 0)
                g_source_remove (refresh->priv->timeout_id);
        if (refresh->priv->periodic_id != 0)
                g_source_remove (refresh->priv->periodic_id);

        g_signal_handlers_disconnect_by_data (refresh->priv->client, refresh);
        g_signal_handlers_disconnect_by_data (refresh->priv->proxy_session, refresh);

        g_object_unref (refresh->priv->control);
        g_object_unref (refresh->priv->settings);
        g_object_unref (refresh->priv->client);
        if (refresh->priv->proxy_session != NULL)
                g_object_unref (refresh->priv->proxy_session);

        G_OBJECT_CLASS (gsd_updates_refresh_parent_class)->finalize (object);
}