static void
gsd_smartcard_service_object_skeleton_set_property (GObject      *gobject,
  guint         prop_id,
  const GValue *value,
  GParamSpec   *pspec)
{
  GsdSmartcardServiceObjectSkeleton *object = GSD_SMARTCARD_SERVICE_OBJECT_SKELETON (gobject);
  GDBusInterfaceSkeleton *interface;

  switch (prop_id)
    {
    case 1:
      interface = g_value_get_object (value);
      if (interface != NULL)
        {
          g_warn_if_fail (GSD_SMARTCARD_SERVICE_IS_MANAGER (interface));
          g_dbus_object_skeleton_add_interface (G_DBUS_OBJECT_SKELETON (object), interface);
        }
      else
        {
          g_dbus_object_skeleton_remove_interface_by_name (G_DBUS_OBJECT_SKELETON (object), "org.gnome.SettingsDaemon.Smartcard.Manager");
        }
      break;

    case 2:
      interface = g_value_get_object (value);
      if (interface != NULL)
        {
          g_warn_if_fail (GSD_SMARTCARD_SERVICE_IS_DRIVER (interface));
          g_dbus_object_skeleton_add_interface (G_DBUS_OBJECT_SKELETON (object), interface);
        }
      else
        {
          g_dbus_object_skeleton_remove_interface_by_name (G_DBUS_OBJECT_SKELETON (object), "org.gnome.SettingsDaemon.Smartcard.Driver");
        }
      break;

    case 3:
      interface = g_value_get_object (value);
      if (interface != NULL)
        {
          g_warn_if_fail (GSD_SMARTCARD_SERVICE_IS_TOKEN (interface));
          g_dbus_object_skeleton_add_interface (G_DBUS_OBJECT_SKELETON (object), interface);
        }
      else
        {
          g_dbus_object_skeleton_remove_interface_by_name (G_DBUS_OBJECT_SKELETON (object), "org.gnome.SettingsDaemon.Smartcard.Token");
        }
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
  }
}