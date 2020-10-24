static gboolean
change_state (GsdUpdatesRefresh *refresh)
{
        gboolean ret;

        g_return_val_if_fail (GSD_IS_UPDATES_REFRESH (refresh), FALSE);

        /* no point continuing if we have no network */
        if (!refresh->priv->network_active) {
                g_debug ("not when no network");
                refresh->priv->timeout_id = 0;
                return FALSE;
        }

        /* not on battery unless overridden */
        ret = g_settings_get_boolean (refresh->priv->settings,
                                      GSD_SETTINGS_UPDATE_BATTERY);
        if (!ret && refresh->priv->on_battery) {
                g_debug ("not when on battery");
                refresh->priv->timeout_id = 0;
                return FALSE;
        }

        /* wait a little time for things to settle down */
        if (refresh->priv->timeout_id != 0)
                g_source_remove (refresh->priv->timeout_id);
        g_debug ("defering action for %i seconds",
                 SESSION_STARTUP_TIMEOUT);
        refresh->priv->timeout_id =
                g_timeout_add_seconds (SESSION_STARTUP_TIMEOUT,
                                       (GSourceFunc) change_state_cb,
                                       refresh);
        g_source_set_name_by_id (refresh->priv->timeout_id,
                                 "[GsdUpdatesRefresh] change-state");

        return TRUE;
}