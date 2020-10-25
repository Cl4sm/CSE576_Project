static gboolean
gvc_mixer_sink_input_push_volume (GvcMixerStream *stream, gpointer *op)
{
        pa_operation        *o;
        guint                index;
        const GvcChannelMap *map;
        pa_context          *context;
        const pa_cvolume    *cv;

        index = gvc_mixer_stream_get_index (stream);

        map = gvc_mixer_stream_get_channel_map (stream);

        cv = gvc_channel_map_get_cvolume(map);

        context = gvc_mixer_stream_get_pa_context (stream);

        o = pa_context_set_sink_input_volume (context,
                                              index,
                                              cv,
                                              NULL,
                                              NULL);

        if (o == NULL) {
                g_warning ("pa_context_set_sink_input_volume() failed");
                return FALSE;
        }

        *op = o;

        return TRUE;
}