static gboolean
convert_network_state (GsdUpdatesRefresh *refresh, PkNetworkEnum state)
{
        /* offline */
        if (state == PK_NETWORK_ENUM_OFFLINE)
                return FALSE;

        /* online */
        if (state == PK_NETWORK_ENUM_ONLINE ||
            state == PK_NETWORK_ENUM_WIFI ||
            state == PK_NETWORK_ENUM_WIRED)
                return TRUE;

        /* check policy */
        if (state == PK_NETWORK_ENUM_MOBILE)
                return g_settings_get_boolean (refresh->priv->settings,
                                               GSD_SETTINGS_CONNECTION_USE_MOBILE);

        /* not recognised */
        g_warning ("state unknown: %i", state);
        return TRUE;
}