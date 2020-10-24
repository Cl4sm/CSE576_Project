gboolean
gsd_smartcard_service_manager_call_get_login_token_sync (
    GsdSmartcardServiceManager *proxy,
    gchar **out_token,
    GCancellable *cancellable,
    GError **error)
{
  GVariant *_ret;
  _ret = g_dbus_proxy_call_sync (G_DBUS_PROXY (proxy),
    "GetLoginToken",
    g_variant_new ("()"),
    G_DBUS_CALL_FLAGS_NONE,
    -1,
    cancellable,
    error);
  if (_ret == NULL)
    goto _out;
  g_variant_get (_ret,
                 "(o)",
                 out_token);
  g_variant_unref (_ret);
_out:
  return _ret != NULL;
}