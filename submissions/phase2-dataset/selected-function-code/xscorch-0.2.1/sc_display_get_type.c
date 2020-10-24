
   static GType sc_display_type = 0;

   if(sc_display_type == 0) {
      static const GTypeInfo sc_display_info = {
         sizeof(ScDisplayClass),          /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         NULL,                            /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScDisplay),               /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_display_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_display_type = g_type_register_static(gtk_fixed_get_type(), "ScDisplay",
                                               &sc_display_info, 0);
   }

   return(sc_display_type);

}
