void
gvc_mixer_ui_device_set_profiles (GvcMixerUIDevice *device,
                                  const GList      *in_profiles)
{
        GHashTable *added_profiles;
        gchar *skip_prefix = device->priv->type == UIDeviceInput ? "output:" : "input:";

        g_debug ("Set profiles for '%s'", gvc_mixer_ui_device_get_description(device));

        if (in_profiles == NULL)
                return;

        device->priv->supported_profiles = g_list_copy ((GList*) in_profiles);

        added_profiles = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

        /* Run two iterations: First, add profiles which are canonical themselves,
         * Second, add profiles for which the canonical name is not added already. */

        add_canonical_names_of_profiles(device, in_profiles, added_profiles, skip_prefix, TRUE);
        add_canonical_names_of_profiles(device, in_profiles, added_profiles, skip_prefix, FALSE);

        /* TODO: Consider adding the "Off" profile here */

        device->priv->disable_profile_swapping = g_hash_table_size (added_profiles) <= 1;
        g_hash_table_destroy (added_profiles);
}