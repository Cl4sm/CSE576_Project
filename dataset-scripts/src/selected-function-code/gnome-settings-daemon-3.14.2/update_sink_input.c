static void
update_sink_input (GvcMixerControl          *control,
                   const pa_sink_input_info *info)
{
        GvcMixerStream *stream;
        gboolean        is_new;
        pa_volume_t     max_volume;
        const char     *name;

#if 0
        g_debug ("Updating sink input: index=%u name='%s' client=%u sink=%u",
                 info->index,
                 info->name,
                 info->client,
                 info->sink);
#endif

        is_new = FALSE;

        stream = g_hash_table_lookup (control->priv->sink_inputs,
                                      GUINT_TO_POINTER (info->index));
        if (stream == NULL) {
                GvcChannelMap *map;
                map = gvc_channel_map_new_from_pa_channel_map (&info->channel_map);
                stream = gvc_mixer_sink_input_new (control->priv->pa_context,
                                                   info->index,
                                                   map);
                g_object_unref (map);
                is_new = TRUE;
        } else if (gvc_mixer_stream_is_running (stream)) {
                /* Ignore events if volume changes are outstanding */
                g_debug ("Ignoring event, volume changes are outstanding");
                return;
        }

        max_volume = pa_cvolume_max (&info->volume);

        name = (const char *)g_hash_table_lookup (control->priv->clients,
                                                  GUINT_TO_POINTER (info->client));
        gvc_mixer_stream_set_name (stream, name);
        gvc_mixer_stream_set_description (stream, info->name);

        set_application_id_from_proplist (stream, info->proplist);
        set_is_event_stream_from_proplist (stream, info->proplist);
        set_icon_name_from_proplist (stream, info->proplist, "applications-multimedia");
        gvc_mixer_stream_set_volume (stream, (guint)max_volume);
        gvc_mixer_stream_set_is_muted (stream, info->mute);
        gvc_mixer_stream_set_is_virtual (stream, info->client == PA_INVALID_INDEX);

        if (is_new) {
                g_hash_table_insert (control->priv->sink_inputs,
                                     GUINT_TO_POINTER (info->index),
                                     g_object_ref (stream));
                add_stream (control, stream);
        }
}