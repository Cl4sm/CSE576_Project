void
gsd_smartcard_service_register_driver (GsdSmartcardService  *self,
                                       SECMODModule         *driver)
{
        char *object_path;
        GDBusObjectSkeleton *object;
        GDBusInterfaceSkeleton *interface;

        object_path = get_object_path_for_driver (self, driver);
        object = G_DBUS_OBJECT_SKELETON (gsd_smartcard_service_object_skeleton_new (object_path));
        g_free (object_path);

        interface = G_DBUS_INTERFACE_SKELETON (gsd_smartcard_service_driver_skeleton_new ());
        g_dbus_object_skeleton_add_interface (object, interface);
        g_object_unref (interface);

        g_object_set (G_OBJECT (interface),
                      "library", driver->dllName,
                      "description", driver->commonName,
                      NULL);
        g_dbus_object_manager_server_export (self->priv->object_manager_server,
                                             object);
        g_object_unref (object);
}