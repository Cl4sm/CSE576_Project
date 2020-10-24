static void
gsd_smartcard_service_object_default_init (GsdSmartcardServiceObjectIface *iface)
{
  /**
   * GsdSmartcardServiceObject:manager:
   *
   * The #GsdSmartcardServiceManager instance corresponding to the D-Bus interface <link linkend="gdbus-interface-org-gnome-SettingsDaemon-Smartcard-Manager.top_of_page">org.gnome.SettingsDaemon.Smartcard.Manager</link>, if any.
   *
   * Connect to the #GObject::notify signal to get informed of property changes.
   */
  g_object_interface_install_property (iface, g_param_spec_object ("manager", "manager", "manager", GSD_SMARTCARD_SERVICE_TYPE_MANAGER, G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS));

  /**
   * GsdSmartcardServiceObject:driver:
   *
   * The #GsdSmartcardServiceDriver instance corresponding to the D-Bus interface <link linkend="gdbus-interface-org-gnome-SettingsDaemon-Smartcard-Driver.top_of_page">org.gnome.SettingsDaemon.Smartcard.Driver</link>, if any.
   *
   * Connect to the #GObject::notify signal to get informed of property changes.
   */
  g_object_interface_install_property (iface, g_param_spec_object ("driver", "driver", "driver", GSD_SMARTCARD_SERVICE_TYPE_DRIVER, G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS));

  /**
   * GsdSmartcardServiceObject:token:
   *
   * The #GsdSmartcardServiceToken instance corresponding to the D-Bus interface <link linkend="gdbus-interface-org-gnome-SettingsDaemon-Smartcard-Token.top_of_page">org.gnome.SettingsDaemon.Smartcard.Token</link>, if any.
   *
   * Connect to the #GObject::notify signal to get informed of property changes.
   */
  g_object_interface_install_property (iface, g_param_spec_object ("token", "token", "token", GSD_SMARTCARD_SERVICE_TYPE_TOKEN, G_PARAM_READWRITE|G_PARAM_STATIC_STRINGS));

}