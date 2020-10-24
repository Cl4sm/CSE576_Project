static void
gsd_smartcard_service_driver_default_init (GsdSmartcardServiceDriverIface *iface)
{
  /* GObject properties for D-Bus properties: */
  /**
   * GsdSmartcardServiceDriver:library:
   *
   * Represents the D-Bus property <link linkend="gdbus-property-org-gnome-SettingsDaemon-Smartcard-Driver.Library">"Library"</link>.
   *
   * Since the D-Bus property for this #GObject property is readable but not writable, it is meaningful to read from it on both the client- and service-side. It is only meaningful, however, to write to it on the service-side.
   */
  g_object_interface_install_property (iface,
    g_param_spec_string ("library", "Library", "Library", NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  /**
   * GsdSmartcardServiceDriver:description:
   *
   * Represents the D-Bus property <link linkend="gdbus-property-org-gnome-SettingsDaemon-Smartcard-Driver.Description">"Description"</link>.
   *
   * Since the D-Bus property for this #GObject property is readable but not writable, it is meaningful to read from it on both the client- and service-side. It is only meaningful, however, to write to it on the service-side.
   */
  g_object_interface_install_property (iface,
    g_param_spec_string ("description", "Description", "Description", NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}