static void
watch_smartcards_from_driver_async (GsdSmartcardManager *self,
                                    SECMODModule        *driver,
                                    GCancellable        *cancellable,
                                    GAsyncReadyCallback  callback,
                                    gpointer             user_data)
{
        GsdSmartcardManagerPrivate *priv = self->priv;
        GTask *task;
        WatchSmartcardsOperation *operation;

        operation = g_new0 (WatchSmartcardsOperation, 1);
        operation->driver = SECMOD_ReferenceModule (driver);
        operation->smartcards = g_hash_table_new_full (g_direct_hash,
                                                       g_direct_equal,
                                                       NULL,
                                                       (GDestroyNotify) PK11_FreeSlot);

        task = g_task_new (self, cancellable, callback, user_data);

        g_task_set_task_data (task,
                              operation,
                              (GDestroyNotify) destroy_watch_smartcards_operation);

        G_LOCK (gsd_smartcards_watch_tasks);
        priv->smartcards_watch_tasks = g_list_prepend (priv->smartcards_watch_tasks,
                                                       task);
        g_object_weak_ref (G_OBJECT (task),
                           (GWeakNotify) on_smartcards_watch_task_destroyed,
                           self);
        G_UNLOCK (gsd_smartcards_watch_tasks);

        g_task_run_in_thread (task, (GTaskThreadFunc) watch_smartcards_from_driver);

        g_object_unref (task);
}