static void
create_ui_device_from_card (GvcMixerControl *control,
                            GvcMixerCard    *card)
{
        GObject          *object;
        GvcMixerUIDevice *in;
        GvcMixerUIDevice *out;
        const GList      *profiles;

        /* For now just create two devices and presume this device is multi directional
         * Ensure to remove both on card removal (available to false by default) */
        profiles = gvc_mixer_card_get_profiles (card);

        g_debug ("Portless card just registered - %i", gvc_mixer_card_get_index (card));

        object = g_object_new (GVC_TYPE_MIXER_UI_DEVICE,
                               "type", UIDeviceInput,
                               "description", gvc_mixer_card_get_name (card),
                               "origin", "", /* Leave it empty for these special cases */
                               "port-name", NULL,
                               "port-available", FALSE,
                               "card", card,
                               NULL);
        in = GVC_MIXER_UI_DEVICE (object);
        gvc_mixer_ui_device_set_profiles (in, profiles);

        g_hash_table_insert (control->priv->ui_inputs,
                             GUINT_TO_POINTER (gvc_mixer_ui_device_get_id (in)),
                             g_object_ref (in));
        object = g_object_new (GVC_TYPE_MIXER_UI_DEVICE,
                               "type", UIDeviceOutput,
                               "description", gvc_mixer_card_get_name (card),
                               "origin", "", /* Leave it empty for these special cases */
                               "port-name", NULL,
                               "port-available", FALSE,
                               "card", card,
                               NULL);
        out = GVC_MIXER_UI_DEVICE (object);
        gvc_mixer_ui_device_set_profiles (out, profiles);

        g_hash_table_insert (control->priv->ui_outputs,
                             GUINT_TO_POINTER (gvc_mixer_ui_device_get_id (out)),
                             g_object_ref (out));
}