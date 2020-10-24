static void
parse_services_list (McdConnection *connection,
    const GPtrArray *services)
{
  guint i;
  GSList *e_numbers = NULL;

  for (i = 0; i < services->len; i++)
    {
      GValueArray *sp_info;
      GValueArray *sp;
      gchar **numbers;
      guint type;

      sp_info = g_ptr_array_index (services, i);
      sp = g_value_get_boxed (sp_info->values);
      type = g_value_get_uint (sp->values);

      if (type == TP_SERVICE_POINT_TYPE_EMERGENCY)
        {
          numbers = g_value_dup_boxed (sp_info->values + 1);
          e_numbers = g_slist_prepend (e_numbers, numbers);
        }
    }

  if (e_numbers != NULL)
    {
      GSList *service;
      TpConnection *tp_conn = mcd_connection_get_tp_connection (connection);

      /* FIXME: in 1.0, drop this and spec that when calling a service point,
       * you should use TargetID. See
       * https://bugs.freedesktop.org/show_bug.cgi?id=59162#c3 */
      for (service = e_numbers; service != NULL; service =g_slist_next (service))
        {
          const gchar * const *iter;

          for (iter = service->data; iter != NULL && *iter != NULL; iter++)
            tp_connection_dup_contact_by_id_async (tp_conn,
                *iter, 0, NULL, service_point_contact_cb,
                g_object_ref (connection));
        }

      _mcd_connection_take_emergency_numbers (connection, e_numbers);
    }
}