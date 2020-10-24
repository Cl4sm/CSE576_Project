static void
mcd_dispatch_operation_handle_channels (McdDispatchOperation *self)
{
    GList *channels = NULL;
    GHashTable *handler_info;
    GHashTable *request_properties;

    g_assert (self->priv->trying_handler != NULL);

    if (self->priv->handler_unsuitable != NULL)
    {
        GError *tmp = self->priv->handler_unsuitable;

        /* move the error out of the way first, in case the callback
         * tries a different handler which will also want to check
         * handler_unsuitable */
        self->priv->handler_unsuitable = NULL;

        _mcd_dispatch_operation_handle_channels_cb (
            (TpClient *) self->priv->trying_handler,
            tmp, self, NULL);
        g_error_free (tmp);

        return;
    }

    /* FIXME: it shouldn't be possible to get here without a channel */
    if (self->priv->channel != NULL)
    {
        collect_satisfied_requests (self->priv->channel, NULL,
                                    &request_properties);
        channels = g_list_prepend (NULL, self->priv->channel);
    }
    else
    {
        request_properties = g_hash_table_new_full (g_str_hash,
            g_str_equal, g_free, (GDestroyNotify) g_hash_table_unref);
    }

    handler_info = tp_asv_new (NULL, NULL);
    tp_asv_take_boxed (handler_info, "request-properties",
        TP_HASH_TYPE_OBJECT_IMMUTABLE_PROPERTIES_MAP, request_properties);
    request_properties = NULL;

    _mcd_client_proxy_handle_channels (self->priv->trying_handler,
        -1, channels, self->priv->handle_with_time,
        handler_info, _mcd_dispatch_operation_handle_channels_cb,
        g_object_ref (self), g_object_unref, NULL);

    g_hash_table_unref (handler_info);
    g_list_free (channels);
}