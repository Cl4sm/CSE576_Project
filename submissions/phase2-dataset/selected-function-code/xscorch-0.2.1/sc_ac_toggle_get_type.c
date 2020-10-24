GType sc_ac_toggle_get_type(void) {
/* sc_ac_toggle_get_type
   Define the toggle's GTK type. */

   static GType sc_ac_toggle_type = 0;

   if(sc_ac_toggle_type == 0) {
      static const GTypeInfo sc_ac_toggle_info = {
         sizeof(ScACToggleClass),         /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_ac_toggle_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScACToggle),              /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_ac_toggle_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_ac_toggle_type = g_type_register_static(sc_gadget_get_type(), "ScACToggle",
                                                 &sc_ac_toggle_info, 0);
   }

   return(sc_ac_toggle_type);

}
