GVariant *
mcd_channel_dup_immutable_properties (McdChannel *channel)
{
    GVariant *ret;

    g_return_val_if_fail (MCD_IS_CHANNEL (channel), NULL);

    if (channel->priv->tp_chan == NULL)
    {
        DEBUG ("Channel %p has no associated TpChannel", channel);
        return NULL;
    }

    ret = tp_channel_dup_immutable_properties (channel->priv->tp_chan);

    if (ret == NULL)
    {
        DEBUG ("Channel %p TpChannel %s (%p) has no immutable properties yet",
               channel, tp_proxy_get_object_path (channel->priv->tp_chan),
               channel->priv->tp_chan);
        return NULL;
    }

    return ret;
}