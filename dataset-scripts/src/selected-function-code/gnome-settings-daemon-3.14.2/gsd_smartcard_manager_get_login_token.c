PK11SlotInfo *
gsd_smartcard_manager_get_login_token (GsdSmartcardManager *self)
{
        GsdSmartcardManagerPrivate *priv = self->priv;
        PK11SlotInfo *card_slot = NULL;
        GList *node;

        G_LOCK (gsd_smartcards_watch_tasks);
        node = priv->smartcards_watch_tasks;
        while (node != NULL) {
                GTask *task = node->data;
                WatchSmartcardsOperation *operation = g_task_get_task_data (task);

                card_slot = get_login_token_for_operation (self, operation);

                if (card_slot != NULL)
                        break;

                node = node->next;
        }
        G_UNLOCK (gsd_smartcards_watch_tasks);

        return card_slot;
}