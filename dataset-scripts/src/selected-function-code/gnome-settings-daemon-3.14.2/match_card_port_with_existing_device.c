static void
match_card_port_with_existing_device (GvcMixerControl   *control,
                                      GvcMixerCardPort  *card_port,
                                      GvcMixerCard      *card,
                                      gboolean           available)
{
        GList                   *d;
        GList                   *devices;
        GvcMixerUIDevice        *device;
        gboolean                 is_output = is_card_port_an_output (card_port);

        devices  = g_hash_table_get_values (is_output ? control->priv->ui_outputs : control->priv->ui_inputs);

        for (d = devices; d != NULL; d = d->next) {
                GvcMixerCard *device_card;
                gchar        *device_port_name;

                device = d->data;
                g_object_get (G_OBJECT (device),
                             "card", &device_card,
                             "port-name", &device_port_name,
                              NULL);

                if (g_strcmp0 (card_port->port, device_port_name) == 0 &&
                        device_card == card) {
                        g_debug ("Found the relevant device %s, update its port availability flag to %i, is_output %i",
                                 device_port_name,
                                 available,
                                 is_output);
                        g_object_set (G_OBJECT (device),
                                      "port-available", available, NULL);
                        g_signal_emit (G_OBJECT (control),
                                       is_output ? signals[available ? OUTPUT_ADDED : OUTPUT_REMOVED] : signals[available ? INPUT_ADDED : INPUT_REMOVED],
                                       0,
                                       gvc_mixer_ui_device_get_id (device));
               }
               g_free (device_port_name);
        }

        g_list_free (devices);
}