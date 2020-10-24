static void
remove_stream (GvcMixerControl *control,
               GvcMixerStream  *stream)
{
        guint id;

        g_object_ref (stream);

        id = gvc_mixer_stream_get_id (stream);

        if (id == control->priv->default_sink_id) {
                _set_default_sink (control, NULL);
        } else if (id == control->priv->default_source_id) {
                _set_default_source (control, NULL);
        }

        g_hash_table_remove (control->priv->all_streams,
                             GUINT_TO_POINTER (id));
        g_signal_emit (G_OBJECT (control),
                       signals[STREAM_REMOVED],
                       0,
                       gvc_mixer_stream_get_id (stream));
        g_object_unref (stream);
}