static void
mcd_dispatch_operation_get_property (GObject *obj, guint prop_id,
                                     GValue *val, GParamSpec *pspec)
{
    McdDispatchOperationPrivate *priv = MCD_DISPATCH_OPERATION_PRIV (obj);

    switch (prop_id)
    {
    case PROP_CLIENT_REGISTRY:
        g_value_set_object (val, priv->client_registry);
        break;

    case PROP_HANDLER_MAP:
        g_value_set_object (val, priv->handler_map);
        break;

    case PROP_POSSIBLE_HANDLERS:
        g_value_set_boxed (val, priv->possible_handlers);
        break;

    case PROP_NEEDS_APPROVAL:
        g_value_set_boolean (val, priv->needs_approval);
        break;

    case PROP_OBSERVE_ONLY:
        g_value_set_boolean (val, priv->observe_only);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, prop_id, pspec);
        break;
    }
}