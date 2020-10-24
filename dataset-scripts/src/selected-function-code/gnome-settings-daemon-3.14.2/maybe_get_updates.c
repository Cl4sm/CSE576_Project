static void
maybe_get_updates (GsdUpdatesRefresh *refresh)
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

        /* get the time since the last refresh */
        pk_control_get_time_since_action_async (refresh->priv->control,
                                                PK_ROLE_ENUM_GET_UPDATES,
                                                NULL,
                                                (GAsyncReadyCallback) get_time_get_updates_cb,
                                                refresh);
}