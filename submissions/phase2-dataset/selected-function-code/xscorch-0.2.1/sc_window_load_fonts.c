void sc_window_load_fonts(sc_window_gtk *w) {

   sc_config *c;

   assert(w != NULL);
   c = w->c;

   /* Load the necessary fonts */
   w->fixed_font = gdk_font_load(c->fixed_font);
   w->italic_fixed_font = gdk_font_load(c->italic_fixed_font);
   w->bold_fixed_font = gdk_font_load(c->bold_fixed_font);

   /* Make sure the fonts actually loaded */
   if(w->fixed_font == NULL) {
      printf("WARNING:  The font \"%s\" could not be loaded.  Trying \"fixed\"\n", c->fixed_font);
      w->fixed_font = gdk_font_load("fixed");
      if(w->fixed_font == NULL) {
         printf("ERROR:    Oh hell, you've got serious problems.  I couldn't find \"fixed\" either. Bailing out.\n");
         abort();
      }
   }
   if(w->italic_fixed_font == NULL) {
      printf("WARNING:  The font \"%s\" could not be loaded.  Trying \"%s\".  You won't have italic fonts.\n",
             c->italic_fixed_font, c->fixed_font);
      w->italic_fixed_font = w->fixed_font;
      gdk_font_ref(w->italic_fixed_font);
   }
   if(w->bold_fixed_font == NULL) {
      printf("WARNING:  The font \"%s\" could not be loaded.  Trying \"%s\".  You won't have bold fonts.\n",
             c->bold_fixed_font, c->fixed_font);
      w->bold_fixed_font = w->fixed_font;
      gdk_font_ref(w->bold_fixed_font);
   }

}
