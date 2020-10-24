static void
create_ui_device_from_port (GvcMixerControl* control,
                            GvcMixerCardPort* port,
                            GvcMixerCard* card)
{
        GvcMixerUIDeviceDirection  direction;
        GObject                   *object;
        GvcMixerUIDevice          *uidevice;
        gboolean                   available = port->available != PA_PORT_AVAILABLE_NO;

        direction = (is_card_port_an_output (port) == TRUE) ? UIDeviceOutput : UIDeviceInput;

        object = g_object_new (GVC_TYPE_MIXER_UI_DEVICE,
                               "type", (uint)direction,
                               "card", card,
                               "port-name", port->port,
                               "description", port->human_port,
                               "origin", gvc_mixer_card_get_name (card),
                               "port-available", available,
                               NULL);

        uidevice = GVC_MIXER_UI_DEVICE (object);
        gvc_mixer_ui_device_set_profiles (uidevice, port->profiles);

        g_hash_table_insert (is_card_port_an_output (port) ? control->priv->ui_outputs : control->priv->ui_inputs,
                             GUINT_TO_POINTER (gvc_mixer_ui_device_get_id (uidevice)),
                             g_object_ref (uidevice));


        if (available) {
                g_signal_emit (G_OBJECT (control),
                               signals[is_card_port_an_output (port) ? OUTPUT_ADDED : INPUT_ADDED],
                               0,
                               gvc_mixer_ui_device_get_id (uidevice));
        }

        g_debug ("create_ui_device_from_port, direction %u, description '%s', origin '%s', port available %i", 
                 direction,
                 port->human_port,
                 gvc_mixer_card_get_name (card),
                 available);
}