static void
gsd_smartcard_service_get_property (GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *param_spec)
{
        GsdSmartcardService *self = GSD_SMARTCARD_SERVICE (object);
        GsdSmartcardServicePrivate *priv = self->priv;

        switch (property_id) {
                case PROP_MANAGER:
                        g_value_set_object (value, priv->smartcard_manager);
                        break;
                case PROP_BUS_CONNECTION:
                        g_value_set_object (value, priv->bus_connection);
                        break;
                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, param_spec);
                        break;
        }
}