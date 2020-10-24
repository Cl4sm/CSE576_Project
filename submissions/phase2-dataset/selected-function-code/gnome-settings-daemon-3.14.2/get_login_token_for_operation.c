static PK11SlotInfo *
get_login_token_for_operation (GsdSmartcardManager      *self,
                               WatchSmartcardsOperation *operation)
{
        GHashTableIter iter;
        gpointer key, value;

        g_hash_table_iter_init (&iter, operation->smartcards);
        while (g_hash_table_iter_next (&iter, &key, &value)) {
                PK11SlotInfo *card_slot;
                const char *token_name;

                card_slot = (PK11SlotInfo *) value;
                token_name = PK11_GetTokenName (card_slot);

                if (g_strcmp0 (g_getenv ("PKCS11_LOGIN_TOKEN_NAME"), token_name) == 0)
                        return card_slot;
        }

        return NULL;
}