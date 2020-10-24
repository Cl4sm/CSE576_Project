void
_mcd_channel_undispatchable (McdChannel *channel)
{
    McdChannelPrivate *priv = MCD_CHANNEL_PRIV (channel);

    if (!_mcd_tp_channel_should_close (priv->tp_chan, "destroying"))
    {
        return;
    }

    DEBUG ("%p: %s", channel, mcd_channel_get_object_path (channel));

    /* Call Destroy() if possible, or Close() */
    if (tp_proxy_has_interface_by_id (priv->tp_chan,
        TP_IFACE_QUARK_CHANNEL_INTERFACE_DESTROYABLE))
    {
        DEBUG ("calling Destroy()");
        tp_cli_channel_interface_destroyable_call_destroy (priv->tp_chan,
                                                           -1, NULL,
                                                           NULL, NULL,
                                                           NULL);
    }
    else
    {
        DEBUG ("calling Close()");
        tp_cli_channel_call_close (priv->tp_chan, -1, NULL, NULL, NULL,
                                   NULL);
    }
}