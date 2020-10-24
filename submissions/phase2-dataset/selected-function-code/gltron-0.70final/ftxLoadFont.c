  char *path;
  file_handle file;
  char buf[100];

  int i;
  int len;
  FontTex *ftx;
  
  path = getPath(PATH_DATA, filename);
  if(path == NULL) {
    fprintf(stderr, FTX_ERR "can't load font file '%s'\n", filename);
    return NULL;
  }
  file = file_open(path, "r");
  free(path);

  /* TODO(5): check for EOF errors in the following code */
  
  /* nTextures, texture width, char width */
  ftx = (FontTex*) malloc(sizeof(FontTex));
  getLine(buf, sizeof(buf), file);
  sscanf(buf, "%d %d %d ", &(ftx->nTextures), &(ftx->texwidth), &(ftx->width));
  /* lowest character, highest character */
  getLine(buf, sizeof(buf), file);
  sscanf(buf, "%d %d ", &(ftx->lower), &(ftx->upper));
  /* font name */
  getLine(buf, sizeof(buf), file);
  len = strlen(buf) + 1;

  ftx->fontname = (char*)malloc(len);
  memcpy(ftx->fontname, buf, len);

  /* prepare space for texture IDs  */
  ftx->texID = (GLuint*) malloc(ftx->nTextures * sizeof(unsigned int));
  glGenTextures(ftx->nTextures, ftx->texID);

  /* the individual textures */
  for(i = 0; i < ftx->nTextures; i++) {
    char *texname;
    getLine(buf, sizeof(buf), file);
    len = strlen(buf) + 1;
    if(buf[len - 2] == '\n') buf[len - 2] = 0;
    texname = (char*)malloc(len);
    memcpy(texname, buf, len); 
    glBindTexture(GL_TEXTURE_2D, ftx->texID[i]);
    loadTexture(texname, GL_RGBA);
    free(texname);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
    /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  }

  file_close(file);
  return ftx;
}
