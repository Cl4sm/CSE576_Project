gboolean
gsd_smartcard_service_manager_call_get_inserted_tokens_sync (
    GsdSmartcardServiceManager *proxy,
    gchar ***out_tokens,
    GCancellable *cancellable,
    GError **error)
{
  GVariant *_ret;
  _ret = g_dbus_proxy_call_sync (G_DBUS_PROXY (proxy),
    "GetInsertedTokens",
    g_variant_new ("()"),
    G_DBUS_CALL_FLAGS_NONE,
    -1,
    cancellable,
    error);
  if (_ret == NULL)
    goto _out;
  g_variant_get (_ret,
                 "(^ao)",
                 out_tokens);
  g_variant_unref (_ret);
_out:
  return _ret != NULL;
}