McdChannel *
_mcd_channel_new_request (McdRequest *request)
{
    McdChannel *channel;

    channel = g_object_new (MCD_TYPE_CHANNEL,
                            "outgoing", TRUE,
                            NULL);

    /* TODO: this could be freed when the channel status becomes
     * MCD_CHANNEL_STATUS_DISPATCHED or MCD_CHANNEL_STATUS_FAILED? */
    channel->priv->request = request;

    channel->priv->satisfied_requests = g_list_prepend (NULL,
        g_object_ref (channel->priv->request));
    channel->priv->latest_request_time =
        _mcd_request_get_user_action_time (request);

    _mcd_channel_set_status (channel, MCD_CHANNEL_STATUS_REQUEST);

    /* for the moment McdChannel implements the later stages of cancelling */
    tp_g_signal_connect_object (request, "cancelling",
        G_CALLBACK (_mcd_channel_request_cancelling_cb), channel, 0);

    return channel;
}