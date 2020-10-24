gboolean
_mcd_channel_create_proxy (McdChannel *channel, TpConnection *connection,
                           const gchar *object_path,
                           const GHashTable *properties)
{
    TpChannel *tp_chan;
    GError *error = NULL;

    g_return_val_if_fail (MCD_IS_CHANNEL (channel), FALSE);

    tp_chan = tp_simple_client_factory_ensure_channel (
        tp_proxy_get_factory (connection), connection, object_path,
        properties, &error);

    if (G_UNLIKELY (error))
    {
        g_warning ("%s: got error: %s", G_STRFUNC, error->message);
        g_error_free (error);
        return FALSE;
    }

    g_object_set (channel,
                  "tp-channel", tp_chan,
                  NULL);
    g_object_unref (tp_chan);
    return TRUE;
}