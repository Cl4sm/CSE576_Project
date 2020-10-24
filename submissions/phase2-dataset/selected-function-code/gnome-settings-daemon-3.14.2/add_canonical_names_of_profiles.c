static void
add_canonical_names_of_profiles (GvcMixerUIDevice *device,
                                 const GList      *in_profiles,
                                 GHashTable       *added_profiles,
                                 const gchar      *skip_prefix,
                                 gboolean          only_canonical)
{
        const GList *l;

        for (l = in_profiles; l != NULL; l = l->next) {
                gchar *canonical_name;
                GvcMixerCardProfile* p = l->data;

                canonical_name = get_profile_canonical_name (p->profile, skip_prefix);
                g_debug ("The canonical name for '%s' is '%s'", p->profile, canonical_name);

                /* Have we already added the canonical version of this profile? */
                if (g_hash_table_contains (added_profiles, canonical_name)) {
                        g_free (canonical_name);
                        continue;
                }

                if (only_canonical && strcmp (p->profile, canonical_name) != 0) {
                        g_free (canonical_name);
                        continue;
                }

                g_free (canonical_name);

                g_debug ("Adding profile to combobox: '%s' - '%s'", p->profile, p->human_profile);
                g_hash_table_insert (added_profiles, g_strdup (p->profile), p);
                device->priv->profiles = g_list_append (device->priv->profiles, p);
        }
}