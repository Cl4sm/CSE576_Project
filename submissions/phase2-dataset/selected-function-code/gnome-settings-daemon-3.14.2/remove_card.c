static void
remove_card (GvcMixerControl *control,
             guint            index)
{

        GList *devices, *d;

        devices = g_list_concat (g_hash_table_get_values (control->priv->ui_inputs),
                                 g_hash_table_get_values (control->priv->ui_outputs));

        for (d = devices; d != NULL; d = d->next) {
                GvcMixerCard *card;
                GvcMixerUIDevice *device = d->data;

                g_object_get (G_OBJECT (device), "card", &card, NULL);

                if (gvc_mixer_card_get_index (card) == index) {
                        g_signal_emit (G_OBJECT (control),
                                       signals[gvc_mixer_ui_device_is_output (device) ? OUTPUT_REMOVED : INPUT_REMOVED],
                                       0,
                                       gvc_mixer_ui_device_get_id (device));
                        g_debug ("Card removal remove device %s",
                                 gvc_mixer_ui_device_get_description (device));
                        g_hash_table_remove (gvc_mixer_ui_device_is_output (device) ? control->priv->ui_outputs : control->priv->ui_inputs,
                                             GUINT_TO_POINTER (gvc_mixer_ui_device_get_id (device)));
                }
        }

        g_list_free (devices);

        g_hash_table_remove (control->priv->cards,
                             GUINT_TO_POINTER (index));

        g_signal_emit (G_OBJECT (control),
                       signals[CARD_REMOVED],
                       0,
                       index);
}