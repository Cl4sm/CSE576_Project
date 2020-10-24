static GObject *
mcd_dispatch_operation_constructor (GType type, guint n_params,
                                    GObjectConstructParam *params)
{
    GObjectClass *object_class =
        (GObjectClass *)_mcd_dispatch_operation_parent_class;
    GObject *object;
    McdDispatchOperation *operation;
    McdDispatchOperationPrivate *priv;

    object = object_class->constructor (type, n_params, params);
    operation = MCD_DISPATCH_OPERATION (object);

    g_return_val_if_fail (operation != NULL, NULL);
    priv = operation->priv;

    if (!priv->client_registry || !priv->handler_map || !priv->channel)
        goto error;

    if (priv->needs_approval && priv->observe_only)
    {
        g_critical ("observe_only => needs_approval must not be TRUE");
        goto error;
    }

    create_object_path (priv);

    DEBUG ("%s/%p: needs_approval=%c", priv->unique_name, object,
           priv->needs_approval ? 'T' : 'F');

    if (priv->channel != NULL)
    {
        DEBUG ("Channel: %s",
               mcd_channel_get_object_path (priv->channel));
        g_assert (mcd_channel_get_account (priv->channel) ==
                  priv->account);
    }

    /* If approval is not needed, we don't appear on D-Bus (and approvers
     * don't run) */
    if (priv->needs_approval)
    {
        TpDBusDaemon *dbus_daemon;
        DBusGConnection *dbus_connection;

        g_object_get (priv->client_registry,
                      "dbus-daemon", &dbus_daemon,
                      NULL);

        /* can be NULL if we have fallen off the bus (in the real MC libdbus
         * would exit in this situation, but in the debug build, we stay
         * active briefly) */
        dbus_connection = tp_proxy_get_dbus_connection (dbus_daemon);

        if (G_LIKELY (dbus_connection != NULL))
            dbus_g_connection_register_g_object (dbus_connection,
                                                 priv->object_path, object);

        g_object_unref (dbus_daemon);
    }

    priv->plugin_api = _mcd_plugin_dispatch_operation_new (operation);

    return object;
error:
    g_object_unref (object);
    g_return_val_if_reached (NULL);
}