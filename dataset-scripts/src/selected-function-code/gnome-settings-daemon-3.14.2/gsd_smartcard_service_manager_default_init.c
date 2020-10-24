static void
gsd_smartcard_service_manager_default_init (GsdSmartcardServiceManagerIface *iface)
{
  /* GObject signals for incoming D-Bus method calls: */
  /**
   * GsdSmartcardServiceManager::handle-get-login-token:
   * @object: A #GsdSmartcardServiceManager.
   * @invocation: A #GDBusMethodInvocation.
   *
   * Signal emitted when a remote caller is invoking the <link linkend="gdbus-method-org-gnome-SettingsDaemon-Smartcard-Manager.GetLoginToken">GetLoginToken()</link> D-Bus method.
   *
   * If a signal handler returns %TRUE, it means the signal handler will handle the invocation (e.g. take a reference to @invocation and eventually call gsd_smartcard_service_manager_complete_get_login_token() or e.g. g_dbus_method_invocation_return_error() on it) and no order signal handlers will run. If no signal handler handles the invocation, the %G_DBUS_ERROR_UNKNOWN_METHOD error is returned.
   *
   * Returns: %TRUE if the invocation was handled, %FALSE to let other signal handlers run.
   */
  g_signal_new ("handle-get-login-token",
    G_TYPE_FROM_INTERFACE (iface),
    G_SIGNAL_RUN_LAST,
    G_STRUCT_OFFSET (GsdSmartcardServiceManagerIface, handle_get_login_token),
    g_signal_accumulator_true_handled,
    NULL,
    g_cclosure_marshal_generic,
    G_TYPE_BOOLEAN,
    1,
    G_TYPE_DBUS_METHOD_INVOCATION);

  /**
   * GsdSmartcardServiceManager::handle-get-inserted-tokens:
   * @object: A #GsdSmartcardServiceManager.
   * @invocation: A #GDBusMethodInvocation.
   *
   * Signal emitted when a remote caller is invoking the <link linkend="gdbus-method-org-gnome-SettingsDaemon-Smartcard-Manager.GetInsertedTokens">GetInsertedTokens()</link> D-Bus method.
   *
   * If a signal handler returns %TRUE, it means the signal handler will handle the invocation (e.g. take a reference to @invocation and eventually call gsd_smartcard_service_manager_complete_get_inserted_tokens() or e.g. g_dbus_method_invocation_return_error() on it) and no order signal handlers will run. If no signal handler handles the invocation, the %G_DBUS_ERROR_UNKNOWN_METHOD error is returned.
   *
   * Returns: %TRUE if the invocation was handled, %FALSE to let other signal handlers run.
   */
  g_signal_new ("handle-get-inserted-tokens",
    G_TYPE_FROM_INTERFACE (iface),
    G_SIGNAL_RUN_LAST,
    G_STRUCT_OFFSET (GsdSmartcardServiceManagerIface, handle_get_inserted_tokens),
    g_signal_accumulator_true_handled,
    NULL,
    g_cclosure_marshal_generic,
    G_TYPE_BOOLEAN,
    1,
    G_TYPE_DBUS_METHOD_INVOCATION);

}