{
    Properties *self = PROPERTIES(object);
    GError *error = NULL;

    switch (property_id)
    {
//    case PROP_EXAMPLE:
//        g_free(self->priv->example);
//        self->priv->example = g_value_dup_string(value);
//        g_print("example: %s\n", self->priv->example);
//        break;

    case PROP_DBUS_TYPE:
        if(self->priv->dbus_type)
            g_free(self->priv->dbus_type);
        self->priv->dbus_type = g_value_dup_string(value);
        // _properties_set_dbus_object_path(self, g_value_dup_string(value), &error);
        _properties_create_gdbus_proxy(self, &error);
        break;
        
    case PROP_DBUS_SERVICE_NAME:
        if(self->priv->dbus_service_name)
            g_free(self->priv->dbus_service_name);
        self->priv->dbus_service_name = g_value_dup_string(value);
        // _properties_set_dbus_object_path(self, g_value_dup_string(value), &error);
        _properties_create_gdbus_proxy(self, &error);
        break;
        
    case PROP_DBUS_OBJECT_PATH:
        if(self->priv->dbus_object_path)
            g_free(self->priv->dbus_object_path);
        self->priv->dbus_object_path = g_value_dup_string(value);
        // _properties_set_dbus_object_path(self, g_value_dup_string(value), &error);
        _properties_create_gdbus_proxy(self, &error);
        break;

    default:
        /* We don't have any other property... */
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }

    g_assert(error == NULL);
}
