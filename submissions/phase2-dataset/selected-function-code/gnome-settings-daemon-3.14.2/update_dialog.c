static void
update_dialog (GsdMediaKeysManager *manager,
               GvcMixerStream      *stream,
               guint                vol,
               gboolean             muted,
               gboolean             sound_changed,
               gboolean             quiet)
{
        GvcMixerUIDevice *device;
        const GvcMixerStreamPort *port;
        const char *icon;

        if (!muted) {
                vol = (int) (100 * (double) vol / PA_VOLUME_NORM);
                vol = CLAMP (vol, 0, 100);
        } else {
                vol = 0.0;
        }

        icon = get_icon_name_for_volume (!GVC_IS_MIXER_SINK (stream), muted, vol);
        port = gvc_mixer_stream_get_port (stream);
        if (g_strcmp0 (gvc_mixer_stream_get_form_factor (stream), "internal") != 0 ||
            (port != NULL &&
             g_strcmp0 (port->port, "analog-output-speaker") != 0 &&
             g_strcmp0 (port->port, "analog-output") != 0)) {
                device = gvc_mixer_control_lookup_device_from_stream (manager->priv->volume, stream);
                show_osd (manager, icon,
                          gvc_mixer_ui_device_get_description (device), vol);
        } else {
                show_osd (manager, icon, NULL, vol);
        }

        if (quiet == FALSE && sound_changed != FALSE && muted == FALSE) {
                ensure_canberra (manager);
                ca_context_change_device (manager->priv->ca,
                                          gvc_mixer_stream_get_name (stream));
                ca_context_play (manager->priv->ca, 1,
                                        CA_PROP_EVENT_ID, "audio-volume-change",
                                        CA_PROP_EVENT_DESCRIPTION, "volume changed through key press",
                                        CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                                        NULL);
        }
}