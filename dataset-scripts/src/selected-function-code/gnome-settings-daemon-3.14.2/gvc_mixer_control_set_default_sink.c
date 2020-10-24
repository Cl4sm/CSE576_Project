gboolean
gvc_mixer_control_set_default_sink (GvcMixerControl *control,
                                    GvcMixerStream  *stream)
{
        pa_operation *o;

        g_return_val_if_fail (GVC_IS_MIXER_CONTROL (control), FALSE);
        g_return_val_if_fail (GVC_IS_MIXER_STREAM (stream), FALSE);

        g_debug ("about to set default sink on server");
        o = pa_context_set_default_sink (control->priv->pa_context,
                                         gvc_mixer_stream_get_name (stream),
                                         NULL,
                                         NULL);
        if (o == NULL) {
                g_warning ("pa_context_set_default_sink() failed: %s",
                           pa_strerror (pa_context_errno (control->priv->pa_context)));
                return FALSE;
        }

        pa_operation_unref (o);

        control->priv->new_default_sink_stream = stream;
        g_object_add_weak_pointer (G_OBJECT (stream), (gpointer *) &control->priv->new_default_sink_stream);

        o = pa_ext_stream_restore_read (control->priv->pa_context,
                                        gvc_mixer_control_stream_restore_sink_cb,
                                        control);

        if (o == NULL) {
                g_warning ("pa_ext_stream_restore_read() failed: %s",
                           pa_strerror (pa_context_errno (control->priv->pa_context)));
                return FALSE;
        }

        pa_operation_unref (o);

        return TRUE;
}