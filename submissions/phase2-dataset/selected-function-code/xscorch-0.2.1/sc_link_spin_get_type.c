GType sc_link_spin_get_type(void) {

   static GType sc_link_spin_type = 0;

   if(sc_link_spin_type == 0) {
      static const GTypeInfo sc_link_spin_info = {
         sizeof(ScLinkSpinClass),         /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_link_spin_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScLinkSpin),              /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_link_spin_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_link_spin_type = g_type_register_static(gtk_spin_button_get_type(), "ScLinkSpin",
                                                 &sc_link_spin_info, 0);
   }

   return(sc_link_spin_type);

}
