static void
_mcd_request_class_init (
    McdRequestClass *cls)
{
  static TpDBusPropertiesMixinPropImpl request_props[] = {
      { "Account", "account-path", NULL },
      { "UserActionTime", "user-action-time", NULL },
      { "PreferredHandler", "preferred-handler", NULL },
      { "Interfaces", "interfaces", NULL },
      { "Requests", "requests", NULL },
      { "Hints", "hints", NULL },
      { NULL }
  };
  static TpDBusPropertiesMixinIfaceImpl prop_interfaces[] = {
      { TP_IFACE_CHANNEL_REQUEST,
          tp_dbus_properties_mixin_getter_gobject_properties,
          NULL,
          request_props,
      },
      { NULL }
  };
  GObjectClass *object_class = (GObjectClass *) cls;

  object_class->constructed = _mcd_request_constructed;
  object_class->get_property = _mcd_request_get_property;
  object_class->set_property = _mcd_request_set_property;
  object_class->dispose = _mcd_request_dispose;
  object_class->finalize = _mcd_request_finalize;

  g_object_class_install_property (object_class, PROP_USE_EXISTING,
      g_param_spec_boolean ("use-existing", "Use EnsureChannel?",
          "TRUE if EnsureChannel should be used for this request",
          FALSE,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
          G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_CLIENT_REGISTRY,
      g_param_spec_object ("client-registry", "Client registry",
          "The client registry",
          MCD_TYPE_CLIENT_REGISTRY,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
          G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_ACCOUNT,
      g_param_spec_object ("account", "Account",
          "The underlying McdAccount",
          MCD_TYPE_ACCOUNT,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
          G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_ACCOUNT_PATH,
      g_param_spec_boxed ("account-path", "Account path",
          "The object path of McdRequest:account",
          DBUS_TYPE_G_OBJECT_PATH,
          G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_PROPERTIES,
       g_param_spec_boxed ("properties", "Properties",
         "Properties requested for the channel",
         TP_HASH_TYPE_QUALIFIED_PROPERTY_VALUE_MAP,
         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_USER_ACTION_TIME,
       g_param_spec_int64 ("user-action-time", "UserActionTime",
         "Time of user action as for TpAccountChannelRequest:user-action-time",
         G_MININT64, G_MAXINT64, TP_USER_ACTION_TIME_NOT_USER_ACTION,
         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_PREFERRED_HANDLER,
       g_param_spec_string ("preferred-handler", "PreferredHandler",
         "Preferred handler for this request, or the empty string",
         "",
         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_HINTS,
      g_param_spec_boxed ("hints", "Hints",
        "GHashTable",
        TP_HASH_TYPE_STRING_VARIANT_MAP,
        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_REQUESTS,
      g_param_spec_boxed ("requests", "Requests", "A dbus-glib aa{sv}",
        TP_ARRAY_TYPE_QUALIFIED_PROPERTY_VALUE_MAP_LIST,
        G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_INTERFACES,
      g_param_spec_boxed ("interfaces", "Interfaces", "A dbus-glib 'as'",
        G_TYPE_STRV, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  sig_id_cancelling = g_signal_new ("cancelling",
      G_OBJECT_CLASS_TYPE (cls), G_SIGNAL_RUN_LAST, 0, NULL, NULL,
      g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

  sig_id_ready_to_request = g_signal_new ("ready-to-request",
      G_OBJECT_CLASS_TYPE (cls), G_SIGNAL_RUN_LAST, 0, NULL, NULL,
      g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

  cls->dbus_properties_class.interfaces = prop_interfaces,
  tp_dbus_properties_mixin_class_init (object_class,
      G_STRUCT_OFFSET (McdRequestClass, dbus_properties_class));
}