static void
_set_default_sink (GvcMixerControl *control,
                   GvcMixerStream  *stream)
{
        guint new_id;

        if (stream == NULL) {
                /* Don't tell front-ends about an unset default
                 * sink if it's already unset */
                if (control->priv->default_sink_is_set == FALSE)
                        return;
                control->priv->default_sink_id = 0;
                control->priv->default_sink_is_set = FALSE;
                g_signal_emit (control,
                               signals[DEFAULT_SINK_CHANGED],
                               0,
                               PA_INVALID_INDEX);
                return;
        }

        new_id = gvc_mixer_stream_get_id (stream);

        if (control->priv->default_sink_id != new_id) {
                GvcMixerUIDevice *output;
                if (control->priv->default_sink_is_set) {
                        g_signal_handlers_disconnect_by_func (gvc_mixer_control_get_default_sink (control),
                                                              on_default_sink_port_notify,
                                                              control);
                }

                control->priv->default_sink_id = new_id;

                control->priv->default_sink_is_set = TRUE;
                g_signal_emit (control,
                               signals[DEFAULT_SINK_CHANGED],
                               0,
                               new_id);

                g_signal_connect (stream,
                                  "notify::port",
                                  G_CALLBACK (on_default_sink_port_notify),
                                  control);

                output = gvc_mixer_control_lookup_device_from_stream (control, stream);

                g_debug ("active_sink change");

                g_signal_emit (G_OBJECT (control),
                               signals[ACTIVE_OUTPUT_UPDATE],
                               0,
                               gvc_mixer_ui_device_get_id (output));
        }
}