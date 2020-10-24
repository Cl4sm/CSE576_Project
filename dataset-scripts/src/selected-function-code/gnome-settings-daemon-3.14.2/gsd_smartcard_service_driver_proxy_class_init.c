static void
gsd_smartcard_service_driver_proxy_class_init (GsdSmartcardServiceDriverProxyClass *klass)
{
  GObjectClass *gobject_class;
  GDBusProxyClass *proxy_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize     = gsd_smartcard_service_driver_proxy_finalize;
  gobject_class->get_property = gsd_smartcard_service_driver_proxy_get_property;
  gobject_class->set_property = gsd_smartcard_service_driver_proxy_set_property;

  proxy_class = G_DBUS_PROXY_CLASS (klass);
  proxy_class->g_signal = gsd_smartcard_service_driver_proxy_g_signal;
  proxy_class->g_properties_changed = gsd_smartcard_service_driver_proxy_g_properties_changed;

  gsd_smartcard_service_driver_override_properties (gobject_class, 1);

#if GLIB_VERSION_MAX_ALLOWED < GLIB_VERSION_2_38
  g_type_class_add_private (klass, sizeof (GsdSmartcardServiceDriverProxyPrivate));
#endif
}