void drawGuiLogo(void) {
  float pos[] = { 512 - 10 - 320, 384 - 80 };
  float size[] = { 320, 80 };
  float glpos = 64;
  float glsize = 32;

  checkGLError("gui logo start");
  
  rasonly(gScreen);

  pos[0] *= gScreen->vp_w / 512.0f;
  pos[1] *= gScreen->vp_h / 384.0f;
  size[0] *= gScreen->vp_w / 512.0f;
  size[1] *= gScreen->vp_h / 384.0f;
  glpos *= gScreen->vp_w / 512.0f;
  glsize *= gScreen->vp_w / 512.0f;
  
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, gScreen->textures[TEX_LOGO]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);

	{ 
		// float texy = 1.0f - 256.0f / 320.0f;
		float texy = 0.0f;
		glTexCoord2f(0.0, texy);
		glVertex2f(pos[0], pos[1]);

		glTexCoord2f(1.0, texy);
		glVertex2f(pos[0] + size[0], pos[1]);

		glTexCoord2f(1.0, 1.0);
		glVertex2f(pos[0] + size[0], pos[1] + size[1]);

		glTexCoord2f(0.0, 1.0);
		glVertex2f(pos[0], pos[1] + size[1]);
	}
	
  glEnd();

  glDisable(GL_BLEND);

  checkGLError("gui background end");
}
