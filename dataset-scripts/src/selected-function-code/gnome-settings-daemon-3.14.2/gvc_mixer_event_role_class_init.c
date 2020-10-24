static void
gvc_mixer_event_role_class_init (GvcMixerEventRoleClass *klass)
{
        GObjectClass        *object_class = G_OBJECT_CLASS (klass);
        GvcMixerStreamClass *stream_class = GVC_MIXER_STREAM_CLASS (klass);

        object_class->finalize = gvc_mixer_event_role_finalize;
        object_class->set_property = gvc_mixer_event_role_set_property;
        object_class->get_property = gvc_mixer_event_role_get_property;

        stream_class->push_volume = gvc_mixer_event_role_push_volume;
        stream_class->change_is_muted = gvc_mixer_event_role_change_is_muted;

        g_object_class_install_property (object_class,
                                         PROP_DEVICE,
                                         g_param_spec_string ("device",
                                                              "Device",
                                                              "Device",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));

        g_type_class_add_private (klass, sizeof (GvcMixerEventRolePrivate));
}