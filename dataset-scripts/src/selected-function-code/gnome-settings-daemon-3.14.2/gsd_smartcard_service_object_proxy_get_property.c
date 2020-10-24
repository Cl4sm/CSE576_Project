static void
gsd_smartcard_service_object_proxy_get_property (GObject      *gobject,
  guint         prop_id,
  GValue       *value,
  GParamSpec   *pspec)
{
  GsdSmartcardServiceObjectProxy *object = GSD_SMARTCARD_SERVICE_OBJECT_PROXY (gobject);
  GDBusInterface *interface;

  switch (prop_id)
    {
    case 1:
      interface = g_dbus_object_get_interface (G_DBUS_OBJECT (object), "org.gnome.SettingsDaemon.Smartcard.Manager");
      g_value_take_object (value, interface);
      break;

    case 2:
      interface = g_dbus_object_get_interface (G_DBUS_OBJECT (object), "org.gnome.SettingsDaemon.Smartcard.Driver");
      g_value_take_object (value, interface);
      break;

    case 3:
      interface = g_dbus_object_get_interface (G_DBUS_OBJECT (object), "org.gnome.SettingsDaemon.Smartcard.Token");
      g_value_take_object (value, interface);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
  }
}