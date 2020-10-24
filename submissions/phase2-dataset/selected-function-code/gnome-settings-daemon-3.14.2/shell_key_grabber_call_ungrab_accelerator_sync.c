gboolean
shell_key_grabber_call_ungrab_accelerator_sync (
    ShellKeyGrabber *proxy,
    guint arg_action,
    gboolean *out_success,
    GCancellable *cancellable,
    GError **error)
{
  GVariant *_ret;
  _ret = g_dbus_proxy_call_sync (G_DBUS_PROXY (proxy),
    "UngrabAccelerator",
    g_variant_new ("(u)",
                   arg_action),
    G_DBUS_CALL_FLAGS_NONE,
    -1,
    cancellable,
    error);
  if (_ret == NULL)
    goto _out;
  g_variant_get (_ret,
                 "(b)",
                 out_success);
  g_variant_unref (_ret);
_out:
  return _ret != NULL;
}