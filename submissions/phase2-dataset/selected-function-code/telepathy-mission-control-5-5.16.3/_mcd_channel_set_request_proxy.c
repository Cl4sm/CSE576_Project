void
_mcd_channel_set_request_proxy (McdChannel *channel, McdChannel *source)
{
    g_return_if_fail (MCD_IS_CHANNEL (channel));
    g_return_if_fail (MCD_IS_CHANNEL (source));
    g_return_if_fail (MCD_IS_REQUEST (channel->priv->request));

    g_return_if_fail (!source->priv->is_proxy);
    g_return_if_fail (source->priv->tp_chan != NULL);

    _mcd_channel_copy_details (channel, source);

    /* Now @source is also satisfying the request of @channel */
    source->priv->latest_request_time = MAX (source->priv->latest_request_time,
        channel->priv->latest_request_time);

    source->priv->satisfied_requests = g_list_prepend (
        source->priv->satisfied_requests,
        g_object_ref (channel->priv->request));

    copy_status (source, channel);
    g_signal_connect (source, "status-changed",
                      G_CALLBACK (on_proxied_channel_status_changed), channel);
}