static void
gvc_mixer_stream_class_init (GvcMixerStreamClass *klass)
{
        GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);

        gobject_class->constructor = gvc_mixer_stream_constructor;
        gobject_class->finalize = gvc_mixer_stream_finalize;
        gobject_class->set_property = gvc_mixer_stream_set_property;
        gobject_class->get_property = gvc_mixer_stream_get_property;

        klass->push_volume = gvc_mixer_stream_real_push_volume;
        klass->change_port = gvc_mixer_stream_real_change_port;
        klass->change_is_muted = gvc_mixer_stream_real_change_is_muted;

        g_object_class_install_property (gobject_class,
                                         PROP_INDEX,
                                         g_param_spec_ulong ("index",
                                                             "Index",
                                                             "The index for this stream",
                                                             0, G_MAXULONG, 0,
                                                             G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));
        g_object_class_install_property (gobject_class,
                                         PROP_ID,
                                         g_param_spec_ulong ("id",
                                                             "id",
                                                             "The id for this stream",
                                                             0, G_MAXULONG, 0,
                                                             G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));
        g_object_class_install_property (gobject_class,
                                         PROP_CHANNEL_MAP,
                                         g_param_spec_object ("channel-map",
                                                              "channel map",
                                                              "The channel map for this stream",
                                                              GVC_TYPE_CHANNEL_MAP,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_PA_CONTEXT,
                                         g_param_spec_pointer ("pa-context",
                                                               "PulseAudio context",
                                                               "The PulseAudio context for this stream",
                                                               G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));
        g_object_class_install_property (gobject_class,
                                         PROP_VOLUME,
                                         g_param_spec_ulong ("volume",
                                                             "Volume",
                                                             "The volume for this stream",
                                                             0, G_MAXULONG, 0,
                                                             G_PARAM_READWRITE));
        g_object_class_install_property (gobject_class,
                                         PROP_DECIBEL,
                                         g_param_spec_double ("decibel",
                                                              "Decibel",
                                                              "The decibel level for this stream",
                                                              -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));

        g_object_class_install_property (gobject_class,
                                         PROP_NAME,
                                         g_param_spec_string ("name",
                                                              "Name",
                                                              "Name to display for this stream",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_DESCRIPTION,
                                         g_param_spec_string ("description",
                                                              "Description",
                                                              "Description to display for this stream",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_APPLICATION_ID,
                                         g_param_spec_string ("application-id",
                                                              "Application identifier",
                                                              "Application identifier for this stream",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_ICON_NAME,
                                         g_param_spec_string ("icon-name",
                                                              "Icon Name",
                                                              "Name of icon to display for this stream",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_FORM_FACTOR,
                                         g_param_spec_string ("form-factor",
                                                              "Form Factor",
                                                              "Device form factor for this stream, as reported by PulseAudio",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_SYSFS_PATH,
                                         g_param_spec_string ("sysfs-path",
                                                              "Sysfs path",
                                                              "Sysfs path for the device associated with this stream",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_IS_MUTED,
                                         g_param_spec_boolean ("is-muted",
                                                               "is muted",
                                                               "Whether stream is muted",
                                                               FALSE,
                                                               G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_CAN_DECIBEL,
                                         g_param_spec_boolean ("can-decibel",
                                                               "can decibel",
                                                               "Whether stream volume can be converted to decibel units",
                                                               FALSE,
                                                               G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_IS_EVENT_STREAM,
                                         g_param_spec_boolean ("is-event-stream",
                                                               "is event stream",
                                                               "Whether stream's role is to play an event",
                                                               FALSE,
                                                               G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_IS_VIRTUAL,
                                         g_param_spec_boolean ("is-virtual",
                                                               "is virtual stream",
                                                               "Whether the stream is virtual",
                                                               FALSE,
                                                               G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_PORT,
                                         g_param_spec_string ("port",
                                                              "Port",
                                                              "The name of the current port for this stream",
                                                              NULL,
                                                              G_PARAM_READWRITE));
        g_object_class_install_property (gobject_class,
                                         PROP_CARD_INDEX,
                                         g_param_spec_long ("card-index",
                                                             "Card index",
                                                             "The index of the card for this stream",
                                                             PA_INVALID_INDEX, G_MAXLONG, PA_INVALID_INDEX,
                                                             G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_type_class_add_private (klass, sizeof (GvcMixerStreamPrivate));
}