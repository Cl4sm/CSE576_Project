static gboolean
on_main_thread_to_register_new_token (GTask *task)
{
        GsdSmartcardService *self;
        GsdSmartcardServicePrivate *priv;
        GDBusObjectSkeleton *object;
        GDBusInterfaceSkeleton *interface;
        RegisterNewTokenOperation *operation;
        SECMODModule *driver;
        char *driver_object_path;
        const char *token_name;

        self = g_task_get_source_object (task);
        priv = self->priv;

        operation = g_task_get_task_data (task);
        operation->main_thread_source = NULL;

        object = G_DBUS_OBJECT_SKELETON (gsd_smartcard_service_object_skeleton_new (operation->object_path));
        interface = G_DBUS_INTERFACE_SKELETON (gsd_smartcard_service_token_skeleton_new ());

        g_dbus_object_skeleton_add_interface (object, interface);
        g_object_unref (interface);

        driver = PK11_GetModule (operation->card_slot);
        driver_object_path = get_object_path_for_driver (self, driver);

        token_name = PK11_GetTokenName (operation->card_slot);

        g_object_set (G_OBJECT (interface),
                      "driver", driver_object_path,
                      "name", token_name,
                      NULL);
        g_free (driver_object_path);

        g_dbus_object_manager_server_export (self->priv->object_manager_server,
                                             object);

        G_LOCK (gsd_smartcard_tokens);
        g_hash_table_insert (priv->tokens, g_strdup (operation->object_path), interface);
        G_UNLOCK (gsd_smartcard_tokens);

        g_task_return_boolean (task, TRUE);

        return G_SOURCE_REMOVE;
}