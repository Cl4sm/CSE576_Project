static void
gvc_mixer_card_class_init (GvcMixerCardClass *klass)
{
        GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);

        gobject_class->constructor = gvc_mixer_card_constructor;
        gobject_class->finalize = gvc_mixer_card_finalize;

        gobject_class->set_property = gvc_mixer_card_set_property;
        gobject_class->get_property = gvc_mixer_card_get_property;

        g_object_class_install_property (gobject_class,
                                         PROP_INDEX,
                                         g_param_spec_ulong ("index",
                                                             "Index",
                                                             "The index for this card",
                                                             0, G_MAXULONG, 0,
                                                             G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));
        g_object_class_install_property (gobject_class,
                                         PROP_ID,
                                         g_param_spec_ulong ("id",
                                                             "id",
                                                             "The id for this card",
                                                             0, G_MAXULONG, 0,
                                                             G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));
        g_object_class_install_property (gobject_class,
                                         PROP_PA_CONTEXT,
                                         g_param_spec_pointer ("pa-context",
                                                               "PulseAudio context",
                                                               "The PulseAudio context for this card",
                                                               G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));
        g_object_class_install_property (gobject_class,
                                         PROP_NAME,
                                         g_param_spec_string ("name",
                                                              "Name",
                                                              "Name to display for this card",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_ICON_NAME,
                                         g_param_spec_string ("icon-name",
                                                              "Icon Name",
                                                              "Name of icon to display for this card",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT));
        g_object_class_install_property (gobject_class,
                                         PROP_PROFILE,
                                         g_param_spec_string ("profile",
                                                              "Profile",
                                                              "Name of current profile for this card",
                                                              NULL,
                                                              G_PARAM_READWRITE));
        g_object_class_install_property (gobject_class,
                                         PROP_HUMAN_PROFILE,
                                         g_param_spec_string ("human-profile",
                                                              "Profile (Human readable)",
                                                              "Name of current profile for this card in human readable form",
                                                              NULL,
                                                              G_PARAM_READABLE));

        g_type_class_add_private (klass, sizeof (GvcMixerCardPrivate));
}