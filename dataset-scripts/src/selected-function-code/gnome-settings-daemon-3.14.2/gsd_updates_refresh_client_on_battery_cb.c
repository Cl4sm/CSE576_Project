static void
gsd_updates_refresh_client_on_battery_cb (UpClient *client,
					  GParamSpec *pspec,
					  GsdUpdatesRefresh *refresh)
{
        gboolean on_battery;

        g_return_if_fail (GSD_IS_UPDATES_REFRESH (refresh));

        /* get the on-battery state */
        on_battery = up_client_get_on_battery (refresh->priv->client);
        if (on_battery == refresh->priv->on_battery) {
                g_debug ("same state as before, ignoring");
                return;
        }

        /* save in local cache */
        g_debug ("setting on_battery %i", on_battery);
        refresh->priv->on_battery = on_battery;
        if (!on_battery)
                change_state (refresh);
}