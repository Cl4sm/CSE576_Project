{
    Properties *self = PROPERTIES(object);

    switch (property_id)
    {
//    case PROP_EXAMPLE:
//        g_value_set_string(value, self->priv->example);
//        break;

    case PROP_DBUS_TYPE:
        g_value_set_string(value, properties_get_dbus_type(self));
        break;
        
    case PROP_DBUS_SERVICE_NAME:
        g_value_set_string(value, properties_get_dbus_service_name(self));
        break;
        
    case PROP_DBUS_OBJECT_PATH:
        g_value_set_string(value, properties_get_dbus_object_path(self));
        break;

    default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
