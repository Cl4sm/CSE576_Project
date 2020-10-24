                                         guint n_param_values, const GValue *param_values,
                                         __libj_unused gpointer invocation_hint, gpointer marshal_data) {

   typedef gint (*GMarshalFunc_BOOLEAN__POINTER)(gpointer data1, gpointer arg_1,
                                                 gpointer data2);

   register GMarshalFunc_BOOLEAN__POINTER callback;
   register GCClosure *cc = (GCClosure *)closure;
   register gpointer data1, data2;
   gboolean v_return;

   g_return_if_fail(return_value != NULL);
   g_return_if_fail(n_param_values == 2);

   if(G_CCLOSURE_SWAP_DATA(closure)) {
      data1 = closure->data;
      data2 = g_value_peek_pointer(param_values + 0);
   } else {
      data1 = g_value_peek_pointer(param_values + 0);
      data2 = closure->data;
   }
   callback = (GMarshalFunc_BOOLEAN__POINTER)(marshal_data ? marshal_data : cc->callback);
   v_return = callback(data1, g_marshal_value_peek_pointer(param_values + 1), data2);
   g_value_set_boolean(return_value, v_return);

}
