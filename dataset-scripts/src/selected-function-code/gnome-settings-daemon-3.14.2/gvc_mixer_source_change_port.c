static gboolean
gvc_mixer_source_change_port (GvcMixerStream *stream,
                              const char     *port)
{
        pa_operation *o;
        guint         index;
        pa_context   *context;

        index = gvc_mixer_stream_get_index (stream);
        context = gvc_mixer_stream_get_pa_context (stream);

        o = pa_context_set_source_port_by_index (context,
                                                 index,
                                                 port,
                                                 NULL,
                                                 NULL);

        if (o == NULL) {
                g_warning ("pa_context_set_source_port_by_index() failed: %s", pa_strerror(pa_context_errno(context)));
                return FALSE;
        }

        pa_operation_unref(o);

        return TRUE;
}