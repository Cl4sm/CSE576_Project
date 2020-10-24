gboolean
gvc_mixer_stream_set_port (GvcMixerStream *stream,
                           const char     *port)
{
        GList *l;

        g_return_val_if_fail (GVC_IS_MIXER_STREAM (stream), FALSE);
        g_return_val_if_fail (stream->priv->ports != NULL, FALSE);

        g_free (stream->priv->port);
        stream->priv->port = g_strdup (port);

        g_free (stream->priv->human_port);
        stream->priv->human_port = NULL;

        for (l = stream->priv->ports; l != NULL; l = l->next) {
                GvcMixerStreamPort *p = l->data;
                if (g_str_equal (stream->priv->port, p->port)) {
                        stream->priv->human_port = g_strdup (p->human_port);
                        break;
                }
        }

        g_object_notify (G_OBJECT (stream), "port");

        return TRUE;
}