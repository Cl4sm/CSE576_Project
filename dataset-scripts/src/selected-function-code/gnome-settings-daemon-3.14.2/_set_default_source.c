static void
_set_default_source (GvcMixerControl *control,
                     GvcMixerStream  *stream)
{
        guint new_id;

        if (stream == NULL) {
                control->priv->default_source_id = 0;
                control->priv->default_source_is_set = FALSE;
                g_signal_emit (control,
                               signals[DEFAULT_SOURCE_CHANGED],
                               0,
                               PA_INVALID_INDEX);
                return;
        }

        new_id = gvc_mixer_stream_get_id (stream);

        if (control->priv->default_source_id != new_id) {
                GvcMixerUIDevice *input;
                control->priv->default_source_id = new_id;
                control->priv->default_source_is_set = TRUE;
                g_signal_emit (control,
                               signals[DEFAULT_SOURCE_CHANGED],
                               0,
                               new_id);

                if (control->priv->default_source_is_set) {
                        g_signal_handlers_disconnect_by_func (gvc_mixer_control_get_default_source (control),
                                                              on_default_source_port_notify,
                                                              control);
                }

                g_signal_connect (stream,
                                  "notify::port",
                                  G_CALLBACK (on_default_source_port_notify),
                                  control);

                input = gvc_mixer_control_lookup_device_from_stream (control, stream);

                g_signal_emit (G_OBJECT (control),
                               signals[ACTIVE_INPUT_UPDATE],
                               0,
                               gvc_mixer_ui_device_get_id (input));
        }
}