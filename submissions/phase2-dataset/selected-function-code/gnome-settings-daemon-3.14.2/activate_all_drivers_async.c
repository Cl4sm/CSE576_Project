static void
activate_all_drivers_async (GsdSmartcardManager *self,
                            GCancellable        *cancellable,
                            GAsyncReadyCallback  callback,
                            gpointer             user_data)
{
        GTask *task;
        SECMODListLock *lock;
        SECMODModuleList *driver_list, *node;
        ActivateAllDriversOperation *operation;

        task = g_task_new (self, cancellable, callback, user_data);
        operation = g_new0 (ActivateAllDriversOperation, 1);
        g_task_set_task_data (task, operation, (GDestroyNotify) g_free);

        lock = SECMOD_GetDefaultModuleListLock ();

        g_assert (lock != NULL);

        SECMOD_GetReadLock (lock);
        driver_list = SECMOD_GetDefaultModuleList ();
        for (node = driver_list; node != NULL; node = node->next) {
                if (!node->module->loaded)
                        continue;

                if (!SECMOD_HasRemovableSlots (node->module))
                        continue;

                if (node->module->dllName == NULL)
                        continue;

                operation->pending_drivers_count++;

                activate_driver (self, node->module,
                                 cancellable,
                                 (GAsyncReadyCallback) on_driver_activated,
                                 task);

        }
        SECMOD_ReleaseReadLock (lock);
}