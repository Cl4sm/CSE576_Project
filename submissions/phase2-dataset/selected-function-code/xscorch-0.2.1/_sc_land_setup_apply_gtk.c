static void _sc_land_setup_apply_gtk(__libj_unused ScDialog *dlg, sc_land_setup_data_gtk *setup) {

   sc_land *land = setup->land;

   land->sky        = sc_land_sky_types()[setup->skyidx];
   land->hostileenv = setup->hostileenv;
   land->generator  = sc_land_generator_types()[setup->generatoridx];
   land->bumpiness  = setup->bumpiness;
   
   sc_land_generate(setup->w->c, land);
   sc_window_paint((sc_window *)setup->w, 0, 0, land->width, land->height, 
                   SC_REGENERATE_LAND | SC_REDRAW_LAND);
   sc_pixmap_copy_gtk(sc_display_get_buffer(SC_DISPLAY(setup->w->screen)), 
                      sc_display_get_gc(SC_DISPLAY(setup->w->screen)), 
                      setup->w->logo, setup->w->logo_m, 
                      land->width - sc_pixmap_width_gtk(setup->w->logo), 
                      land->height - sc_pixmap_height_gtk(setup->w->logo));
   
   #if USE_NETWORK
   if(SC_NETWORK_SERVER(setup->c)) sc_net_server_send_config(setup->c, setup->c->server);
   #endif
   
}
