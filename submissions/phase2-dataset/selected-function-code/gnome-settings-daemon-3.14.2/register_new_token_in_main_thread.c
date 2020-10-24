static void
register_new_token_in_main_thread (GsdSmartcardService *self,
                                   PK11SlotInfo        *card_slot,
                                   char                *object_path,
                                   GCancellable        *cancellable,
                                   GAsyncReadyCallback  callback,
                                   gpointer             user_data)
{
        RegisterNewTokenOperation *operation;
        GTask *task;

        operation = g_new0 (RegisterNewTokenOperation, 1);
        operation->card_slot = PK11_ReferenceSlot (card_slot);
        operation->object_path = g_strdup (object_path);

        task = g_task_new (self, cancellable, callback, user_data);

        g_task_set_task_data (task,
                              operation,
                              (GDestroyNotify) destroy_register_new_token_operation);

        create_main_thread_source ((GSourceFunc) on_main_thread_to_register_new_token,
                                   task,
                                   &operation->main_thread_source);
}