static void _sc_graphics_setup_apply_gtk(__libj_unused ScDialog *dlg,
                                         sc_graphics_setup_data_gtk *setup) {

   sc_config *c = setup->c;
   sc_window_gtk *w = (sc_window_gtk *)c->window;

   c->graphics.gfxanimate  = setup->gfxanimate;
   c->graphics.gfxfast     = setup->gfxfast;
   c->graphics.gfxcompfast = setup->gfxcompfast;

   if(c->fieldwidth != setup->fieldwidth || c->fieldheight != setup->fieldheight) {
      c->graphics.gfxdither = setup->gfxdither;
      c->fieldwidth  = setup->fieldwidth;
      c->fieldheight = setup->fieldheight;
      c->maxheight   = c->fieldheight - (SC_DEF_FIELD_HEIGHT - SC_DEF_MAX_HEIGHT);

      /* There went the landbuffer... */
      /* Colormap MUST be recalculated before land is regenerated! */
      sc_color_gradient_init(c, c->colors);

      /* Attempt to rebuild land */   
      sc_land_setup(c->land, c->fieldwidth, c->fieldheight, sc_land_flags(c));
      sc_land_generate(c, c->land);

      /*  We just seriously fsck'd things up  */
      
      /* Redraw everything */
      sc_window_resize(c->window);
      
   } else if(c->graphics.gfxdither != setup->gfxdither) {
      c->graphics.gfxdither = setup->gfxdither;

      /* Attempt to rebuild land */   
      sc_land_generate(c, c->land);
      sc_window_paint(c->window, 0, 0, c->land->width, c->land->height, SC_REGENERATE_LAND | SC_REDRAW_LAND);
      sc_pixmap_copy_gtk(sc_display_get_buffer(SC_DISPLAY(w->screen)), sc_display_get_gc(SC_DISPLAY(w->screen)), w->logo, w->logo_m, c->land->width - sc_pixmap_width_gtk(w->logo), c->land->height - sc_pixmap_height_gtk(w->logo));
   } /* Screen size was changed? */
   
   #if USE_NETWORK
   if(SC_NETWORK_SERVER(setup->c)) sc_net_server_send_config(setup->c, setup->c->server);
   #endif
   
}
