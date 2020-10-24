  texture *tex;
  GLint internal;
	int maxSize;
	
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	
  tex = loadTextureData(filename);
  if(tex->channels == 3) internal = GL_RGB;
  else internal = GL_RGBA;
  if(format == GL_DONT_CARE) {
    if(tex->channels == 3) format = GL_RGB;
    if(tex->channels == 4) format = GL_RGBA;
  }
  /* TODO: build mipmaps the proper way, box filters suck */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	{
		texture *newtex;
		int level = 0;
    while (tex->width > 1 || tex->height > 1) {
			if(tex->width <= maxSize && tex->height <= maxSize) {
					glTexImage2D(GL_TEXTURE_2D, level, format, 
											 tex->width, tex->height,
											 0, internal, GL_UNSIGNED_BYTE, tex->data);
#ifdef PRINTF_VERBOSE
					printf("uploading level %d, %dx%d texture\n", 
						 level, tex->width, tex->height);
#endif
					level++;
			}
      newtex = mipmap_png_texture(tex, 1, 0, 0);
      freeTextureData(tex);
      tex = newtex;
		}
		/* upload 1x1 mip level */
		glTexImage2D(GL_TEXTURE_2D, level, format, 
								 tex->width, tex->height,
								 0, internal, GL_UNSIGNED_BYTE, tex->data);
#ifdef PRINTF_VERBOSE
		printf("uploading level %d, %dx%d texture\n", 
					 level, tex->width, tex->height);
#endif
		freeTextureData(tex);
	}
}
