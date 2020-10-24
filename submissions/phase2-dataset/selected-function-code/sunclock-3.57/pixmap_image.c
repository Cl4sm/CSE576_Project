XImage *
pixmap_image()
{
  int i, j, k, l, w, h;
  char *c;
  XImage *image;
  Visual visual;

  visual = *DefaultVisual(dpy, scr);

  w = mapwidth;
  h = mapheight;

  image = XCreateImage(dpy,&visual, 
      DefaultDepth(dpy, scr), ZPixmap, 0, NULL, w, h, color_pad, 0);
  if (!image) return NULL;

  bytes_per_pixel = (image->bits_per_pixel/8);
  image->data = (char *) salloc(image->bytes_per_line * h);

  if (color_depth>16)
  for (j=0; j<mapheight; j++) {
    c = image->data + j * image->bytes_per_line;
    for (i=0; i<mapwidth; i++) {
       k = bytes_per_pixel * i;
       l = grid[(i+1)*mapheight+j]/65536;
       if (l>=0 && l<num_palette) 
           l = palette[l];
       else
	   l = default_color;
       if (bigendian) {
          c[k+1] = (pixels[l] >> 16) & 255;
          c[k+2] = (pixels[l] >> 8) & 255;
          c[k+3] = pixels[l];
       } else {
          c[k] = pixels[l];
          c[k+1] = (pixels[l] >> 8) & 255;
          c[k+2] = (pixels[l] >> 16) & 255;
       }
    }
  }
  else
  if (color_depth>8)
  for (j=0; j<mapheight; j++) {
    c = image->data + j * mapwidth * bytes_per_pixel;
    for (i=0; i<mapwidth; i++) {
       k = bytes_per_pixel * i;
       l = grid[(i+1)*mapheight+j]/65536;
       if (l>=0 && l<num_palette) 
           l = palette[l];
       else
	 l = default_color;
       if (bigendian) {
          c[k] = pixels[l] / 256;
          c[k+1] = pixels[l] & 255;
       } else {
          c[k] = pixels[l] & 255;
          c[k+1] = pixels[l] / 256;
       }
    }
  }
  else
  for (j=0; j<mapheight; j++) {
    c = image->data + j * mapwidth * bytes_per_pixel;
    for (i=0; i<mapwidth; i++) {
       k = bytes_per_pixel * i;
       l = grid[(i+1)*mapheight+j]/65536;
       if (l>=0 && l<num_palette) 
           l = palette[l];
       else
	   l = default_color;
       c[k] = pixels[l] & 255;
    }
  }

  return image;
}
