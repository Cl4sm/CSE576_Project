#else
void PWGetUnzoomedPixmap(w, pixmap, pixmap_mask)
     Widget w;
     Pixmap *pixmap, *pixmap_mask;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    XImage *mask_image, *image;
    
    if (PW->pixmap.zooming) 
      {    
	image = CreatePixmapImage(PW,
				  (Dimension) PW->pixmap.zoom.image->width,
				  (Dimension) PW->pixmap.zoom.image->height);
	CopyImageData(PW->pixmap.zoom.image, image, 0, 0, 
		      (Dimension) PW->pixmap.zoom.image->width,
		      (Dimension) PW->pixmap.zoom.image->height, 0, 0);
	CopyImageData(PW->pixmap.image, image, 
		      0, 0, 
		      PW->pixmap.image->width - 1,
		      PW->pixmap.image->height - 1,
		      PW->pixmap.zoom.at_x, PW->pixmap.zoom.at_y);

	mask_image = CreateMaskImage(PW,
				     (Dimension)PW->pixmap.zoom.image->width,
				     (Dimension)PW->pixmap.zoom.image->height);
	CopyImageData(PW->pixmap.zoom.mask_image, mask_image, 0, 0, 
		      (Dimension) PW->pixmap.zoom.mask_image->width,
		      (Dimension) PW->pixmap.zoom.mask_image->height, 0, 0);
	CopyImageData(PW->pixmap.mask_image, mask_image, 
		      0, 0, 
		      PW->pixmap.mask_image->width - 1,
		      PW->pixmap.mask_image->height - 1,
		      PW->pixmap.zoom.at_x, PW->pixmap.zoom.at_y);
	
      }
    else
      {
	  image = PW->pixmap.image;
	  mask_image = PW->pixmap.mask_image;
      }

    
    *pixmap = GetPixmap(PW, image);
    *pixmap_mask = GetPixmap(PW, mask_image);

    if (PW->pixmap.zooming) {
	DestroyPixmapImage(&image);
	DestroyMaskImage(&mask_image);
    }
}
