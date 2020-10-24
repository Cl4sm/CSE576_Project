static gboolean
on_main_thread_to_register_driver (GTask *task)
{
        GsdSmartcardManager *self;
        GsdSmartcardManagerPrivate *priv;
        DriverRegistrationOperation *operation;
        GSource *source;

        self = g_task_get_source_object (task);
        priv = self->priv;
        operation = g_task_get_task_data (task);

        gsd_smartcard_service_register_driver (priv->service,
                                               operation->driver);

        source = g_idle_source_new ();
        g_task_attach_source (task,
                              source,
                              (GSourceFunc) on_task_thread_to_complete_driver_registration);
        g_source_unref (source);

        return G_SOURCE_REMOVE;
}