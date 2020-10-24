static void
sync_devices (GvcMixerControl *control,
              GvcMixerStream* stream)
{
        /* Go through ports to see what outputs can be created. */
        const GList *stream_ports;
        const GList *n = NULL;
        gboolean     is_output = !GVC_IS_MIXER_SOURCE (stream);
        gint         stream_port_count = 0;

        stream_ports = gvc_mixer_stream_get_ports (stream);

        if (stream_ports == NULL) {
                GvcMixerUIDevice *device;
                /* Bluetooth, no ports but a valid card */
                if (gvc_mixer_stream_get_card_index (stream) != PA_INVALID_INDEX) {
                        GList *devices, *d;
                        gboolean in_possession = FALSE;

                        devices = g_hash_table_get_values (is_output ? control->priv->ui_outputs : control->priv->ui_inputs);

                        for (d = devices; d != NULL; d = d->next) {
                                GvcMixerCard *card;
                                gint card_id;

                                device = d->data;

                                g_object_get (G_OBJECT (device),
                                             "card", &card,
                                              NULL);
                                card_id = gvc_mixer_card_get_index (card);
                                g_debug ("sync devices, device description - '%s', device card id - %i, stream description - %s, stream card id - %i",
                                         gvc_mixer_ui_device_get_description (device),
                                         card_id,
                                         gvc_mixer_stream_get_description (stream),
                                         gvc_mixer_stream_get_card_index (stream));
                                if (card_id == gvc_mixer_stream_get_card_index (stream)) {
                                        in_possession = TRUE;
                                        break;
                                }
                        }
                        g_list_free (devices);

                        if (!in_possession) {
                                g_warning ("Couldn't match the portless stream (with card) - '%s' is it an input ? -> %i, streams card id -> %i",
                                           gvc_mixer_stream_get_description (stream),
                                           GVC_IS_MIXER_SOURCE (stream),
                                           gvc_mixer_stream_get_card_index (stream));
                                return;
                        }

                        g_object_set (G_OBJECT (device),
                                      "stream-id", (gint)gvc_mixer_stream_get_id (stream),
                                      "description", gvc_mixer_stream_get_description (stream),
                                      "origin", "", /*Leave it empty for these special cases*/
                                      "port-name", NULL,
                                      "port-available", TRUE,
                                      NULL);
                } else { /* Network sink/source has no ports and no card. */
                        GObject *object;

                        object = g_object_new (GVC_TYPE_MIXER_UI_DEVICE,
                                               "stream-id", (gint)gvc_mixer_stream_get_id (stream),
                                               "description", gvc_mixer_stream_get_description (stream),
                                               "origin", "", /* Leave it empty for these special cases */
                                               "port-name", NULL,
                                               "port-available", TRUE,
                                                NULL);
                        device = GVC_MIXER_UI_DEVICE (object);

                        g_hash_table_insert (is_output ? control->priv->ui_outputs : control->priv->ui_inputs,
                                             GUINT_TO_POINTER (gvc_mixer_ui_device_get_id (device)),
                                             g_object_ref (device));

                }
                g_signal_emit (G_OBJECT (control),
                               signals[is_output ? OUTPUT_ADDED : INPUT_ADDED],
                               0,
                               gvc_mixer_ui_device_get_id (device));

                return;
        }

        /* Go ahead and make sure to match each port against a previously created device */
        for (n = stream_ports; n != NULL; n = n->next) {

                GvcMixerStreamPort *stream_port;
                stream_port = n->data;
                stream_port_count ++;

                if (match_stream_with_devices (control, stream_port, stream))
                        continue;

                g_warning ("Sync_devices: Failed to match stream id: %u, description: '%s', origin: '%s'",
                           gvc_mixer_stream_get_id (stream),
                           stream_port->human_port,
                           gvc_mixer_stream_get_description (stream));
        }
}