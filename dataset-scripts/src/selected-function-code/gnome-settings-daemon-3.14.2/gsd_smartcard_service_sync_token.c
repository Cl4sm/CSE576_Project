void
gsd_smartcard_service_sync_token (GsdSmartcardService *self,
                                  PK11SlotInfo        *card_slot,
                                  GCancellable        *cancellable)
{
        GsdSmartcardServicePrivate *priv = self->priv;
        char *object_path;
        GDBusInterfaceSkeleton *interface;

        object_path = get_object_path_for_token (self, card_slot);

        G_LOCK (gsd_smartcard_tokens);
        interface = g_hash_table_lookup (priv->tokens, object_path);
        G_UNLOCK (gsd_smartcard_tokens);

        if (interface == NULL)
                register_new_token_in_main_thread (self,
                                                   card_slot,
                                                   object_path,
                                                   cancellable,
                                                   (GAsyncReadyCallback)
                                                   on_token_registered,
                                                   PK11_ReferenceSlot (card_slot));

        else
                synchronize_token_in_main_thread (self,
                                                  card_slot,
                                                  cancellable,
                                                  (GAsyncReadyCallback)
                                                  on_token_synchronized,
                                                  PK11_ReferenceSlot (card_slot));

        g_free (object_path);
}