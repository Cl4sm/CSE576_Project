static void
gvc_mixer_card_finalize (GObject *object)
{
        GvcMixerCard *mixer_card;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GVC_IS_MIXER_CARD (object));

        mixer_card = GVC_MIXER_CARD (object);

        g_return_if_fail (mixer_card->priv != NULL);

        g_free (mixer_card->priv->name);
        mixer_card->priv->name = NULL;

        g_free (mixer_card->priv->icon_name);
        mixer_card->priv->icon_name = NULL;

        g_free (mixer_card->priv->target_profile);
        mixer_card->priv->target_profile = NULL;

        g_free (mixer_card->priv->profile);
        mixer_card->priv->profile = NULL;

        g_free (mixer_card->priv->human_profile);
        mixer_card->priv->human_profile = NULL;

        g_list_foreach (mixer_card->priv->profiles, (GFunc) free_profile, NULL);
        g_list_free (mixer_card->priv->profiles);
        mixer_card->priv->profiles = NULL;

        G_OBJECT_CLASS (gvc_mixer_card_parent_class)->finalize (object);
}