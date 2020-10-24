static void
req_update_source_info (GvcMixerControl *control,
                        int              index)
{
        pa_operation *o;

        if (index < 0) {
                o = pa_context_get_source_info_list (control->priv->pa_context,
                                                     _pa_context_get_source_info_cb,
                                                     control);
        } else {
                o = pa_context_get_source_info_by_index(control->priv->pa_context,
                                                        index,
                                                        _pa_context_get_source_info_cb,
                                                        control);
        }

        if (o == NULL) {
                g_warning ("pa_context_get_source_info_list() failed");
                return;
        }
        pa_operation_unref (o);
}