static void
_pa_context_subscribe_cb (pa_context                  *context,
                          pa_subscription_event_type_t t,
                          uint32_t                     index,
                          void                        *userdata)
{
        GvcMixerControl *control = GVC_MIXER_CONTROL (userdata);

        switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK) {
        case PA_SUBSCRIPTION_EVENT_SINK:
                if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                        remove_sink (control, index);
                } else {
                        req_update_sink_info (control, index);
                }
                break;

        case PA_SUBSCRIPTION_EVENT_SOURCE:
                if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                        remove_source (control, index);
                } else {
                        req_update_source_info (control, index);
                }
                break;

        case PA_SUBSCRIPTION_EVENT_SINK_INPUT:
                if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                        remove_sink_input (control, index);
                } else {
                        req_update_sink_input_info (control, index);
                }
                break;

        case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT:
                if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                        remove_source_output (control, index);
                } else {
                        req_update_source_output_info (control, index);
                }
                break;

        case PA_SUBSCRIPTION_EVENT_CLIENT:
                if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                        remove_client (control, index);
                } else {
                        req_update_client_info (control, index);
                }
                break;

        case PA_SUBSCRIPTION_EVENT_SERVER:
                req_update_server_info (control, index);
                break;

        case PA_SUBSCRIPTION_EVENT_CARD:
                if ((t & PA_SUBSCRIPTION_EVENT_TYPE_MASK) == PA_SUBSCRIPTION_EVENT_REMOVE) {
                        remove_card (control, index);
                } else {
                        req_update_card (control, index);
                }
                break;
        }
}