static void
on_default_source_port_notify (GObject        *object,
                               GParamSpec     *pspec,
                               GvcMixerControl *control)
{
        char             *port;
        GvcMixerUIDevice *input;

        g_object_get (object, "port", &port, NULL);
        input = gvc_mixer_control_lookup_device_from_stream (control,
                                                             GVC_MIXER_STREAM (object));

        g_debug ("on_default_source_port_notify - moved to port '%s' which SHOULD ?? correspond to output '%s'",
                 port,
                 gvc_mixer_ui_device_get_description (input));

        g_signal_emit (G_OBJECT (control),
                       signals[ACTIVE_INPUT_UPDATE],
                       0,
                       gvc_mixer_ui_device_get_id (input));

        g_free (port);
}