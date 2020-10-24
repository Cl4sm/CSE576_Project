gboolean
shell_key_grabber_call_grab_accelerators_sync (
    ShellKeyGrabber *proxy,
    GVariant *arg_accelerators,
    GVariant **out_actions,
    GCancellable *cancellable,
    GError **error)
{
  GVariant *_ret;
  _ret = g_dbus_proxy_call_sync (G_DBUS_PROXY (proxy),
    "GrabAccelerators",
    g_variant_new ("(@a(su))",
                   arg_accelerators),
    G_DBUS_CALL_FLAGS_NONE,
    -1,
    cancellable,
    error);
  if (_ret == NULL)
    goto _out;
  g_variant_get (_ret,
                 "(@au)",
                 out_actions);
  g_variant_unref (_ret);
_out:
  return _ret != NULL;
}