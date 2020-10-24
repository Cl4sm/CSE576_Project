GType sc_gadget_get_type(void) {

   static GType sc_gadget_type = 0;

   if(sc_gadget_type == 0) {
      static const GTypeInfo sc_gadget_info = {
         sizeof(ScGadgetClass),           /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_gadget_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScGadget),                /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_gadget_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_gadget_type = g_type_register_static(gtk_object_get_type(), "ScGadget",
                                              &sc_gadget_info, 0);
   }

   return(sc_gadget_type);

}
