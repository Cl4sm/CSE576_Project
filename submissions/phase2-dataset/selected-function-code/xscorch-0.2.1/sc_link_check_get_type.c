GType sc_link_check_get_type(void) {

   static GType sc_link_check_type = 0;

   if(sc_link_check_type == 0) {
      static const GTypeInfo sc_link_check_info = {
         sizeof(ScLinkCheckClass),        /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_link_check_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScLinkCheck),             /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_link_check_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_link_check_type = g_type_register_static(gtk_check_button_get_type(), "ScLinkCheck",
                                                  &sc_link_check_info, 0);
   }

   return(sc_link_check_type);

}
