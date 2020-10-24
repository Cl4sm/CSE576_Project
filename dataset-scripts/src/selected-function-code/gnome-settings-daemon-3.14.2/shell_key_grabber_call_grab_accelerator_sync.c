gboolean
shell_key_grabber_call_grab_accelerator_sync (
    ShellKeyGrabber *proxy,
    const gchar *arg_accelerator,
    guint arg_flags,
    guint *out_action,
    GCancellable *cancellable,
    GError **error)
{
  GVariant *_ret;
  _ret = g_dbus_proxy_call_sync (G_DBUS_PROXY (proxy),
    "GrabAccelerator",
    g_variant_new ("(su)",
                   arg_accelerator,
                   arg_flags),
    G_DBUS_CALL_FLAGS_NONE,
    -1,
    cancellable,
    error);
  if (_ret == NULL)
    goto _out;
  g_variant_get (_ret,
                 "(u)",
                 out_action);
  g_variant_unref (_ret);
_out:
  return _ret != NULL;
}