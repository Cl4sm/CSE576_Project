static gboolean
gvc_mixer_sink_input_change_is_muted (GvcMixerStream *stream,
                                      gboolean        is_muted)
{
        pa_operation *o;
        guint         index;
        pa_context   *context;

        index = gvc_mixer_stream_get_index (stream);
        context = gvc_mixer_stream_get_pa_context (stream);

        o = pa_context_set_sink_input_mute (context,
                                            index,
                                            is_muted,
                                            NULL,
                                            NULL);

        if (o == NULL) {
                g_warning ("pa_context_set_sink_input_mute_by_index() failed");
                return FALSE;
        }

        pa_operation_unref(o);

        return TRUE;
}