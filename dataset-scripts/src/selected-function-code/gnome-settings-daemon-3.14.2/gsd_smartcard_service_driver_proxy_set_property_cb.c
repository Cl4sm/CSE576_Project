static void
gsd_smartcard_service_driver_proxy_set_property_cb (GDBusProxy *proxy,
  GAsyncResult *res,
  gpointer      user_data)
{
  const _ExtendedGDBusPropertyInfo *info = user_data;
  GError *error;
  GVariant *_ret;
  error = NULL;
  _ret = g_dbus_proxy_call_finish (proxy, res, &error);
  if (!_ret)
    {
      g_warning ("Error setting property '%s' on interface org.gnome.SettingsDaemon.Smartcard.Driver: %s (%s, %d)",
                 info->parent_struct.name, 
                 error->message, g_quark_to_string (error->domain), error->code);
      g_error_free (error);
    }
  else
    {
      g_variant_unref (_ret);
    }
}