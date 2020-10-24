static void
update_event_role_stream (GvcMixerControl                  *control,
                          const pa_ext_stream_restore_info *info)
{
        GvcMixerStream *stream;
        gboolean        is_new;
        pa_volume_t     max_volume;

        if (strcmp (info->name, "sink-input-by-media-role:event") != 0) {
                return;
        }

#if 0
        g_debug ("Updating event role: name='%s' device='%s'",
                 info->name,
                 info->device);
#endif

        is_new = FALSE;

        if (!control->priv->event_sink_input_is_set) {
                pa_channel_map pa_map;
                GvcChannelMap *map;

                pa_map.channels = 1;
                pa_map.map[0] = PA_CHANNEL_POSITION_MONO;
                map = gvc_channel_map_new_from_pa_channel_map (&pa_map);

                stream = gvc_mixer_event_role_new (control->priv->pa_context,
                                                   info->device,
                                                   map);
                control->priv->event_sink_input_id = gvc_mixer_stream_get_id (stream);
                control->priv->event_sink_input_is_set = TRUE;

                is_new = TRUE;
        } else {
                stream = g_hash_table_lookup (control->priv->all_streams,
                                              GUINT_TO_POINTER (control->priv->event_sink_input_id));
        }

        max_volume = pa_cvolume_max (&info->volume);

        gvc_mixer_stream_set_name (stream, _("System Sounds"));
        gvc_mixer_stream_set_icon_name (stream, "multimedia-volume-control");
        gvc_mixer_stream_set_volume (stream, (guint)max_volume);
        gvc_mixer_stream_set_is_muted (stream, info->mute);

        if (is_new) {
                add_stream (control, stream);
        }
}