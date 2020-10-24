static gint _sc_dialog_key_press(GtkWidget *widget, GdkEventKey *key) {
/* sc_dialog_key_press
   Process a keypress event in this dialog window.  */

   ScDialog *dlg = SC_DIALOG(widget);

   /* Check to see if enter or escape were pressed. */
   switch(key->keyval) {
      case GDK_Return:
      case GDK_KP_Enter:
         if(dlg->flags & SC_DIALOG_OK)          _sc_dialog_ok_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_YES)    _sc_dialog_ok_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_APPLY)  _sc_dialog_apply_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_CANCEL) _sc_dialog_cancel_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_NO)     _sc_dialog_cancel_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_CLOSE)  _sc_dialog_cancel_clicked(widget, dlg);
         else return(FALSE);
         return(TRUE);
      case GDK_Escape:
         if(dlg->flags & SC_DIALOG_CANCEL)      _sc_dialog_cancel_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_NO)     _sc_dialog_cancel_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_CLOSE)  _sc_dialog_cancel_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_OK)     _sc_dialog_ok_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_YES)    _sc_dialog_ok_clicked(widget, dlg);
         else if(dlg->flags & SC_DIALOG_APPLY)  _sc_dialog_apply_clicked(widget, dlg);
         else return(FALSE);
         return(TRUE);
   }

   /* Try to run parent keyhandler if we weren't able to do anything. */
   if(GTK_WIDGET_CLASS(parent_class)->key_press_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->key_press_event(widget, key)) {
         return(TRUE);
      }
   }

   /* We don't understand this key; defer to next handler. */
   return(FALSE);

}
