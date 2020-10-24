static void
_pa_ext_stream_restore_read_cb (pa_context                       *context,
                                const pa_ext_stream_restore_info *i,
                                int                               eol,
                                void                             *userdata)
{
        GvcMixerControl *control = GVC_MIXER_CONTROL (userdata);

        if (eol < 0) {
                g_debug ("Failed to initialized stream_restore extension: %s",
                         pa_strerror (pa_context_errno (context)));
                remove_event_role_stream (control);
                return;
        }

        if (eol > 0) {
                dec_outstanding (control);
                /* If we don't have an event stream to restore, then
                 * set one up with a default 100% volume */
                if (!control->priv->event_sink_input_is_set) {
                        pa_ext_stream_restore_info info;

                        memset (&info, 0, sizeof(info));
                        info.name = "sink-input-by-media-role:event";
                        info.volume.channels = 1;
                        info.volume.values[0] = PA_VOLUME_NORM;
                        update_event_role_stream (control, &info);
                }
                return;
        }

        update_event_role_stream (control, i);
}