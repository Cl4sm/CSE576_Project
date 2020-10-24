static void
gsd_updates_refresh_init (GsdUpdatesRefresh *refresh)
{
        GVariant *status;
        guint status_code;

        refresh->priv = GSD_UPDATES_REFRESH_GET_PRIVATE (refresh);
        refresh->priv->on_battery = FALSE;
        refresh->priv->network_active = FALSE;
        refresh->priv->timeout_id = 0;
        refresh->priv->periodic_id = 0;

        /* we need to know the updates frequency */
        refresh->priv->settings = g_settings_new (GSD_SETTINGS_SCHEMA);
        g_signal_connect (refresh->priv->settings, "changed",
                          G_CALLBACK (settings_key_changed_cb), refresh);

        /* we need to query the last cache refresh time */
        refresh->priv->control = pk_control_new ();
        g_signal_connect (refresh->priv->control, "notify::network-state",
                          G_CALLBACK (notify_network_state_cb),
                          refresh);

        /* get network state */
        pk_control_get_properties_async (refresh->priv->control,
                                         NULL,
                                         (GAsyncReadyCallback) get_properties_cb,
                                         refresh);

        /* use a UpClient */
        refresh->priv->client = up_client_new ();
        g_signal_connect (refresh->priv->client, "notify::on-battery",
                          G_CALLBACK (gsd_updates_refresh_client_on_battery_cb), refresh);

        /* get the battery state */
        refresh->priv->on_battery = up_client_get_on_battery (refresh->priv->client);
        g_debug ("setting on battery %i", refresh->priv->on_battery);

        /* use gnome-session for the idle detection */
        refresh->priv->proxy_session =
                gnome_settings_bus_get_session_proxy ();
        if (refresh->priv->proxy_session != NULL) {
                g_signal_connect (G_DBUS_PROXY (refresh->priv->proxy_session),
                                  "g-signal",
                                  G_CALLBACK (session_presence_signal_cb),
                                  refresh);
                status = g_dbus_proxy_get_cached_property (G_DBUS_PROXY (refresh->priv->proxy_session),
                                                           "status");
                if (status) {
                        g_variant_get (status, "u", &status_code);
                        refresh->priv->session_idle = (status_code == PRESENCE_STATUS_IDLE);
                        g_variant_unref (status);
                }
                else {
                        refresh->priv->session_idle = FALSE;
                }
        }

        /* we check this in case we miss one of the async signals */
        refresh->priv->periodic_id =
                g_timeout_add_seconds (PERIODIC_CHECK_TIME,
                                       periodic_timeout_cb, refresh);
        g_source_set_name_by_id (refresh->priv->periodic_id,
                                 "[GsdUpdatesRefresh] periodic check");

        /* check system state */
        change_state (refresh);
}