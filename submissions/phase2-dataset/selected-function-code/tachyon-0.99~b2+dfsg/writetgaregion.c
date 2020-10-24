                    int stopx, int stopy, unsigned char * buffer) {
  int x, y, totalx, totaly, xbytes, widthbytes, regionstart;
  unsigned char * bufpos;
  int filepos, numbytes;
  tgahandle * tga = (tgahandle *) voidhandle;
  unsigned char * fixbuf; 

  totalx = stopx - startx + 1;
  totaly = stopy - starty + 1;
  xbytes = totalx*3;
  widthbytes = tga->width*3;
  fixbuf = (unsigned char *) malloc(xbytes);
  if (fixbuf == NULL) {
    rt_ui_message(MSG_ERR, "writetgaregion: failed memory allocation!\n");
    return;
  }
 
  regionstart = 18 + (startx-1)*3 + widthbytes*(tga->height-starty-totaly+1);
  if (totalx == tga->width) {
    filepos=regionstart;
    if (filepos >= 18) {
      fseek(tga->ofp, filepos, 0); 
    } else {
      rt_ui_message(MSG_ERR, "writetgaregion: file ptr out of range!!!\n");
      free(fixbuf);
      return;  /* don't try to continue */
    }

    for (y=0; y<totaly; y++) {
      bufpos=buffer + xbytes*(totaly-y-1);
      for (x=0; x<xbytes; x+=3) {
        fixbuf[x    ] = bufpos[x + 2];
        fixbuf[x + 1] = bufpos[x + 1];
        fixbuf[x + 2] = bufpos[x    ];
      }
      numbytes = fwrite(fixbuf, 1, xbytes, tga->ofp);
      if (numbytes != xbytes) {
        char msgtxt[256];
        sprintf(msgtxt, "File write problem, %d bytes written.", numbytes);  
        rt_ui_message(MSG_ERR, msgtxt);
        free(fixbuf);
        return;  /* don't try to continue */
      }
    }
  } else {
    for (y=0; y<totaly; y++) {
      bufpos=buffer + xbytes*(totaly-y-1);
      filepos=regionstart + widthbytes*y;

      if (filepos >= 18) {
        fseek(tga->ofp, filepos, 0); 
  
        for (x=0; x<xbytes; x+=3) {
          fixbuf[x    ] = bufpos[x + 2];
          fixbuf[x + 1] = bufpos[x + 1];
          fixbuf[x + 2] = bufpos[x    ];
        }
 
        numbytes = fwrite(fixbuf, 1, xbytes, tga->ofp);
        if (numbytes != xbytes) {
          char msgtxt[256];
          sprintf(msgtxt, "File write problem, %d bytes written.", numbytes);  
          rt_ui_message(MSG_ERR, msgtxt);
          free(fixbuf);
          return;  /* don't try to continue */
        }
      } else {
        rt_ui_message(MSG_ERR, "writetgaregion: file ptr out of range!!!\n");
        free(fixbuf);
        return;  /* don't try to continue */
      }
    }
  }

  free(fixbuf);
}
