  int filesize;
  FILE * ofp;

  filesize = 3*width*height + 18 - 10;
  
  if (name==NULL) {
    return IMAGEWRITEERR;
  } else {
    ofp=fopen(name, "w+b");
    if (ofp == NULL) {
      char msgtxt[2048];
      sprintf(msgtxt, "Cannot create %s for output!", name);
      rt_ui_message(MSG_ERR, msgtxt);
      rt_ui_message(MSG_ABORT, "Rendering Aborted.");
      return IMAGEWRITEERR;
    } 

    fputc(0, ofp); /* IdLength      */
    fputc(0, ofp); /* ColorMapType  */
    fputc(2, ofp); /* ImageTypeCode */
    fputc(0, ofp); /* ColorMapOrigin, low byte */
    fputc(0, ofp); /* ColorMapOrigin, high byte */
    fputc(0, ofp); /* ColorMapLength, low byte */
    fputc(0, ofp); /* ColorMapLength, high byte */
    fputc(0, ofp); /* ColorMapEntrySize */
    fputc(0, ofp); /* XOrigin, low byte */
    fputc(0, ofp); /* XOrigin, high byte */
    fputc(0, ofp); /* YOrigin, low byte */
    fputc(0, ofp); /* YOrigin, high byte */
    fputc((width & 0xff),         ofp); /* Width, low byte */
    fputc(((width >> 8) & 0xff),  ofp); /* Width, high byte */
    fputc((height & 0xff),        ofp); /* Height, low byte */
    fputc(((height >> 8) & 0xff), ofp); /* Height, high byte */
    fputc(24, ofp);   /* ImagePixelSize */
    fputc(0x20, ofp); /* ImageDescriptorByte 0x20 == flip vertically */

    fseek(ofp, filesize, 0);
    fprintf(ofp, "9876543210"); 

    fclose(ofp);
  } 

  return IMAGENOERR;
}    
