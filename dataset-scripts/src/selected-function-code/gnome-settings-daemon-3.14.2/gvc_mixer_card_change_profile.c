gboolean
gvc_mixer_card_change_profile (GvcMixerCard *card,
                               const char *profile)
{
        g_return_val_if_fail (GVC_IS_MIXER_CARD (card), FALSE);
        g_return_val_if_fail (card->priv->profiles != NULL, FALSE);

        /* Same profile, or already requested? */
        if (g_strcmp0 (card->priv->profile, profile) == 0)
                return TRUE;
        if (g_strcmp0 (profile, card->priv->target_profile) == 0)
                return TRUE;
        if (card->priv->profile_op != NULL) {
                pa_operation_cancel (card->priv->profile_op);
                pa_operation_unref (card->priv->profile_op);
                card->priv->profile_op = NULL;
        }

        if (card->priv->profile != NULL) {
                g_free (card->priv->target_profile);
                card->priv->target_profile = g_strdup (profile);

                card->priv->profile_op = pa_context_set_card_profile_by_index (card->priv->pa_context,
                                                                               card->priv->index,
                                                                               card->priv->target_profile,
                                                                               _pa_context_set_card_profile_by_index_cb,
                                                                               card);

                if (card->priv->profile_op == NULL) {
                        g_warning ("pa_context_set_card_profile_by_index() failed");
                        return FALSE;
                }
        } else {
                g_assert (card->priv->human_profile == NULL);
                card->priv->profile = g_strdup (profile);
        }

        return TRUE;
}