/* sc_console_get_color
   Returns the GdkColor for the specified console attribute.  */

   g_return_val_if_fail(IS_SC_CONSOLE(cons), NULL);

   switch(rqst) {
   case SC_CONSOLE_FOREGROUND:
      return(&cons->colors.foreground);
   case SC_CONSOLE_BACKGROUND:
      return(&cons->colors.background);
   case SC_CONSOLE_FORECURSOR:
      return(&cons->colors.forecursor);
   case SC_CONSOLE_BACKCURSOR:
      return(&cons->colors.backcursor);
   case SC_CONSOLE_FORESHADOW:
      return(&cons->colors.foreshadow);
   case SC_CONSOLE_BACKSHADOW:
      return(&cons->colors.backshadow);
   case SC_CONSOLE_FORESCROLL:
      return(&cons->colors.forescroll);
   case SC_CONSOLE_BACKSCROLL:
      return(&cons->colors.backscroll);
   case SC_CONSOLE_FORELIGHT:
      return(&cons->colors.forelight);
   case SC_CONSOLE_BACKLIGHT:
      return(&cons->colors.backlight);
   case SC_CONSOLE_FOREDISABLED:
      return(&cons->colors.foredisabled);
   case SC_CONSOLE_BACKDISABLED:
      return(&cons->colors.backdisabled);
   case SC_CONSOLE_FORESTANDARD:
      return(&cons->colors.forestandard);
   case SC_CONSOLE_BACKSTANDARD:
      return(&cons->colors.backstandard);
   }
   return(NULL);

}
