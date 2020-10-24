static void
_mcd_dispatch_operation_run_observers (McdDispatchOperation *self)
{
    const gchar *dispatch_operation_path = "/";
    GHashTable *observer_info;
    GHashTableIter iter;
    gpointer client_p;

    observer_info = tp_asv_new (NULL, NULL);

    _mcd_client_registry_init_hash_iter (self->priv->client_registry, &iter);

    while (g_hash_table_iter_next (&iter, NULL, &client_p))
    {
        McdClientProxy *client = MCD_CLIENT_PROXY (client_p);
        gboolean observed = FALSE;
        const gchar *account_path, *connection_path;
        GPtrArray *channels_array, *satisfied_requests;
        GHashTable *request_properties;

        if (!tp_proxy_has_interface_by_id (client,
                                           TP_IFACE_QUARK_CLIENT_OBSERVER))
            continue;

        if (self->priv->channel != NULL)
        {
            McdChannel *channel = MCD_CHANNEL (self->priv->channel);
            GVariant *properties;

            properties = mcd_channel_dup_immutable_properties (channel);
            g_assert (properties != NULL);

            if (_mcd_client_match_filters (properties,
                _mcd_client_proxy_get_observer_filters (client),
                FALSE))
                observed = TRUE;

            g_variant_unref (properties);
        }

        /* in particular this happens if there is no channel at all */
        if (!observed) continue;

        /* build up the parameters and invoke the observer */

        connection_path = _mcd_dispatch_operation_get_connection_path (self);
        account_path = _mcd_dispatch_operation_get_account_path (self);

        /* TODO: there's room for optimization here: reuse the channels_array,
         * if the observed list is the same */
        channels_array = _mcd_tp_channel_details_build_from_tp_chan (
            mcd_channel_get_tp_channel (self->priv->channel));

        collect_satisfied_requests (self->priv->channel, &satisfied_requests,
                                    &request_properties);

        /* transfer ownership into observer_info */
        tp_asv_take_boxed (observer_info, "request-properties",
            TP_HASH_TYPE_OBJECT_IMMUTABLE_PROPERTIES_MAP,
            request_properties);
        request_properties = NULL;

        if (_mcd_dispatch_operation_needs_approval (self))
        {
            dispatch_operation_path = _mcd_dispatch_operation_get_path (self);
        }

        _mcd_dispatch_operation_inc_observers_pending (self, client);

        DEBUG ("calling ObserveChannels on %s for CDO %p",
               tp_proxy_get_bus_name (client), self);
        tp_cli_client_observer_call_observe_channels (
            (TpClient *) client, -1,
            account_path, connection_path, channels_array,
            dispatch_operation_path, satisfied_requests, observer_info,
            observe_channels_cb,
            g_object_ref (self), g_object_unref, NULL);

        g_ptr_array_unref (satisfied_requests);

        _mcd_tp_channel_details_free (channels_array);
    }

    g_hash_table_unref (observer_info);
}