static void
remove_sink (GvcMixerControl *control,
             guint            index)
{
        GvcMixerStream   *stream;
        GvcMixerUIDevice *device;

        g_debug ("Removing sink: index=%u", index);

        stream = g_hash_table_lookup (control->priv->sinks,
                                      GUINT_TO_POINTER (index));
        if (stream == NULL)
                return;

        device = gvc_mixer_control_lookup_device_from_stream (control, stream);

        if (device != NULL) {
                gvc_mixer_ui_device_invalidate_stream (device);
                if (!gvc_mixer_ui_device_has_ports (device)) {
                        g_signal_emit (G_OBJECT (control),
                                       signals[OUTPUT_REMOVED],
                                       0,
                                       gvc_mixer_ui_device_get_id (device));
                } else {
                        GList *devices, *d;

                        devices = g_hash_table_get_values (control->priv->ui_outputs);

                        for (d = devices; d != NULL; d = d->next) {
                                gint stream_id = GVC_MIXER_UI_DEVICE_INVALID;
                                device = d->data;
                                g_object_get (G_OBJECT (device),
                                             "stream-id", &stream_id,
                                              NULL);
                                if (stream_id == gvc_mixer_stream_get_id (stream))
                                        gvc_mixer_ui_device_invalidate_stream (device);
                        }

                        g_list_free (devices);
                }
        }

        g_hash_table_remove (control->priv->sinks,
                             GUINT_TO_POINTER (index));

        remove_stream (control, stream);
}