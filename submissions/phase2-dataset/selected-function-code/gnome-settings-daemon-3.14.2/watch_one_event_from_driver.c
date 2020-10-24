static gboolean
watch_one_event_from_driver (GsdSmartcardManager       *self,
                             WatchSmartcardsOperation  *operation,
                             GCancellable              *cancellable,
                             GError                   **error)
{
        GsdSmartcardManagerPrivate *priv = self->priv;
        PK11SlotInfo *card, *old_card;
        CK_SLOT_ID slot_id;
        gulong handler_id;
        int old_slot_series = -1, slot_series;

        handler_id = g_cancellable_connect (cancellable,
                                            G_CALLBACK (on_watch_cancelled),
                                            operation,
                                            NULL);

        card = SECMOD_WaitForAnyTokenEvent (operation->driver, 0, PR_SecondsToInterval (1));

        g_cancellable_disconnect (cancellable, handler_id);

        if (g_cancellable_is_cancelled (cancellable)) {
                g_warning ("smartcard event function cancelled");
                return FALSE;
        }

        if (card == NULL) {
                int error_code;

                error_code = PORT_GetError ();

                operation->number_of_consecutive_errors++;
                if (operation->number_of_consecutive_errors > 10) {
                     g_warning ("Got %d consecutive smartcard errors, so giving up.",
                                operation->number_of_consecutive_errors);

                     g_set_error (error,
                                  GSD_SMARTCARD_MANAGER_ERROR,
                                  GSD_SMARTCARD_MANAGER_ERROR_WITH_NSS,
                                  "encountered unexpected error while "
                                  "waiting for smartcard events (error %x)",
                                  error_code);
                     return FALSE;
                }

                g_warning ("Got potentially spurious smartcard event error: %x.", error_code);

                g_usleep (0.5 * G_USEC_PER_SEC);
                return TRUE;
        }
        operation->number_of_consecutive_errors = 0;

        slot_id = PK11_GetSlotID (card);
        slot_series = PK11_GetSlotSeries (card);

        old_card = g_hash_table_lookup (operation->smartcards, GINT_TO_POINTER ((int) slot_id));

        /* If there is a different card in the slot now than
         * there was before, then we need to emit a removed signal
         * for the old card
         */
        if (old_card != NULL) {
                old_slot_series = PK11_GetSlotSeries (old_card);

                if (old_slot_series != slot_series) {
                        /* Card registered with slot previously is
                         * different than this card, so update its
                         * exported state to track the implicit missed
                         * removal
                         */
                        gsd_smartcard_service_sync_token (priv->service, old_card, cancellable);
                }

                g_hash_table_remove (operation->smartcards, GINT_TO_POINTER ((int) slot_id));
        }

        if (PK11_IsPresent (card)) {
                g_debug ("Detected smartcard insertion event in slot %d", (int) slot_id);

                g_hash_table_replace (operation->smartcards,
                                      GINT_TO_POINTER ((int) slot_id),
                                      PK11_ReferenceSlot (card));

                gsd_smartcard_service_sync_token (priv->service, card, cancellable);
        } else if (old_card == NULL) {
                /* If the just removed smartcard is not known to us then
                 * ignore the removal event. NSS sends a synthentic removal
                 * event for slots that are empty at startup
                 */
                g_debug ("Detected slot %d is empty in reader", (int) slot_id);
        } else {
                g_debug ("Detected smartcard removal event in slot %d", (int) slot_id);

                /* If the just removed smartcard is known to us then
                 * we need to update its exported state to reflect the
                 * removal
                 */
                if (old_slot_series == slot_series)
                        gsd_smartcard_service_sync_token (priv->service, card, cancellable);
        }

        PK11_FreeSlot (card);

        return TRUE;
}