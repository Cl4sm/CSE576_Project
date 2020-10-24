static void
gsd_smartcard_service_token_default_init (GsdSmartcardServiceTokenIface *iface)
{
  /* GObject properties for D-Bus properties: */
  /**
   * GsdSmartcardServiceToken:name:
   *
   * Represents the D-Bus property <link linkend="gdbus-property-org-gnome-SettingsDaemon-Smartcard-Token.Name">"Name"</link>.
   *
   * Since the D-Bus property for this #GObject property is readable but not writable, it is meaningful to read from it on both the client- and service-side. It is only meaningful, however, to write to it on the service-side.
   */
  g_object_interface_install_property (iface,
    g_param_spec_string ("name", "Name", "Name", NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  /**
   * GsdSmartcardServiceToken:driver:
   *
   * Represents the D-Bus property <link linkend="gdbus-property-org-gnome-SettingsDaemon-Smartcard-Token.Driver">"Driver"</link>.
   *
   * Since the D-Bus property for this #GObject property is readable but not writable, it is meaningful to read from it on both the client- and service-side. It is only meaningful, however, to write to it on the service-side.
   */
  g_object_interface_install_property (iface,
    g_param_spec_string ("driver", "Driver", "Driver", NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  /**
   * GsdSmartcardServiceToken:is-inserted:
   *
   * Represents the D-Bus property <link linkend="gdbus-property-org-gnome-SettingsDaemon-Smartcard-Token.IsInserted">"IsInserted"</link>.
   *
   * Since the D-Bus property for this #GObject property is readable but not writable, it is meaningful to read from it on both the client- and service-side. It is only meaningful, however, to write to it on the service-side.
   */
  g_object_interface_install_property (iface,
    g_param_spec_boolean ("is-inserted", "IsInserted", "IsInserted", FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  /**
   * GsdSmartcardServiceToken:used-to-login:
   *
   * Represents the D-Bus property <link linkend="gdbus-property-org-gnome-SettingsDaemon-Smartcard-Token.UsedToLogin">"UsedToLogin"</link>.
   *
   * Since the D-Bus property for this #GObject property is readable but not writable, it is meaningful to read from it on both the client- and service-side. It is only meaningful, however, to write to it on the service-side.
   */
  g_object_interface_install_property (iface,
    g_param_spec_boolean ("used-to-login", "UsedToLogin", "UsedToLogin", FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}