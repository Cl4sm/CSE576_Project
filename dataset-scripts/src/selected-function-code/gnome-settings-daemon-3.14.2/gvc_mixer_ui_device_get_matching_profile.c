const gchar *
gvc_mixer_ui_device_get_matching_profile (GvcMixerUIDevice *device, const gchar *profile)
{
        gchar *skip_prefix = device->priv->type == UIDeviceInput ? "output:" : "input:";
        gchar *target_cname = get_profile_canonical_name (profile, skip_prefix);
        GList *l;
        gchar *result = NULL;

        for (l = device->priv->profiles; l != NULL; l = l->next) {
                gchar *canonical_name;
                GvcMixerCardProfile* p = l->data;
                canonical_name = get_profile_canonical_name (p->profile, skip_prefix);
                if (strcmp (canonical_name, target_cname) == 0)
                        result = p->profile;
                g_free (canonical_name);
        }

        g_free (target_cname);
        g_debug ("Matching profile for '%s' is '%s'", profile, result ? result : "(null)");
        return result;
}