static GVariant *
gsd_smartcard_service_token_skeleton_dbus_interface_get_properties (GDBusInterfaceSkeleton *_skeleton)
{
  GsdSmartcardServiceTokenSkeleton *skeleton = GSD_SMARTCARD_SERVICE_TOKEN_SKELETON (_skeleton);

  GVariantBuilder builder;
  guint n;
  g_variant_builder_init (&builder, G_VARIANT_TYPE ("a{sv}"));
  if (_gsd_smartcard_service_token_interface_info.parent_struct.properties == NULL)
    goto out;
  for (n = 0; _gsd_smartcard_service_token_interface_info.parent_struct.properties[n] != NULL; n++)
    {
      GDBusPropertyInfo *info = _gsd_smartcard_service_token_interface_info.parent_struct.properties[n];
      if (info->flags & G_DBUS_PROPERTY_INFO_FLAGS_READABLE)
        {
          GVariant *value;
          value = _gsd_smartcard_service_token_skeleton_handle_get_property (g_dbus_interface_skeleton_get_connection (G_DBUS_INTERFACE_SKELETON (skeleton)), NULL, g_dbus_interface_skeleton_get_object_path (G_DBUS_INTERFACE_SKELETON (skeleton)), "org.gnome.SettingsDaemon.Smartcard.Token", info->name, NULL, skeleton);
          if (value != NULL)
            {
              g_variant_take_ref (value);
              g_variant_builder_add (&builder, "{sv}", info->name, value);
              g_variant_unref (value);
            }
        }
    }
out:
  return g_variant_builder_end (&builder);
}