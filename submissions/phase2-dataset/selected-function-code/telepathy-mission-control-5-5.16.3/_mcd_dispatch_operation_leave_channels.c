void
_mcd_dispatch_operation_leave_channels (McdDispatchOperation *self,
                                        TpChannelGroupChangeReason reason,
                                        const gchar *message)
{
    if (message == NULL)
    {
        message = "";
    }

    if (self->priv->channel != NULL)
    {
        /* Take a temporary copy, because self->priv->channels could
         * be modified as a result */
        McdChannel *channel = g_object_ref (self->priv->channel);

        _mcd_channel_depart (channel, reason, message);
        g_object_unref (channel);
    }

    _mcd_dispatch_operation_forget_channels (self);
}