static void
on_token_registered (GsdSmartcardService *self,
                     GAsyncResult        *result,
                     PK11SlotInfo        *card_slot)
{
        gboolean registered;
        GError *error = NULL;

        registered = register_new_token_in_main_thread_finish (self, result, &error);

        if (!registered) {
                g_debug ("Couldn't register token: %s",
                         error->message);
                goto out;
        }

        synchronize_token_now (self, card_slot);

out:
        PK11_FreeSlot (card_slot);
}