static void
mcd_channel_class_init (McdChannelClass * klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    McdMissionClass *mission_class = MCD_MISSION_CLASS (klass);
    g_type_class_add_private (object_class, sizeof (McdChannelPrivate));

    object_class->constructed = _mcd_channel_constructed;
    object_class->dispose = _mcd_channel_dispose;
    object_class->finalize = _mcd_channel_finalize;
    object_class->set_property = _mcd_channel_set_property;
    object_class->get_property = _mcd_channel_get_property;
    mission_class->abort = mcd_channel_abort;
    klass->status_changed_signal = mcd_channel_status_changed;

    /* signals */
    mcd_channel_signals[STATUS_CHANGED] =
	g_signal_new ("status-changed",
		      G_OBJECT_CLASS_TYPE (klass),
		      G_SIGNAL_RUN_FIRST,
		      G_STRUCT_OFFSET (McdChannelClass,
				       status_changed_signal),
		      NULL, NULL, g_cclosure_marshal_VOID__INT, G_TYPE_NONE,
		      1, G_TYPE_INT);

    /* properties */
    g_object_class_install_property
        (object_class, PROP_TP_CHANNEL,
         g_param_spec_object ("tp-channel",
                              "Telepathy Channel",
                              "Telepathy Channel",
                              TP_TYPE_CHANNEL,
                              G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property
        (object_class, PROP_OUTGOING,
         g_param_spec_boolean ("outgoing",
                               "Outgoing channel",
                               "True if the channel was requested by us",
                               FALSE,
                               G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property
        (object_class, PROP_ACCOUNT_PATH,
         g_param_spec_boxed ("account-path",
                             "Account",
                             "Object path of the Account",
                             DBUS_TYPE_G_OBJECT_PATH,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property
        (object_class, PROP_PREFERRED_HANDLER,
         g_param_spec_string ("preferred-handler",
                             "PreferredHandler",
                             "Well-known bus name of the preferred Handler",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property
        (object_class, PROP_USER_ACTION_TIME,
         g_param_spec_int64 ("user-action-time",
                             "UserActionTime",
                             "Time of user action",
                             G_MININT64, G_MAXINT64,
                             TP_USER_ACTION_TIME_NOT_USER_ACTION,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property
        (object_class, PROP_REQUESTS,
         g_param_spec_boxed ("requests",
                             "Requests",
                             "A dbus-glib aa{sv}",
                             TP_ARRAY_TYPE_QUALIFIED_PROPERTY_VALUE_MAP_LIST,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property
        (object_class, PROP_INTERFACES,
         g_param_spec_boxed ("interfaces",
                             "Interfaces",
                             "A dbus-glib 'as'",
                             G_TYPE_STRV,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property
        (object_class, PROP_HINTS,
         g_param_spec_boxed ("hints",
                             "Hints",
                             "GHashTable",
                             TP_HASH_TYPE_STRING_VARIANT_MAP,
                             G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));
}