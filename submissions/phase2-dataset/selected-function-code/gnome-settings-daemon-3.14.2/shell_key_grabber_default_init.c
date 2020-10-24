static void
shell_key_grabber_default_init (ShellKeyGrabberIface *iface)
{
  /* GObject signals for incoming D-Bus method calls: */
  /**
   * ShellKeyGrabber::handle-grab-accelerator:
   * @object: A #ShellKeyGrabber.
   * @invocation: A #GDBusMethodInvocation.
   * @arg_accelerator: Argument passed by remote caller.
   * @arg_flags: Argument passed by remote caller.
   *
   * Signal emitted when a remote caller is invoking the <link linkend="gdbus-method-org-gnome-Shell.GrabAccelerator">GrabAccelerator()</link> D-Bus method.
   *
   * If a signal handler returns %TRUE, it means the signal handler will handle the invocation (e.g. take a reference to @invocation and eventually call shell_key_grabber_complete_grab_accelerator() or e.g. g_dbus_method_invocation_return_error() on it) and no order signal handlers will run. If no signal handler handles the invocation, the %G_DBUS_ERROR_UNKNOWN_METHOD error is returned.
   *
   * Returns: %TRUE if the invocation was handled, %FALSE to let other signal handlers run.
   */
  g_signal_new ("handle-grab-accelerator",
    G_TYPE_FROM_INTERFACE (iface),
    G_SIGNAL_RUN_LAST,
    G_STRUCT_OFFSET (ShellKeyGrabberIface, handle_grab_accelerator),
    g_signal_accumulator_true_handled,
    NULL,
    g_cclosure_marshal_generic,
    G_TYPE_BOOLEAN,
    3,
    G_TYPE_DBUS_METHOD_INVOCATION, G_TYPE_STRING, G_TYPE_UINT);

  /**
   * ShellKeyGrabber::handle-grab-accelerators:
   * @object: A #ShellKeyGrabber.
   * @invocation: A #GDBusMethodInvocation.
   * @arg_accelerators: Argument passed by remote caller.
   *
   * Signal emitted when a remote caller is invoking the <link linkend="gdbus-method-org-gnome-Shell.GrabAccelerators">GrabAccelerators()</link> D-Bus method.
   *
   * If a signal handler returns %TRUE, it means the signal handler will handle the invocation (e.g. take a reference to @invocation and eventually call shell_key_grabber_complete_grab_accelerators() or e.g. g_dbus_method_invocation_return_error() on it) and no order signal handlers will run. If no signal handler handles the invocation, the %G_DBUS_ERROR_UNKNOWN_METHOD error is returned.
   *
   * Returns: %TRUE if the invocation was handled, %FALSE to let other signal handlers run.
   */
  g_signal_new ("handle-grab-accelerators",
    G_TYPE_FROM_INTERFACE (iface),
    G_SIGNAL_RUN_LAST,
    G_STRUCT_OFFSET (ShellKeyGrabberIface, handle_grab_accelerators),
    g_signal_accumulator_true_handled,
    NULL,
    g_cclosure_marshal_generic,
    G_TYPE_BOOLEAN,
    2,
    G_TYPE_DBUS_METHOD_INVOCATION, G_TYPE_VARIANT);

  /**
   * ShellKeyGrabber::handle-ungrab-accelerator:
   * @object: A #ShellKeyGrabber.
   * @invocation: A #GDBusMethodInvocation.
   * @arg_action: Argument passed by remote caller.
   *
   * Signal emitted when a remote caller is invoking the <link linkend="gdbus-method-org-gnome-Shell.UngrabAccelerator">UngrabAccelerator()</link> D-Bus method.
   *
   * If a signal handler returns %TRUE, it means the signal handler will handle the invocation (e.g. take a reference to @invocation and eventually call shell_key_grabber_complete_ungrab_accelerator() or e.g. g_dbus_method_invocation_return_error() on it) and no order signal handlers will run. If no signal handler handles the invocation, the %G_DBUS_ERROR_UNKNOWN_METHOD error is returned.
   *
   * Returns: %TRUE if the invocation was handled, %FALSE to let other signal handlers run.
   */
  g_signal_new ("handle-ungrab-accelerator",
    G_TYPE_FROM_INTERFACE (iface),
    G_SIGNAL_RUN_LAST,
    G_STRUCT_OFFSET (ShellKeyGrabberIface, handle_ungrab_accelerator),
    g_signal_accumulator_true_handled,
    NULL,
    g_cclosure_marshal_generic,
    G_TYPE_BOOLEAN,
    2,
    G_TYPE_DBUS_METHOD_INVOCATION, G_TYPE_UINT);

  /* GObject signals for received D-Bus signals: */
  /**
   * ShellKeyGrabber::accelerator-activated:
   * @object: A #ShellKeyGrabber.
   * @arg_action: Argument.
   * @arg_device: Argument.
   * @arg_timestamp: Argument.
   *
   * On the client-side, this signal is emitted whenever the D-Bus signal <link linkend="gdbus-signal-org-gnome-Shell.AcceleratorActivated">"AcceleratorActivated"</link> is received.
   *
   * On the service-side, this signal can be used with e.g. g_signal_emit_by_name() to make the object emit the D-Bus signal.
   */
  g_signal_new ("accelerator-activated",
    G_TYPE_FROM_INTERFACE (iface),
    G_SIGNAL_RUN_LAST,
    G_STRUCT_OFFSET (ShellKeyGrabberIface, accelerator_activated),
    NULL,
    NULL,
    g_cclosure_marshal_generic,
    G_TYPE_NONE,
    3, G_TYPE_UINT, G_TYPE_UINT, G_TYPE_UINT);

}