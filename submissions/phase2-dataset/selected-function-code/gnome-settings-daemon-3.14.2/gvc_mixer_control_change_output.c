void
gvc_mixer_control_change_output (GvcMixerControl *control,
                                 GvcMixerUIDevice* output)
{
        GvcMixerStream           *stream;
        GvcMixerStream           *default_stream;
        const GvcMixerStreamPort *active_port;
        const gchar              *output_port;

        g_debug ("control change output");

        stream = gvc_mixer_control_get_stream_from_device (control, output);
        if (stream == NULL) {
                gvc_mixer_control_change_profile_on_selected_device (control,
                        output, NULL);
                return;
        }

        /* Handle a network sink as a portless or cardless device */
        if (!gvc_mixer_ui_device_has_ports (output)) {
                g_debug ("Did we try to move to a software/bluetooth sink ?");
                if (gvc_mixer_control_set_default_sink (control, stream)) {
                        /* sink change was successful,  update the UI.*/
                        g_signal_emit (G_OBJECT (control),
                                       signals[ACTIVE_OUTPUT_UPDATE],
                                       0,
                                       gvc_mixer_ui_device_get_id (output));
                }
                else {
                        g_warning ("Failed to set default sink with stream from output %s",
                                   gvc_mixer_ui_device_get_description (output));
                }
                return;
        }

        active_port = gvc_mixer_stream_get_port (stream);
        output_port = gvc_mixer_ui_device_get_port (output);
        /* First ensure the correct port is active on the sink */
        if (g_strcmp0 (active_port->port, output_port) != 0) {
                g_debug ("Port change, switch to = %s", output_port);
                if (gvc_mixer_stream_change_port (stream, output_port) == FALSE) {
                        g_warning ("Could not change port !");
                        return;
                }
        }

        default_stream = gvc_mixer_control_get_default_sink (control);

        /* Finally if we are not on the correct stream, swap over. */
        if (stream != default_stream) {
                GvcMixerUIDevice* output;

                g_debug ("Attempting to swap over to stream %s ",
                         gvc_mixer_stream_get_description (stream));
                if (gvc_mixer_control_set_default_sink (control, stream)) {
                        output = gvc_mixer_control_lookup_device_from_stream (control, stream);
                        g_signal_emit (G_OBJECT (control),
                                       signals[ACTIVE_OUTPUT_UPDATE],
                                       0,
                                       gvc_mixer_ui_device_get_id (output));
                } else {
                        /* If the move failed for some reason reset the UI. */
                        output = gvc_mixer_control_lookup_device_from_stream (control, default_stream);
                        g_signal_emit (G_OBJECT (control),
                                       signals[ACTIVE_OUTPUT_UPDATE],
                                       0,
                                       gvc_mixer_ui_device_get_id (output));
                }
        }
}