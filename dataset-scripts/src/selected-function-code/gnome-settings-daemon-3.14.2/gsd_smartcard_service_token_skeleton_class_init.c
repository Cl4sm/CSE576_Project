static void
gsd_smartcard_service_token_skeleton_class_init (GsdSmartcardServiceTokenSkeletonClass *klass)
{
  GObjectClass *gobject_class;
  GDBusInterfaceSkeletonClass *skeleton_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = gsd_smartcard_service_token_skeleton_finalize;
  gobject_class->get_property = gsd_smartcard_service_token_skeleton_get_property;
  gobject_class->set_property = gsd_smartcard_service_token_skeleton_set_property;
  gobject_class->notify       = gsd_smartcard_service_token_skeleton_notify;


  gsd_smartcard_service_token_override_properties (gobject_class, 1);

  skeleton_class = G_DBUS_INTERFACE_SKELETON_CLASS (klass);
  skeleton_class->get_info = gsd_smartcard_service_token_skeleton_dbus_interface_get_info;
  skeleton_class->get_properties = gsd_smartcard_service_token_skeleton_dbus_interface_get_properties;
  skeleton_class->flush = gsd_smartcard_service_token_skeleton_dbus_interface_flush;
  skeleton_class->get_vtable = gsd_smartcard_service_token_skeleton_dbus_interface_get_vtable;

#if GLIB_VERSION_MAX_ALLOWED < GLIB_VERSION_2_38
  g_type_class_add_private (klass, sizeof (GsdSmartcardServiceTokenSkeletonPrivate));
#endif
}