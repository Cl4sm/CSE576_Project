
   GtkFixedChild *child;
   GtkWidget *cwidget;
   ScConsole *cons;
   GList *children;

   #if SC_GTK_DEBUG_GTK
      printf("sc_display_console_set_fonts:  installing new console fonts for %p\n", (void *)dpy);
   #endif /* SC_GTK_DEBUG_GTK */
            
   g_return_if_fail(IS_SC_DISPLAY(dpy));
   g_return_if_fail(font != NULL && boldfont != NULL);

   children = GTK_FIXED(dpy)->children;

   if(children == NULL) return;
   children = children->next;
   while(children != NULL) {
      child = children->data;
      cwidget = child->widget;
      g_return_if_fail(IS_SC_CONSOLE(cwidget));
      cons = SC_CONSOLE(cwidget);
      #if SC_GTK_DEBUG_GTK
         printf("sc_display_console_set_fonts:  installing new console fonts for %p, registered console %p\n", 
                (void *)dpy, (void *)cons);
      #endif /* SC_GTK_DEBUG_GTK */

      sc_console_set_fonts(cons, font, boldfont);
      /* The font update will automatically adjust the console's 
         requested X/Y position, but we still need to tell the
         container to adjust the position.  */
      gtk_fixed_move(GTK_FIXED(dpy), cwidget, cons->req_alloc.x, cons->req_alloc.y);
      children = children->next;
   }

   #if SC_GTK_DEBUG_GTK
      printf("sc_display_console_set_fonts:  finished installing new console fonts for %p\n", (void *)dpy);
   #endif /* SC_GTK_DEBUG_GTK */

}
