GType sc_link_spinf_get_type(void) {

   static GType sc_link_spinf_type = 0;

   if(sc_link_spinf_type == 0) {
      static const GTypeInfo sc_link_spinf_info = {
         sizeof(ScLinkSpinFClass),        /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_link_spinf_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScLinkSpinF),             /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_link_spinf_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_link_spinf_type = g_type_register_static(gtk_spin_button_get_type(), "ScLinkSpinF",
                                                  &sc_link_spinf_info, 0);
   }

   return(sc_link_spinf_type);

}
