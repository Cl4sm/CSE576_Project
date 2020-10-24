static void
synchronize_token_in_main_thread (GsdSmartcardService *self,
                                  PK11SlotInfo        *card_slot,
                                  GCancellable        *cancellable,
                                  GAsyncReadyCallback  callback,
                                  gpointer             user_data)
{
        SynchronizeTokenOperation *operation;
        GTask *task;

        operation = g_new0 (SynchronizeTokenOperation, 1);
        operation->card_slot = PK11_ReferenceSlot (card_slot);

        task = g_task_new (self, cancellable, callback, user_data);

        g_task_set_task_data (task,
                              operation,
                              (GDestroyNotify)
                              destroy_synchronize_token_operation);

        create_main_thread_source ((GSourceFunc)
                                   on_main_thread_to_synchronize_token,
                                   task,
                                   &operation->main_thread_source);
}