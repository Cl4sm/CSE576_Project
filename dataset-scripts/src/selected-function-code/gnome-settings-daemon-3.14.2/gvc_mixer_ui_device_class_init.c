static void
gvc_mixer_ui_device_class_init (GvcMixerUIDeviceClass *klass)
{
        GObjectClass* object_class = G_OBJECT_CLASS (klass);
        GParamSpec *pspec;

        object_class->constructor = gvc_mixer_ui_device_constructor;
        object_class->dispose = gvc_mixer_ui_device_dispose;
        object_class->finalize = gvc_mixer_ui_device_finalize;
        object_class->set_property = gvc_mixer_ui_device_set_property;
        object_class->get_property = gvc_mixer_ui_device_get_property;

        pspec = g_param_spec_string ("description",
                                     "Description construct prop",
                                     "Set first line description",
                                     "no-name-set",
                                     G_PARAM_READWRITE);
        g_object_class_install_property (object_class, PROP_DESC_LINE_1, pspec);

        pspec = g_param_spec_string ("origin",
                                     "origin construct prop",
                                     "Set second line description name",
                                     "no-name-set",
                                     G_PARAM_READWRITE);
        g_object_class_install_property (object_class, PROP_DESC_LINE_2, pspec);

        pspec = g_param_spec_pointer ("card",
                                      "Card from pulse",
                                      "Set/Get card",
                                      G_PARAM_READWRITE);

        g_object_class_install_property (object_class, PROP_CARD, pspec);

        pspec = g_param_spec_string ("port-name",
                                     "port-name construct prop",
                                     "Set port-name",
                                     NULL,
                                     G_PARAM_READWRITE);
        g_object_class_install_property (object_class, PROP_PORT_NAME, pspec);

        pspec = g_param_spec_int ("stream-id",
                                  "stream id assigned by gvc-stream",
                                  "Set/Get stream id",
                                  -1,
                                   G_MAXINT,
                                   GVC_MIXER_UI_DEVICE_INVALID,
                                   G_PARAM_READWRITE);
        g_object_class_install_property (object_class, PROP_STREAM_ID, pspec);

        pspec = g_param_spec_uint ("type",
                                   "ui-device type",
                                   "determine whether its an input and output",
                                   0, 1, 0, G_PARAM_READWRITE);
        g_object_class_install_property (object_class, PROP_UI_DEVICE_TYPE, pspec);

        pspec = g_param_spec_boolean ("port-available",
                                      "available",
                                      "determine whether this port is available",
                                      FALSE,
                                      G_PARAM_READWRITE);
        g_object_class_install_property (object_class, PROP_PORT_AVAILABLE, pspec);

        g_type_class_add_private (klass, sizeof (GvcMixerUIDevicePrivate));
}