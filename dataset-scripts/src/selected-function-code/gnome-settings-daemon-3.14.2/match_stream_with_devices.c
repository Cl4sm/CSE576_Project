static gboolean
match_stream_with_devices (GvcMixerControl    *control,
                           GvcMixerStreamPort *stream_port,
                           GvcMixerStream     *stream)
{
        GList                   *devices, *d;
        guint                    stream_card_id;
        guint                    stream_id;
        gboolean                 in_possession = FALSE;

        stream_id      =  gvc_mixer_stream_get_id (stream);
        stream_card_id =  gvc_mixer_stream_get_card_index (stream);

        devices  = g_hash_table_get_values (GVC_IS_MIXER_SOURCE (stream) ? control->priv->ui_inputs : control->priv->ui_outputs);

        for (d = devices; d != NULL; d = d->next) {
                GvcMixerUIDevice *device;
                gint              device_stream_id;
                gchar            *device_port_name;
                gchar            *origin;
                gchar            *description;
                GvcMixerCard     *card;
                gint              card_id;

                device = d->data;
                g_object_get (G_OBJECT (device),
                             "stream-id", &device_stream_id,
                             "card", &card,
                             "origin", &origin,
                             "description", &description,
                             "port-name", &device_port_name,
                              NULL);

                card_id = gvc_mixer_card_get_index (card);

                g_debug ("Attempt to match_stream update_with_existing_outputs - Try description : '%s', origin : '%s', device port name : '%s', card : %p, AGAINST stream port: '%s', sink card id %i",
                         description,
                         origin,
                         device_port_name,
                         card,
                         stream_port->port,
                         stream_card_id);

                if (stream_card_id == card_id &&
                    g_strcmp0 (device_port_name, stream_port->port) == 0) {
                        g_debug ("Match device with stream: We have a match with description: '%s', origin: '%s', cached already with device id %u, so set stream id to %i",
                                 description,
                                 origin,
                                 gvc_mixer_ui_device_get_id (device),
                                 stream_id);

                        g_object_set (G_OBJECT (device),
                                      "stream-id", (gint)stream_id,
                                      NULL);
                        in_possession = TRUE;
                }

                g_free (device_port_name);
                g_free (origin);
                g_free (description);

                if (in_possession == TRUE)
                        break;
        }

        g_list_free (devices);
        return in_possession;
}