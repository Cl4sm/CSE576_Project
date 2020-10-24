static void
maybe_get_upgrades (GsdUpdatesRefresh *refresh)
{
        guint thresh;

        g_return_if_fail (GSD_IS_UPDATES_REFRESH (refresh));

        /* get this each time, as it may have changed behind out back */
        thresh = g_settings_get_int (refresh->priv->settings,
                                     GSD_SETTINGS_FREQUENCY_GET_UPGRADES);
        if (thresh == 0) {
                g_debug ("not when policy is set to never");
                return;
        }

        /* get the time since the last refresh */
        pk_control_get_time_since_action_async (refresh->priv->control,
                                                PK_ROLE_ENUM_GET_DISTRO_UPGRADES,
                                                NULL,
                                                (GAsyncReadyCallback) get_time_get_upgrades_cb,
                                                refresh);
}