static void
on_channel_ready (GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    TpChannel *tp_chan = TP_CHANNEL (source_object);
    McdChannel *channel, **channel_ptr = user_data;
    GError *error = NULL;

    if (!tp_proxy_prepare_finish (tp_chan, result, &error))
    {
        DEBUG ("failed to prepare channel: %s", error->message);
        g_clear_error (&error);
        return;
    }

    channel = *channel_ptr;
    if (channel)
	g_object_remove_weak_pointer ((GObject *)channel,
				      (gpointer)channel_ptr);
    g_slice_free (McdChannel *, channel_ptr);
    if (error)
    {
        DEBUG ("got error: %s", error->message);
	return;
    }

    if (!channel) return;

    DEBUG ("channel %p is ready", channel);
    channel->priv->outgoing = tp_channel_get_requested (tp_chan);
}