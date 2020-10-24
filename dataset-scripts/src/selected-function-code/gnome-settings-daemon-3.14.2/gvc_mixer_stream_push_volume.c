gboolean
gvc_mixer_stream_push_volume (GvcMixerStream *stream)
{
        pa_operation *op;
        gboolean ret;

        g_return_val_if_fail (GVC_IS_MIXER_STREAM (stream), FALSE);

        if (stream->priv->is_event_stream != FALSE)
                return TRUE;

        g_debug ("Pushing new volume to stream '%s' (%s)",
                 stream->priv->description, stream->priv->name);

        ret = GVC_MIXER_STREAM_GET_CLASS (stream)->push_volume (stream, (gpointer *) &op);
        if (ret) {
                if (stream->priv->change_volume_op != NULL)
                        pa_operation_unref (stream->priv->change_volume_op);
                stream->priv->change_volume_op = op;
        }
        return ret;
}