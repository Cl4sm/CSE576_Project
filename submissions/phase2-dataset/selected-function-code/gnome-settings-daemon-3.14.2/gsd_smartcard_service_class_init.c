static void
gsd_smartcard_service_class_init (GsdSmartcardServiceClass *service_class)
{
        GObjectClass *object_class = G_OBJECT_CLASS (service_class);
        GParamSpec *param_spec;

        object_class->dispose = gsd_smartcard_service_dispose;
        object_class->set_property = gsd_smartcard_service_set_property;
        object_class->get_property = gsd_smartcard_service_get_property;

        param_spec = g_param_spec_object ("manager",
                                          "Smartcard Manager",
                                          "Smartcard Manager",
                                          GSD_TYPE_SMARTCARD_MANAGER,
                                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
        g_object_class_install_property (object_class, PROP_MANAGER, param_spec);
        param_spec = g_param_spec_object ("bus-connection",
                                          "Bus Connection",
                                          "bus connection",
                                          G_TYPE_DBUS_CONNECTION,
                                          G_PARAM_READABLE);
        g_object_class_install_property (object_class, PROP_BUS_CONNECTION, param_spec);

        g_type_class_add_private (service_class, sizeof (GsdSmartcardServicePrivate));
}