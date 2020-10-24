static void
_pa_context_set_card_profile_by_index_cb (pa_context                       *context,
                                          int                               success,
                                          void                             *userdata)
{
        GvcMixerCard *card = GVC_MIXER_CARD (userdata);

        g_assert (card->priv->target_profile);

        if (success > 0) {
                gvc_mixer_card_set_profile (card, card->priv->target_profile);
        } else {
                g_debug ("Failed to switch profile on '%s' from '%s' to '%s'",
                         card->priv->name,
                         card->priv->profile,
                         card->priv->target_profile);
        }
        g_free (card->priv->target_profile);
        card->priv->target_profile = NULL;

        pa_operation_unref (card->priv->profile_op);
        card->priv->profile_op = NULL;
}