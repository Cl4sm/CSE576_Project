static void
_shell_key_grabber_on_signal_accelerator_activated (
    ShellKeyGrabber *object,
    guint arg_action,
    guint arg_device,
    guint arg_timestamp)
{
  ShellKeyGrabberSkeleton *skeleton = SHELL_KEY_GRABBER_SKELETON (object);

  GList      *connections, *l;
  GVariant   *signal_variant;
  connections = g_dbus_interface_skeleton_get_connections (G_DBUS_INTERFACE_SKELETON (skeleton));

  signal_variant = g_variant_ref_sink (g_variant_new ("(uuu)",
                   arg_action,
                   arg_device,
                   arg_timestamp));
  for (l = connections; l != NULL; l = l->next)
    {
      GDBusConnection *connection = l->data;
      g_dbus_connection_emit_signal (connection,
        NULL, g_dbus_interface_skeleton_get_object_path (G_DBUS_INTERFACE_SKELETON (skeleton)), "org.gnome.Shell", "AcceleratorActivated",
        signal_variant, NULL);
    }
  g_variant_unref (signal_variant);
  g_list_free_full (connections, g_object_unref);
}