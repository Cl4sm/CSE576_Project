static void
_mcd_client_registry_class_init (McdClientRegistryClass *cls)
{
  GObjectClass *object_class = G_OBJECT_CLASS (cls);

  g_type_class_add_private (cls, sizeof (McdClientRegistryPrivate));

  object_class->constructed = mcd_client_registry_constructed;
  object_class->get_property = mcd_client_registry_get_property;
  object_class->set_property = mcd_client_registry_set_property;
  object_class->dispose = mcd_client_registry_dispose;

  g_object_class_install_property (object_class, PROP_DBUS_DAEMON,
      g_param_spec_object ("dbus-daemon", "D-Bus daemon", "D-Bus daemon",
        TP_TYPE_DBUS_DAEMON,
        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS));

  signals[S_CLIENT_ADDED] = g_signal_new ("client-added",
      G_OBJECT_CLASS_TYPE (cls),
      G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
      0, NULL, NULL,
      g_cclosure_marshal_VOID__OBJECT,
      G_TYPE_NONE, 1, MCD_TYPE_CLIENT_PROXY);

  signals[S_READY] = g_signal_new ("ready",
      G_OBJECT_CLASS_TYPE (cls),
      G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
      0, NULL, NULL,
      g_cclosure_marshal_VOID__VOID,
      G_TYPE_NONE, 0);
}