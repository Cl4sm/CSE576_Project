static void
activate_driver (GsdSmartcardManager *self,
                 SECMODModule        *driver,
                 GCancellable        *cancellable,
                 GAsyncReadyCallback  callback,
                 gpointer             user_data)
{
        GTask *task;

        g_debug ("Activating driver '%s'", driver->commonName);

        task = g_task_new (self, cancellable, callback, user_data);

        register_driver (self,
                         driver,
                         cancellable,
                         (GAsyncReadyCallback) on_driver_registered,
                         task);
        watch_smartcards_from_driver_async (self,
                                            driver,
                                            cancellable,
                                            (GAsyncReadyCallback) on_smartcards_from_driver_watched,
                                            task);
}