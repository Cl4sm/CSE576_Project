static void
synchronize_token_now (GsdSmartcardService *self,
                       PK11SlotInfo        *card_slot)
{
        GsdSmartcardServicePrivate *priv = self->priv;
        GDBusInterfaceSkeleton *interface;
        char *object_path;
        const char *token_name;
        gboolean is_present, is_login_card;

        object_path = get_object_path_for_token (self, card_slot);

        G_LOCK (gsd_smartcard_tokens);
        interface = g_hash_table_lookup (priv->tokens, object_path);
        g_free (object_path);

        if (interface == NULL)
                goto out;

        token_name = PK11_GetTokenName (card_slot);
        is_present = PK11_IsPresent (card_slot);

        if (g_strcmp0 (g_getenv ("PKCS11_LOGIN_TOKEN_NAME"), token_name) == 0)
                is_login_card = TRUE;
        else
                is_login_card = FALSE;

        g_debug ("===============================");
        g_debug (" Token '%s'", token_name);
        g_debug (" Inserted: %s", is_present? "yes" : "no");
        g_debug (" Previously used to login: %s", is_login_card? "yes" : "no");
        g_debug ("===============================\n");

        if (!is_present && is_login_card) {
                gboolean was_present;

                g_object_get (G_OBJECT (interface),
                              "is-inserted", &was_present,
                              NULL);

                if (was_present)
                        gsd_smartcard_manager_do_remove_action (priv->smartcard_manager);
        }

        g_object_set (G_OBJECT (interface),
                      "used-to-login", is_login_card,
                      "is-inserted", is_present,
                      NULL);
        g_object_get (G_OBJECT (interface),
                      "used-to-login", &is_login_card,
                      "is-inserted", &is_present,
                      NULL);

out:
        G_UNLOCK (gsd_smartcard_tokens);
}