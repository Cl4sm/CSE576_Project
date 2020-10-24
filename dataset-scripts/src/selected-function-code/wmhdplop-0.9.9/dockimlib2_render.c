void dockimlib2_render(DockImlib2 *dock) {
  Pixmap olddrawable = imlib_context_get_drawable();
  Imlib_Image oldimage = imlib_context_get_image();
  //imlib_context_set_drawable(dock->win);
  imlib_context_set_image(dock->img);
  if (imlib_image_has_alpha()) {
    imlib_context_set_image(dock->bg);
    Imlib_Image bg = imlib_clone_image();
    imlib_context_set_image(bg);
    imlib_blend_image_onto_image(dock->img, 0, 0, 0, dock->w, dock->h, 0, 0, dock->w, dock->h);

    if (dock->normalwin) {
      imlib_context_set_drawable(dock->normalwin);
      imlib_render_image_on_drawable(dock->x0, dock->y0);
    }
    if (dock->iconwin) {
      imlib_context_set_drawable(dock->iconwin);
      imlib_render_image_on_drawable(dock->x0, dock->y0);
    }    
    /* XSetWindowBackgroundPixmap(dock->display, dock->GKwin, dock->win);
       XClearWindow(dock->display, dock->GKWin); */
    imlib_free_image();
  } else {
    if (dock->normalwin_mapped && dock->normalwin) {
      imlib_context_set_drawable(dock->normalwin);
      imlib_render_image_on_drawable(dock->x0, dock->y0); /* imlib_render_image_on_drawable generates many pages faults !? */
    }
    if (dock->iconwin_mapped && dock->iconwin) {
      imlib_context_set_drawable(dock->iconwin);
      imlib_render_image_on_drawable(dock->x0, dock->y0);
    }
  }
  imlib_context_set_image(oldimage);
  imlib_context_set_drawable(olddrawable);
}