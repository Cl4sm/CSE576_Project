static void
gvc_mixer_control_class_init (GvcMixerControlClass *klass)
{
        GObjectClass   *object_class = G_OBJECT_CLASS (klass);

        object_class->constructor = gvc_mixer_control_constructor;
        object_class->dispose = gvc_mixer_control_dispose;
        object_class->finalize = gvc_mixer_control_finalize;
        object_class->set_property = gvc_mixer_control_set_property;
        object_class->get_property = gvc_mixer_control_get_property;

        g_object_class_install_property (object_class,
                                         PROP_NAME,
                                         g_param_spec_string ("name",
                                                              "Name",
                                                              "Name to display for this mixer control",
                                                              NULL,
                                                              G_PARAM_READWRITE|G_PARAM_CONSTRUCT_ONLY));

        signals [STATE_CHANGED] =
                g_signal_new ("state-changed",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, state_changed),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [STREAM_ADDED] =
                g_signal_new ("stream-added",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, stream_added),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [STREAM_REMOVED] =
                g_signal_new ("stream-removed",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, stream_removed),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [CARD_ADDED] =
                g_signal_new ("card-added",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, card_added),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [CARD_REMOVED] =
                g_signal_new ("card-removed",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, card_removed),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [DEFAULT_SINK_CHANGED] =
                g_signal_new ("default-sink-changed",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, default_sink_changed),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [DEFAULT_SOURCE_CHANGED] =
                g_signal_new ("default-source-changed",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, default_source_changed),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [ACTIVE_OUTPUT_UPDATE] =
                g_signal_new ("active-output-update",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, active_output_update),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [ACTIVE_INPUT_UPDATE] =
                g_signal_new ("active-input-update",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, active_input_update),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [OUTPUT_ADDED] =
                g_signal_new ("output-added",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, output_added),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [INPUT_ADDED] =
                g_signal_new ("input-added",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, input_added),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [OUTPUT_REMOVED] =
                g_signal_new ("output-removed",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, output_removed),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        signals [INPUT_REMOVED] =
                g_signal_new ("input-removed",
                              G_TYPE_FROM_CLASS (klass),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GvcMixerControlClass, input_removed),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__UINT,
                              G_TYPE_NONE, 1, G_TYPE_UINT);
        g_type_class_add_private (klass, sizeof (GvcMixerControlPrivate));
}