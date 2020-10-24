GType sc_drawbuf_get_type(void) {

   static GType sc_drawbuf_type = 0;

   if(sc_drawbuf_type == 0) {
      static const GTypeInfo sc_drawbuf_info = {
         sizeof(ScDrawbufClass),          /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_drawbuf_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScDrawbuf),               /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_drawbuf_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_drawbuf_type = g_type_register_static(gtk_drawing_area_get_type(), "ScDrawbuf",
                                               &sc_drawbuf_info, 0);
   }

   return(sc_drawbuf_type);

}
