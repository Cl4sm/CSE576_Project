static void
copy_status (McdChannel *source, McdChannel *dest)
{
    McdChannelPrivate *src_priv, *dst_priv;

    src_priv = source->priv;
    dst_priv = dest->priv;
    if (dst_priv->status != src_priv->status)
    {
        DEBUG ("source is %d, dest is %d", src_priv->status, dst_priv->status);
        if (src_priv->status == MCD_CHANNEL_STATUS_FAILED)
        {
            const GError *error;

            error = mcd_channel_get_error (source);
            /* this also takes care of setting the status */
            mcd_channel_take_error (dest, g_error_copy (error));
        }
        else
            _mcd_channel_set_status (dest, src_priv->status);
    }

    if (dst_priv->status == MCD_CHANNEL_STATUS_FAILED ||
        dst_priv->status == MCD_CHANNEL_STATUS_DISPATCHED)
    {
        /* the request is completed, we are not interested in monitor the
         * channel anymore */
        g_signal_handlers_disconnect_by_func
            (source, on_proxied_channel_status_changed, dest);
        mcd_mission_abort (MCD_MISSION (dest));
    }
}