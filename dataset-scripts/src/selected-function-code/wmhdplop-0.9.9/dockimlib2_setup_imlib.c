static void dockimlib2_setup_imlib(DockImlib2 *dock) {
  char fp[512];
   /* set the maximum number of colors to allocate for 8bpp and less to 32 */
  imlib_set_color_usage(32);
  /* dither for depths < 24bpp */
  imlib_context_set_dither(1);
  /* set the display , visual, colormap and drawable we are using */
  imlib_context_set_display(dock->display);
  imlib_context_set_visual(dock->visual);
  imlib_context_set_colormap(dock->colormap);
  imlib_context_set_drawable(dock->win);
  dock->img = imlib_create_image(dock->w, dock->h);
  imlib_context_set_image(dock->img);

  /* some default font paths */
  snprintf(fp, 512, "%s/.fonts", getenv("HOME"));
  add_fontpath(fp,1);
  /*add_fontpath("/usr/share/fonts/truetype",1);
  add_fontpath("/usr/share/fonts/ttf",1);*/
  add_fontpath("/usr/share/fonts",1);
  add_fontpath("/usr/X11R6/lib/X11/fonts/truetype",1);
  add_fontpath("/usr/X11R6/lib/X11/fonts/TrueType",1);
  add_fontpath("/usr/X11R6/lib/X11/fonts/ttf",1);
  add_fontpath("/usr/X11R6/lib/X11/fonts/TTF",1);
  /*imlib_add_path_to_font_path(fp);
  imlib_add_path_to_font_path("/usr/share/fonts/truetype");
  imlib_add_path_to_font_path("/usr/share/fonts/truetype/freefont");
  imlib_add_path_to_font_path("/usr/share/fonts/truetype/ttf-bitstream-vera");
  imlib_add_path_to_font_path("/usr/share/fonts/ttf/vera");*/ /* vera on mdk */
  imlib_context_set_TTF_encoding(IMLIB_TTF_ENCODING_ISO_8859_1);
  //imlib_set_cache_size(0);imlib_set_font_cache_size(0);
}