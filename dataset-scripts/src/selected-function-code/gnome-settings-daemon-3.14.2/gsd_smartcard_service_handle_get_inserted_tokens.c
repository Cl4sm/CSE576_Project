static gboolean
gsd_smartcard_service_handle_get_inserted_tokens (GsdSmartcardServiceManager *manager,
                                                  GDBusMethodInvocation      *invocation)
{
        GsdSmartcardService *self = GSD_SMARTCARD_SERVICE (manager);
        GsdSmartcardServicePrivate *priv = self->priv;
        GList *inserted_tokens, *node;
        GPtrArray *object_paths;

        inserted_tokens = gsd_smartcard_manager_get_inserted_tokens (priv->smartcard_manager,
                                                                     NULL);

        object_paths = g_ptr_array_new ();
        for (node = inserted_tokens; node != NULL; node = node->next) {
                PK11SlotInfo *card_slot = node->data;
                char *object_path;

                object_path = get_object_path_for_token (self, card_slot);
                g_ptr_array_add (object_paths, object_path);
        }
        g_ptr_array_add (object_paths, NULL);
        g_list_free (inserted_tokens);

        gsd_smartcard_service_manager_complete_get_inserted_tokens (manager,
                                                                    invocation,
                                                                    (const char * const *) object_paths->pdata);

        g_ptr_array_free (object_paths, TRUE);

        return TRUE;
}