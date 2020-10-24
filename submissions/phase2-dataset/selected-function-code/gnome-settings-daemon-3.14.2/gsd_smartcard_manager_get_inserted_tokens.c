GList *
gsd_smartcard_manager_get_inserted_tokens (GsdSmartcardManager *self,
                                           gsize               *num_tokens)
{
        GsdSmartcardManagerPrivate *priv = self->priv;
        GList *inserted_tokens = NULL, *node;

        G_LOCK (gsd_smartcards_watch_tasks);
        for (node = priv->smartcards_watch_tasks; node != NULL; node = node->next) {
                GTask *task = node->data;
                WatchSmartcardsOperation *operation = g_task_get_task_data (task);
                GList *operation_inserted_tokens;

                operation_inserted_tokens = get_inserted_tokens_for_operation (self, operation);

                inserted_tokens = g_list_concat (inserted_tokens, operation_inserted_tokens);
        }
        G_UNLOCK (gsd_smartcards_watch_tasks);

        if (num_tokens != NULL)
                *num_tokens = g_list_length (inserted_tokens);

        return inserted_tokens;
}