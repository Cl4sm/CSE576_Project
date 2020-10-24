static void
update_sink (GvcMixerControl    *control,
             const pa_sink_info *info)
{
        GvcMixerStream  *stream;
        gboolean        is_new;
        pa_volume_t     max_volume;
        GvcChannelMap   *map;
        char            map_buff[PA_CHANNEL_MAP_SNPRINT_MAX];

        pa_channel_map_snprint (map_buff, PA_CHANNEL_MAP_SNPRINT_MAX, &info->channel_map);
#if 1
        g_debug ("Updating sink: index=%u name='%s' description='%s' map='%s'",
                 info->index,
                 info->name,
                 info->description,
                 map_buff);
#endif

        map = NULL;
        is_new = FALSE;
        stream = g_hash_table_lookup (control->priv->sinks,
                                      GUINT_TO_POINTER (info->index));

        if (stream == NULL) {
                GList *list = NULL;
                guint i;

                map = gvc_channel_map_new_from_pa_channel_map (&info->channel_map);
                stream = gvc_mixer_sink_new (control->priv->pa_context,
                                             info->index,
                                             map);

                for (i = 0; i < info->n_ports; i++) {
                        GvcMixerStreamPort *port;

                        port = g_slice_new0 (GvcMixerStreamPort);
                        port->port = g_strdup (info->ports[i]->name);
                        port->human_port = g_strdup (info->ports[i]->description);
                        port->priority = info->ports[i]->priority;
                        port->available = info->ports[i]->available != PA_PORT_AVAILABLE_NO;

                        list = g_list_prepend (list, port);
                }
                gvc_mixer_stream_set_ports (stream, list);

                g_object_unref (map);
                is_new = TRUE;

        } else if (gvc_mixer_stream_is_running (stream)) {
                /* Ignore events if volume changes are outstanding */
                g_debug ("Ignoring event, volume changes are outstanding");
                return;
        }

        max_volume = pa_cvolume_max (&info->volume);
        gvc_mixer_stream_set_name (stream, info->name);
        gvc_mixer_stream_set_card_index (stream, info->card);
        gvc_mixer_stream_set_description (stream, info->description);
        set_icon_name_from_proplist (stream, info->proplist, "audio-card");
        gvc_mixer_stream_set_form_factor (stream, pa_proplist_gets (info->proplist, PA_PROP_DEVICE_FORM_FACTOR));
        gvc_mixer_stream_set_sysfs_path (stream, pa_proplist_gets (info->proplist, "sysfs.path"));
        gvc_mixer_stream_set_volume (stream, (guint)max_volume);
        gvc_mixer_stream_set_is_muted (stream, info->mute);
        gvc_mixer_stream_set_can_decibel (stream, !!(info->flags & PA_SINK_DECIBEL_VOLUME));
        gvc_mixer_stream_set_base_volume (stream, (guint32) info->base_volume);

        /* Messy I know but to set the port everytime regardless of whether it has changed will cost us a
         * port change notify signal which causes the frontend to resync.
         * Only update the UI when something has changed. */
        if (info->active_port != NULL) {
                if (is_new)
                        gvc_mixer_stream_set_port (stream, info->active_port->name);
                else {
                        const GvcMixerStreamPort *active_port;
                        active_port = gvc_mixer_stream_get_port (stream);
                        if (active_port == NULL ||
                            g_strcmp0 (active_port->port, info->active_port->name) != 0) {
                                g_debug ("update sink - apparently a port update");
                                gvc_mixer_stream_set_port (stream, info->active_port->name);
                        }
                }
        }

        if (is_new) {
                g_debug ("update sink - is new");

                g_hash_table_insert (control->priv->sinks,
                                     GUINT_TO_POINTER (info->index),
                                     g_object_ref (stream));
                add_stream (control, stream);
                /* Always sink on a new stream to able to assign the right stream id
                 * to the appropriate outputs (multiple potential outputs per stream). */
                sync_devices (control, stream);
        }

        /*
         * When we change profile on a device that is not the server default sink,
         * it will jump back to the default sink set by the server to prevent the audio setup from being 'outputless'.
         * All well and good but then when we get the new stream created for the new profile how do we know
         * that this is the intended default or selected device the user wishes to use.
         * This is messy but it's the only reliable way that it can be done without ripping the whole thing apart.
         */
        if (control->priv->profile_swapping_device_id != GVC_MIXER_UI_DEVICE_INVALID) {
                GvcMixerUIDevice *dev = NULL;
                dev = gvc_mixer_control_lookup_output_id (control, control->priv->profile_swapping_device_id);
                if (dev != NULL) {
                        /* now check to make sure this new stream is the same stream just matched and set on the device object */
                        if (gvc_mixer_ui_device_get_stream_id (dev) == gvc_mixer_stream_get_id (stream)) {
                                g_debug ("Looks like we profile swapped on a non server default sink");
                                gvc_mixer_control_set_default_sink (control, stream);
                        }
                }
                control->priv->profile_swapping_device_id = GVC_MIXER_UI_DEVICE_INVALID;
        }

        if (control->priv->default_sink_name != NULL
            && info->name != NULL
            && strcmp (control->priv->default_sink_name, info->name) == 0) {
                _set_default_sink (control, stream);
        }

        if (map == NULL)
                map = (GvcChannelMap *) gvc_mixer_stream_get_channel_map (stream);

        gvc_channel_map_volume_changed (map, &info->volume, FALSE);
}