const gchar *
gvc_mixer_ui_device_get_best_profile (GvcMixerUIDevice *device,
                                      const gchar      *selected,
                                      const gchar      *current)
{
        GList *candidates, *l;
        const gchar *result;
        gchar *skip_prefix;
        gchar *canonical_name_selected;

        if (device->priv->type == UIDeviceInput)
                skip_prefix = "output:";
        else
                skip_prefix = "input:";

        /* First make a list of profiles acceptable to switch to */
        canonical_name_selected = NULL;
        if (selected)
                canonical_name_selected = get_profile_canonical_name (selected, skip_prefix);

	candidates = NULL;
        for (l = device->priv->supported_profiles; l != NULL; l = l->next) {
                gchar *canonical_name;
                GvcMixerCardProfile* p = l->data;
                canonical_name = get_profile_canonical_name (p->profile, skip_prefix);
                if (!canonical_name_selected || strcmp (canonical_name, canonical_name_selected) == 0) {
                        candidates = g_list_append (candidates, p);
                        g_debug ("Candidate for profile switching: '%s'", p->profile);
                }
        }

        if (!candidates) {
                g_warning ("No suitable profile candidates for '%s'", selected ? selected : "(null)");
                g_free (canonical_name_selected);
                return current;
        }

        /* 1) Maybe we can skip profile switching altogether? */
        result = NULL;
        for (l = candidates; (result == NULL) && (l != NULL); l = l->next) {
                GvcMixerCardProfile* p = l->data;
                if (strcmp (current, p->profile) == 0)
                        result = p->profile;
        }

        /* 2) Try to keep the other side unchanged if possible */
        if (result == NULL) {
                guint prio = 0;
                gchar *skip_prefix_reverse = device->priv->type == UIDeviceInput ? "input:" : "output:";
                gchar *current_reverse = get_profile_canonical_name (current, skip_prefix_reverse);
                for (l = candidates; l != NULL; l = l->next) {
                        gchar *p_reverse;
                        GvcMixerCardProfile* p = l->data;
                        p_reverse = get_profile_canonical_name (p->profile, skip_prefix_reverse);
                        g_debug ("Comparing '%s' (from '%s') with '%s', prio %d", p_reverse, p->profile, current_reverse, p->priority); 
                        if (strcmp (p_reverse, current_reverse) == 0 && (!result || p->priority > prio)) {
                                result = p->profile;
                                prio = p->priority;
                        }
                        g_free (p_reverse);
                }
                g_free (current_reverse);
        }

        /* 3) All right, let's just pick the profile with highest priority.
         * TODO: We could consider asking a GUI question if this stops streams
         * in the other direction */
        if (result == NULL) {
                guint prio = 0;
                for (l = candidates; l != NULL; l = l->next) {
                        GvcMixerCardProfile* p = l->data;
                        if ((p->priority > prio) || !result) {
                                result = p->profile;
                                prio = p->priority;
                        }
                }
        }

        g_list_free (candidates);
        g_free (canonical_name_selected);
        return result;
}