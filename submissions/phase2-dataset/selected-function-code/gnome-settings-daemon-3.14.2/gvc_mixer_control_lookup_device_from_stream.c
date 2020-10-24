GvcMixerUIDevice *
gvc_mixer_control_lookup_device_from_stream (GvcMixerControl *control,
                                             GvcMixerStream *stream)
{
        GList                   *devices, *d;
        gboolean                 is_network_stream;
        const GList             *ports;
        GvcMixerUIDevice        *ret;

        if (GVC_IS_MIXER_SOURCE (stream))
               devices = g_hash_table_get_values (control->priv->ui_inputs);
        else
               devices = g_hash_table_get_values (control->priv->ui_outputs);

        ret = NULL;
        ports = gvc_mixer_stream_get_ports (stream);
        is_network_stream = (ports == NULL);

        for (d = devices; d != NULL; d = d->next) {
                GvcMixerUIDevice *device = d->data;
                gint stream_id = G_MAXINT;

                g_object_get (G_OBJECT (device),
                             "stream-id", &stream_id,
                              NULL);

                if (is_network_stream &&
                    stream_id == gvc_mixer_stream_get_id (stream)) {
                        g_debug ("lookup device from stream - %s - it is a network_stream ",
                                 gvc_mixer_ui_device_get_description (device));
                        ret = device;
                        break;
                } else if (!is_network_stream) {
                        const GvcMixerStreamPort *port;
                        port = gvc_mixer_stream_get_port (stream);

                        if (stream_id == gvc_mixer_stream_get_id (stream) &&
                            g_strcmp0 (gvc_mixer_ui_device_get_port (device),
                                       port->port) == 0) {
                                g_debug ("lookup-device-from-stream found device: device description '%s', device port = '%s', device stream id %i AND stream port = '%s' stream id '%u' and stream description '%s'",
                                         gvc_mixer_ui_device_get_description (device),
                                         gvc_mixer_ui_device_get_port (device),
                                         stream_id,
                                         port->port,
                                         gvc_mixer_stream_get_id (stream),
                                         gvc_mixer_stream_get_description (stream));
                                ret = device;
                                break;
                        }
                }
        }

        g_debug ("gvc_mixer_control_lookup_device_from_stream - Could not find a device for stream '%s'",gvc_mixer_stream_get_description (stream));

        g_list_free (devices);

        return ret;
}