void sc_dialog_text(const char *filename, GdkFont *font_normal, GdkFont *font_italic, GdkFont *font_bold) {

   ScDialog *dlg;
   char buf[SC_DIALOG_STRING_BUFFER];
   char out[SC_DIALOG_STRING_BUFFER];
   char *pin;
   char *pout;
   int  width;
   int height;
   FILE *f;
   GdkFont *font;
   GdkFont *lfont;
   GtkWidget *scroll;
   GtkWidget *message;
   GtkTextBuffer *textbuf;
   GtkTextIter iter;
   GtkTextTag *tag;

   if((f = fopen(filename, "r")) == NULL) {
      sbprintf(buf, sizeof(buf), "Cannot open file %s.\n", filename);
      sc_dialog_error(buf);
      return;
   }

   dlg = SC_DIALOG(sc_dialog_new(filename, NULL, SC_DIALOG_CLOSE | SC_DIALOG_NONMODAL));

   /* Load the needed fonts */
   if(font_normal != NULL) gdk_font_ref(font_normal);
   if(font_italic != NULL) gdk_font_ref(font_italic);
   if(font_bold   != NULL) gdk_font_ref(font_bold);
   if(font_normal == NULL) {
      width = 8;
      height = 16;
   } else {
      width = gdk_char_width(font_normal, 'W');
      height = (font_normal->ascent + font_normal->descent);
   }
   width  *= 88;
   height *= 33;

   scroll = gtk_scrolled_window_new(NULL, NULL);
   sc_dialog_grid_attach(dlg, scroll, 0, 0);
   gtk_widget_set_size_request(scroll, width, height);

   message = gtk_text_view_new();
   textbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(message));
   gtk_text_view_set_editable(GTK_TEXT_VIEW(message), FALSE);
   gtk_container_add(GTK_CONTAINER(scroll), message);

   /* Load in the text data */
   g_object_freeze_notify(G_OBJECT(message));
   while(fgets(buf, SC_DIALOG_STRING_BUFFER, f)) {
      pin   = buf;
      pout  = out;
      font  = NULL;
      lfont = font_normal;
      while(*pin != '\0') {
         if(*(pin + 1) != 0x08) {
            font = font_normal;
         } else {
            font = (*pin == '_' ? font_italic : font_bold);
            pin += 2;
         }
         if(font != lfont) {
            if(pout - out > 0) {
               if(lfont == font_italic) {
                  tag = gtk_text_buffer_create_tag(textbuf, NULL,
                                                   "weight", PANGO_WEIGHT_NORMAL,
                                                   "style",  PANGO_STYLE_ITALIC,
                                                   NULL);
               } else if(lfont == font_bold) {
                  tag = gtk_text_buffer_create_tag(textbuf, NULL,
                                                   "weight", PANGO_WEIGHT_BOLD,
                                                   "style",  PANGO_STYLE_NORMAL,
                                                   NULL);
               } else {
                  tag = gtk_text_buffer_create_tag(textbuf, NULL,
                                                   "weight", PANGO_WEIGHT_NORMAL,
                                                   "style",  PANGO_STYLE_NORMAL,
                                                   NULL);
               }
               gtk_text_buffer_get_end_iter(textbuf, &iter);
               gtk_text_buffer_insert_with_tags(textbuf, &iter, out, pout - out, tag, NULL);
            }
            pout  = out;
            lfont = font;
         }
         *(pout++) = *pin;
         if(*pin == '\n') {
            *(pout++) = ' ';
            *(pout++) = ' ';
         }
         pin++;
      }
      if(pout - out > 0) {
         if(lfont == font_italic) {
            tag = gtk_text_buffer_create_tag(textbuf, NULL,
                                             "weight", PANGO_WEIGHT_NORMAL,
                                             "style",  PANGO_STYLE_ITALIC,
                                             NULL);
         } else if(lfont == font_bold) {
            tag = gtk_text_buffer_create_tag(textbuf, NULL,
                                             "weight", PANGO_WEIGHT_BOLD,
                                             "style",  PANGO_STYLE_NORMAL,
                                             NULL);
         } else {
            tag = gtk_text_buffer_create_tag(textbuf, NULL,
                                             "weight", PANGO_WEIGHT_NORMAL,
                                             "style",  PANGO_STYLE_NORMAL,
                                             NULL);
         }
         gtk_text_buffer_get_end_iter(textbuf, &iter);
         gtk_text_buffer_insert_with_tags(textbuf, &iter, out, pout - out, tag, NULL);
      }
   }
   if(font_normal != NULL) {
      gdk_font_unref(font_normal);
   }
   if(font_italic != NULL) {
      gdk_font_unref(font_italic);
   }
   if(font_bold   != NULL) {
      gdk_font_unref(font_bold);
   }
   g_object_thaw_notify(G_OBJECT(message));
   fclose(f);

   sc_dialog_show(dlg);
   return;

}
