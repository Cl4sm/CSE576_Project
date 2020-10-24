void deja_dup_nautilus_extension_register_type (GTypeModule *module)
{
  static const GTypeInfo info = {
    sizeof (DejaDupNautilusExtensionClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) deja_dup_nautilus_extension_class_init,
    NULL,
    NULL,
    sizeof (DejaDupNautilusExtension),
    0,
    (GInstanceInitFunc) deja_dup_nautilus_extension_instance_init,
  };

  deja_dup_nautilus_extension_type = g_type_module_register_type (module,
    G_TYPE_OBJECT,
    "DejaDupNautilusExtension",
    &info, 0);

  static const GInterfaceInfo menu_provider_iface_info =
  {
    (GInterfaceInitFunc)deja_dup_nautilus_extension_menu_provider_iface_init,
     NULL,
     NULL
  };

  g_type_module_add_interface (module, deja_dup_nautilus_extension_type,
    NAUTILUS_TYPE_MENU_PROVIDER, &menu_provider_iface_info);
}