McdChannel *
mcd_channel_new_from_properties (TpConnection *connection,
                                 const gchar *object_path,
                                 const GHashTable *properties)
{
    McdChannel *channel;
    TpChannel *tp_chan;
    GError *error = NULL;

    tp_chan = tp_simple_client_factory_ensure_channel (
        tp_proxy_get_factory (connection), connection, object_path,
        properties, &error);

    if (G_UNLIKELY (error))
    {
        g_warning ("%s: got error: %s", G_STRFUNC, error->message);
        g_error_free (error);
        return NULL;
    }

    channel = g_object_new (MCD_TYPE_CHANNEL,
                            "tp-channel", tp_chan,
                            NULL);
    g_object_unref (tp_chan);
    return channel;
}