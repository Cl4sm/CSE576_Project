static void
update_source_output (GvcMixerControl             *control,
                      const pa_source_output_info *info)
{
        GvcMixerStream *stream;
        gboolean        is_new;
        const char     *name;

#if 1
        g_debug ("Updating source output: index=%u name='%s' client=%u source=%u",
                 info->index,
                 info->name,
                 info->client,
                 info->source);
#endif

        is_new = FALSE;
        stream = g_hash_table_lookup (control->priv->source_outputs,
                                      GUINT_TO_POINTER (info->index));
        if (stream == NULL) {
                GvcChannelMap *map;
                map = gvc_channel_map_new_from_pa_channel_map (&info->channel_map);
                stream = gvc_mixer_source_output_new (control->priv->pa_context,
                                                      info->index,
                                                      map);
                g_object_unref (map);
                is_new = TRUE;
        }

        name = (const char *)g_hash_table_lookup (control->priv->clients,
                                                  GUINT_TO_POINTER (info->client));

        gvc_mixer_stream_set_name (stream, name);
        gvc_mixer_stream_set_description (stream, info->name);
        set_application_id_from_proplist (stream, info->proplist);
        set_is_event_stream_from_proplist (stream, info->proplist);
        set_icon_name_from_proplist (stream, info->proplist, "audio-input-microphone");

        if (is_new) {
                g_hash_table_insert (control->priv->source_outputs,
                                     GUINT_TO_POINTER (info->index),
                                     g_object_ref (stream));
                add_stream (control, stream);
        }
}