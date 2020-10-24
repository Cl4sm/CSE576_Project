gboolean
gvc_mixer_card_set_profile (GvcMixerCard *card,
                            const char     *profile)
{
        GList *l;

        g_return_val_if_fail (GVC_IS_MIXER_CARD (card), FALSE);
        g_return_val_if_fail (card->priv->profiles != NULL, FALSE);

        g_free (card->priv->profile);
        card->priv->profile = g_strdup (profile);

        g_free (card->priv->human_profile);
        card->priv->human_profile = NULL;

        for (l = card->priv->profiles; l != NULL; l = l->next) {
                GvcMixerCardProfile *p = l->data;
                if (g_str_equal (card->priv->profile, p->profile)) {
                        card->priv->human_profile = g_strdup (p->human_profile);
                        break;
                }
        }

        g_object_notify (G_OBJECT (card), "profile");

        return TRUE;
}