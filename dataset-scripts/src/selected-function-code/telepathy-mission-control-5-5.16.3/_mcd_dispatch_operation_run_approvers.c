static void
_mcd_dispatch_operation_run_approvers (McdDispatchOperation *self)
{
    GHashTableIter iter;
    gpointer client_p;

    /* we temporarily increment this count and decrement it at the end of the
     * function, to make sure it won't become 0 while we are still invoking
     * approvers */
    _mcd_dispatch_operation_inc_ado_pending (self);

    _mcd_client_registry_init_hash_iter (self->priv->client_registry, &iter);
    while (g_hash_table_iter_next (&iter, NULL, &client_p))
    {
        McdClientProxy *client = MCD_CLIENT_PROXY (client_p);
        GPtrArray *channel_details;
        const gchar *dispatch_operation;
        GHashTable *properties;
        gboolean matched = FALSE;

        if (!tp_proxy_has_interface_by_id (client,
                                           TP_IFACE_QUARK_CLIENT_APPROVER))
            continue;

        if (self->priv->channel != NULL)
        {
            McdChannel *channel = MCD_CHANNEL (self->priv->channel);
            GVariant *channel_properties;

            channel_properties = mcd_channel_dup_immutable_properties (channel);
            g_assert (channel_properties != NULL);

            if (_mcd_client_match_filters (channel_properties,
                _mcd_client_proxy_get_approver_filters (client),
                FALSE))
            {
                matched = TRUE;
            }

            g_variant_unref (channel_properties);
        }

        /* in particular, after this point, self->priv->channel can't
         * be NULL */
        if (!matched) continue;

        dispatch_operation = _mcd_dispatch_operation_get_path (self);
        properties = _mcd_dispatch_operation_get_properties (self);
        channel_details = _mcd_tp_channel_details_build_from_tp_chan (
            mcd_channel_get_tp_channel (self->priv->channel));

        DEBUG ("Calling AddDispatchOperation on approver %s for CDO %s @ %p",
               tp_proxy_get_bus_name (client), dispatch_operation, self);

        _mcd_dispatch_operation_inc_ado_pending (self);

        tp_cli_client_approver_call_add_dispatch_operation (
            (TpClient *) client, -1,
            channel_details, dispatch_operation, properties,
            add_dispatch_operation_cb,
            g_object_ref (self), g_object_unref, NULL);

        g_boxed_free (TP_ARRAY_TYPE_CHANNEL_DETAILS_LIST, channel_details);
    }

    /* This matches the approvers count set to 1 at the beginning of the
     * function */
    _mcd_dispatch_operation_dec_ado_pending (self);
}