GType sc_console_get_type(void) {
/* sc_console_get_type
   Return the console type.  */

   static GType sc_console_type = 0;

   if(sc_console_type == 0) {
      static const GTypeInfo sc_console_info = {
         sizeof(ScConsoleClass),          /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_console_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScConsole),               /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_console_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_console_type = g_type_register_static(sc_drawbuf_get_type(), "ScConsole",
                                               &sc_console_info, 0);
   }

   return(sc_console_type);

}
