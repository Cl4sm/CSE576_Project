static gboolean
gvc_mixer_stream_set_channel_map (GvcMixerStream *stream,
                                  GvcChannelMap  *channel_map)
{
        g_return_val_if_fail (GVC_IS_MIXER_STREAM (stream), FALSE);

        if (channel_map != NULL) {
                g_object_ref (channel_map);
        }

        if (stream->priv->channel_map != NULL) {
                g_signal_handlers_disconnect_by_func (stream->priv->channel_map,
                                                      on_channel_map_volume_changed,
                                                      stream);
                g_object_unref (stream->priv->channel_map);
        }

        stream->priv->channel_map = channel_map;

        if (stream->priv->channel_map != NULL) {
                g_signal_connect (stream->priv->channel_map,
                                  "volume-changed",
                                  G_CALLBACK (on_channel_map_volume_changed),
                                  stream);

                g_object_notify (G_OBJECT (stream), "channel-map");
        }

        return TRUE;
}