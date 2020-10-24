void
gvc_mixer_control_change_input (GvcMixerControl *control,
                                GvcMixerUIDevice* input)
{
        GvcMixerStream           *stream;
        GvcMixerStream           *default_stream;
        const GvcMixerStreamPort *active_port;
        const gchar              *input_port;

        stream = gvc_mixer_control_get_stream_from_device (control, input);
        if (stream == NULL) {
                gvc_mixer_control_change_profile_on_selected_device (control,
                        input, NULL);
                return;
        }

        /* Handle a network sink as a portless/cardless device */
        if (!gvc_mixer_ui_device_has_ports (input)) {
                g_debug ("Did we try to move to a software/bluetooth source ?");
                if (! gvc_mixer_control_set_default_source (control, stream)) {
                        g_warning ("Failed to set default source with stream from input %s",
                                   gvc_mixer_ui_device_get_description (input));
                }
                return;
        }

        active_port = gvc_mixer_stream_get_port (stream);
        input_port = gvc_mixer_ui_device_get_port (input);
        /* First ensure the correct port is active on the sink */
        if (g_strcmp0 (active_port->port, input_port) != 0) {
                g_debug ("Port change, switch to = %s", input_port);
                if (gvc_mixer_stream_change_port (stream, input_port) == FALSE) {
                        g_warning ("Could not change port!");
                        return;
                }
        }

        default_stream = gvc_mixer_control_get_default_source (control);

        /* Finally if we are not on the correct stream, swap over. */
        if (stream != default_stream) {
                g_debug ("change-input - attempting to swap over to stream %s",
                         gvc_mixer_stream_get_description (stream));
                gvc_mixer_control_set_default_source (control, stream);
        }
}