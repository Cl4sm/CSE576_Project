gboolean
gvc_mixer_control_set_default_source (GvcMixerControl *control,
                                      GvcMixerStream  *stream)
{
        GvcMixerUIDevice* input;
        pa_operation *o;

        g_return_val_if_fail (GVC_IS_MIXER_CONTROL (control), FALSE);
        g_return_val_if_fail (GVC_IS_MIXER_STREAM (stream), FALSE);

        o = pa_context_set_default_source (control->priv->pa_context,
                                           gvc_mixer_stream_get_name (stream),
                                           NULL,
                                           NULL);
        if (o == NULL) {
                g_warning ("pa_context_set_default_source() failed");
                return FALSE;
        }

        pa_operation_unref (o);

        control->priv->new_default_source_stream = stream;
        g_object_add_weak_pointer (G_OBJECT (stream), (gpointer *) &control->priv->new_default_source_stream);

        o = pa_ext_stream_restore_read (control->priv->pa_context,
                                        gvc_mixer_control_stream_restore_source_cb,
                                        control);

        if (o == NULL) {
                g_warning ("pa_ext_stream_restore_read() failed: %s",
                           pa_strerror (pa_context_errno (control->priv->pa_context)));
                return FALSE;
        }

        pa_operation_unref (o);

        /* source change successful, update the UI. */
        input = gvc_mixer_control_lookup_device_from_stream (control, stream);
        g_signal_emit (G_OBJECT (control),
                       signals[ACTIVE_INPUT_UPDATE],
                       0,
                       gvc_mixer_ui_device_get_id (input));

        return TRUE;
}