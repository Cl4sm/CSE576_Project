static void
update_card (GvcMixerControl      *control,
             const pa_card_info   *info)
{
        const GList  *card_ports = NULL;
        const GList  *m = NULL;
        GvcMixerCard *card;
        gboolean      is_new = FALSE;
#if 1
        guint i;
        const char *key;
        void *state;

        g_debug ("Udpating card %s (index: %u driver: %s):",
                 info->name, info->index, info->driver);

        for (i = 0; i < info->n_profiles; i++) {
                struct pa_card_profile_info pi = info->profiles[i];
                gboolean is_default;

                is_default = (g_strcmp0 (pi.name, info->active_profile->name) == 0);
                g_debug ("\tProfile '%s': %d sources %d sinks%s",
                         pi.name, pi.n_sources, pi.n_sinks,
                         is_default ? " (Current)" : "");
        }
        state = NULL;
        key = pa_proplist_iterate (info->proplist, &state);
        while (key != NULL) {
                g_debug ("\tProperty: '%s' = '%s'",
                        key, pa_proplist_gets (info->proplist, key));
                key = pa_proplist_iterate (info->proplist, &state);
        }
#endif
        card = g_hash_table_lookup (control->priv->cards,
                                    GUINT_TO_POINTER (info->index));
        if (card == NULL) {
                GList *profile_list = NULL;
                GList *port_list = NULL;

                for (i = 0; i < info->n_profiles; i++) {
                        GvcMixerCardProfile *profile;
                        struct pa_card_profile_info pi = info->profiles[i];

                        profile = g_new0 (GvcMixerCardProfile, 1);
                        profile->profile = g_strdup (pi.name);
                        profile->human_profile = g_strdup (pi.description);
                        profile->status = card_num_streams_to_status (pi.n_sinks, pi.n_sources);
                        profile->n_sinks = pi.n_sinks;
                        profile->n_sources = pi.n_sources;
                        profile->priority = pi.priority;
                        profile_list = g_list_prepend (profile_list, profile);
                }
                card = gvc_mixer_card_new (control->priv->pa_context,
                                           info->index);
                gvc_mixer_card_set_profiles (card, profile_list);

                for (i = 0; i < info->n_ports; i++) {
                        GvcMixerCardPort *port;
                        port = g_new0 (GvcMixerCardPort, 1);
                        port->port = g_strdup (info->ports[i]->name);
                        port->human_port = g_strdup (info->ports[i]->description);
                        port->priority = info->ports[i]->priority;
                        port->available = info->ports[i]->available;
                        port->direction = info->ports[i]->direction;
                        port->profiles = determine_profiles_for_port (info->ports[i], profile_list);
                        port_list = g_list_prepend (port_list, port);
                }
                gvc_mixer_card_set_ports (card, port_list);
                is_new = TRUE;
        }

        gvc_mixer_card_set_name (card, pa_proplist_gets (info->proplist, "device.description"));
        gvc_mixer_card_set_icon_name (card, pa_proplist_gets (info->proplist, "device.icon_name"));
        gvc_mixer_card_set_profile (card, info->active_profile->name);

        if (is_new) {
                g_hash_table_insert (control->priv->cards,
                                     GUINT_TO_POINTER (info->index),
                                     g_object_ref (card));
        }

        card_ports = gvc_mixer_card_get_ports (card);

        if (card_ports == NULL && is_new) {
                g_debug ("Portless card just registered - %s", gvc_mixer_card_get_name (card));
                create_ui_device_from_card (control, card);
        }

        for (m = card_ports; m != NULL; m = m->next) {
                GvcMixerCardPort *card_port;
                card_port = m->data;
                if (is_new)
                        create_ui_device_from_port (control, card_port, card);
                else {
                        for (i = 0; i < info->n_ports; i++) {
                                if (g_strcmp0 (card_port->port, info->ports[i]->name) == 0) {
                                        if (card_port->available != info->ports[i]->available) {
                                                card_port->available = info->ports[i]->available;
                                                g_debug ("sync port availability on card %i, card port name '%s', new available value %i",
                                                          gvc_mixer_card_get_index (card),
                                                          card_port->port,
                                                          card_port->available);
                                                match_card_port_with_existing_device (control,
                                                                                      card_port,
                                                                                      card,
                                                                                      card_port->available != PA_PORT_AVAILABLE_NO);
                                        }
                                }
                        }
                }
        }
        g_signal_emit (G_OBJECT (control),
                       signals[CARD_ADDED],
                       0,
                       info->index);
}