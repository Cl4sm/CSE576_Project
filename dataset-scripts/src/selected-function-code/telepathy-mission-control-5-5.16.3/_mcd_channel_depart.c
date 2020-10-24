void
_mcd_channel_depart (McdChannel *channel,
                     TpChannelGroupChangeReason reason,
                     const gchar *message)
{
    DepartData *d;
    const GError *invalidated;
    GQuark just_group_feature[] = { TP_CHANNEL_FEATURE_GROUP, 0 };

    g_return_if_fail (MCD_IS_CHANNEL (channel));

    g_return_if_fail (channel->priv->tp_chan != NULL);
    g_return_if_fail (message != NULL);

    invalidated = tp_proxy_get_invalidated (channel->priv->tp_chan);

    if (invalidated != NULL)
    {
        DEBUG ("%s %d: %s", g_quark_to_string (invalidated->domain),
               invalidated->code, invalidated->message);
        return;
    }

    if (message[0] == '\0' && reason == TP_CHANNEL_GROUP_CHANGE_REASON_NONE)
    {
        /* exactly equivalent to Close(), so skip the Group interface */
        tp_channel_close_async (channel->priv->tp_chan, NULL, NULL);
        return;
    }

    d = g_slice_new (DepartData);
    d->reason = reason;
    d->message = g_strdup (message);

    /* tp_channel_leave_async documents calling it without first preparing
     * GROUP as deprecated. */
    tp_proxy_prepare_async (channel->priv->tp_chan, just_group_feature,
                            mcd_channel_ready_to_depart_cb, d);
}