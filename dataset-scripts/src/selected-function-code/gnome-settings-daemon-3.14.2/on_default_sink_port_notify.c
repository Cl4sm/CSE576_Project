static void
on_default_sink_port_notify (GObject        *object,
                             GParamSpec     *pspec,
                             GvcMixerControl *control)
{
        char             *port;
        GvcMixerUIDevice *output;

        g_object_get (object, "port", &port, NULL);

        output = gvc_mixer_control_lookup_device_from_stream (control,
                                                              GVC_MIXER_STREAM (object));
        if (output != NULL) {
                g_debug ("on_default_sink_port_notify - moved to port %s - which SHOULD correspond to output %s",
                         port,
                         gvc_mixer_ui_device_get_description (output));
                g_signal_emit (G_OBJECT (control),
                               signals[ACTIVE_OUTPUT_UPDATE],
                               0,
                               gvc_mixer_ui_device_get_id (output));
        }
        g_free (port);
}