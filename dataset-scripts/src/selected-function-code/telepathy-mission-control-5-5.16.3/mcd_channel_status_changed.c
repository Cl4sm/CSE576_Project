static void
mcd_channel_status_changed (McdChannel *channel, McdChannelStatus status)
{
    channel->priv->status = status;

    switch (status)
    {
        case MCD_CHANNEL_STATUS_UNDISPATCHED:
        case MCD_CHANNEL_STATUS_DISPATCHING:
        case MCD_CHANNEL_STATUS_HANDLER_INVOKED:
        case MCD_CHANNEL_STATUS_DISPATCHED:
            g_assert (channel->priv->tp_chan != NULL);
            break;

        case MCD_CHANNEL_STATUS_REQUEST:
        case MCD_CHANNEL_STATUS_REQUESTED:
            g_assert (channel->priv->tp_chan == NULL);
            break;

        case MCD_CHANNEL_STATUS_FAILED:
        case MCD_CHANNEL_STATUS_ABORTED:
            { /* no particular assertion */ }

        /* no default case, so the compiler will warn on unhandled states */
    }

    if (channel->priv->request != NULL &&
        !_mcd_request_is_complete (channel->priv->request))
    {
        if (status == MCD_CHANNEL_STATUS_FAILED)
        {
            const GError *error = mcd_channel_get_error (channel);

            if (G_LIKELY (error != NULL))
            {
                _mcd_request_set_failure (channel->priv->request,
                                          error->domain, error->code,
                                          error->message);
            }
            else
            {
                g_critical ("Requested channel's status changed to FAILED "
                            "without a proper error");
                _mcd_request_set_failure (channel->priv->request,
                                          TP_ERROR, TP_ERROR_NOT_AVAILABLE,
                                          "MC bug! FAILED but no error");
            }
        }
        else if (status == MCD_CHANNEL_STATUS_DISPATCHED)
        {
            _mcd_request_set_success (channel->priv->request,
                                      channel->priv->tp_chan);
        }
        else if (status == MCD_CHANNEL_STATUS_HANDLER_INVOKED)
        {
            _mcd_request_set_uncancellable (channel->priv->request);
        }
    }
}