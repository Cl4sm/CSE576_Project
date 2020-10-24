static void
maybe_refresh_cache (GsdUpdatesRefresh *refresh)
{
        guint thresh;

        g_return_if_fail (GSD_IS_UPDATES_REFRESH (refresh));

        /* if we don't want to auto check for updates, don't do this either */
        thresh = g_settings_get_int (refresh->priv->settings,
                                     GSD_SETTINGS_FREQUENCY_GET_UPDATES);
        if (thresh == 0) {
                g_debug ("not when policy is set to never");
                return;
        }

        /* only do the refresh cache when the user is idle */
        if (!refresh->priv->session_idle) {
                g_debug ("not when session active");
                return;
        }

        /* get this each time, as it may have changed behind out back */
        thresh = g_settings_get_int (refresh->priv->settings,
                                     GSD_SETTINGS_FREQUENCY_REFRESH_CACHE);
        if (thresh == 0) {
                g_debug ("not when policy is set to never");
                return;
        }

        /* get the time since the last refresh */
        pk_control_get_time_since_action_async (refresh->priv->control,
                                                PK_ROLE_ENUM_REFRESH_CACHE,
                                                NULL,
                                                (GAsyncReadyCallback) get_time_refresh_cache_cb,
                                                refresh);
}