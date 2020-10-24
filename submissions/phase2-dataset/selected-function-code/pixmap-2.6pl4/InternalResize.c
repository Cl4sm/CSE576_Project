     PixmapWidget PW;
{
    Dimension w, h, rw, rh, oldw, oldh;
    XtGeometryResult status;

    oldw = PW->core.width;
    oldh = PW->core.height;
    w = 2 * (int)PW->pixmap.distance +
      (int)PW->pixmap.width * PW->pixmap.squareW;
    h = 2 * (int)PW->pixmap.distance +
      (int)PW->pixmap.height * PW->pixmap.squareH;
    status = XtMakeResizeRequest((Widget)PW, w, h, &rw, &rh);

    if (status == XtGeometryAlmost)
      status = XtMakeResizeRequest((Widget)PW, rw, rh, NULL, NULL);
    else if (status == XtGeometryNo)
      XtWarning("PixmapWidget: Geometry request denied");

    return ((oldw != PW->core.width) || (oldh != PW->core.height));
}
