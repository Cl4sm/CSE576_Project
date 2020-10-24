{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    
    gobject_class->get_property = _properties_get_property;
    gobject_class->set_property = _properties_set_property;
    
    /* object DBusType [readwrite, construct only] */
    obj_properties[PROP_DBUS_TYPE] =
    g_param_spec_string ("DBusType",
                         "dbus_type",
                         "Properties D-Bus connection type",
                         NULL /* default value */,
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_READWRITE);
    
    /* object DBusServiceName [readwrite, construct only] */
    obj_properties[PROP_DBUS_SERVICE_NAME] =
    g_param_spec_string ("DBusServiceName",
                         "dbus_service_name",
                         "Properties D-Bus service name",
                         NULL /* default value */,
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_READWRITE);
    
    /* object DBusObjectPath [readwrite, construct only] */
    obj_properties[PROP_DBUS_OBJECT_PATH] =
    g_param_spec_string ("DBusObjectPath",
                         "dbus_object_path",
                         "Properties D-Bus object path",
                         NULL /* default value */,
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_READWRITE);

    g_object_class_install_properties (gobject_class,
                                       N_PROPERTIES,
                                       obj_properties);
}
