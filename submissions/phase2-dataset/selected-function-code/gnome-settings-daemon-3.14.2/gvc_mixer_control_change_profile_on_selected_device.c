gboolean
gvc_mixer_control_change_profile_on_selected_device (GvcMixerControl  *control,
                                                     GvcMixerUIDevice *device,
                                                     const gchar      *profile)
{
        const gchar         *best_profile;
        GvcMixerCardProfile *current_profile;
        GvcMixerCard        *card;

        g_object_get (G_OBJECT (device), "card", &card, NULL);
        current_profile = gvc_mixer_card_get_profile (card);

        if (current_profile)
                best_profile = gvc_mixer_ui_device_get_best_profile (device, profile, current_profile->profile);
        else
                best_profile = profile;

        g_assert (best_profile);

        g_debug ("Selected '%s', moving to profile '%s' on card '%s' on stream id %i",
                profile ? profile : "(any)", best_profile,
                gvc_mixer_card_get_name (card),
                gvc_mixer_ui_device_get_stream_id (device));

        g_debug ("default sink name = %s and default sink id %u",
                 control->priv->default_sink_name,
                 control->priv->default_sink_id);

        control->priv->profile_swapping_device_id = gvc_mixer_ui_device_get_id (device);

        if (gvc_mixer_card_change_profile (card, best_profile)) {
                gvc_mixer_ui_device_set_user_preferred_profile (device, best_profile);
                return TRUE;
        }
        return FALSE;
}