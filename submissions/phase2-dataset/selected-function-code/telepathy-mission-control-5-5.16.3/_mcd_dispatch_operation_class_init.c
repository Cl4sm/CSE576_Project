static void
_mcd_dispatch_operation_class_init (McdDispatchOperationClass * klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    g_type_class_add_private (object_class,
                              sizeof (McdDispatchOperationPrivate));

    object_class->constructor = mcd_dispatch_operation_constructor;
    object_class->dispose = mcd_dispatch_operation_dispose;
    object_class->finalize = mcd_dispatch_operation_finalize;
    object_class->set_property = mcd_dispatch_operation_set_property;
    object_class->get_property = mcd_dispatch_operation_get_property;

    g_object_class_install_property (object_class, PROP_CLIENT_REGISTRY,
        g_param_spec_object ("client-registry", "Client registry",
            "Reference to a global registry of Telepathy clients",
            MCD_TYPE_CLIENT_REGISTRY,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
            G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (object_class, PROP_HANDLER_MAP,
        g_param_spec_object ("handler-map", "Handler map",
            "Reference to a global map from handled channels to handlers",
            MCD_TYPE_HANDLER_MAP,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
            G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (object_class, PROP_CHANNEL,
        g_param_spec_object ("channel", "Channel", "the channel to dispatch",
                             MCD_TYPE_CHANNEL,
                             G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY |
                             G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (object_class, PROP_POSSIBLE_HANDLERS,
        g_param_spec_boxed ("possible-handlers", "Possible handlers",
                            "Well-known bus names of possible handlers",
                            G_TYPE_STRV,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
                            G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (object_class, PROP_NEEDS_APPROVAL,
        g_param_spec_boolean ("needs-approval", "Needs approval?",
                              "TRUE if this CDO should run Approvers and "
                              "appear on D-Bus", FALSE,
                              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
                              G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (object_class, PROP_OBSERVE_ONLY,
        g_param_spec_boolean ("observe-only", "Observe only?",
                              "TRUE if this CDO should stop dispatching "
                              "as soon as Observers have been run",
                              FALSE,
                              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
                              G_PARAM_STATIC_STRINGS));
}