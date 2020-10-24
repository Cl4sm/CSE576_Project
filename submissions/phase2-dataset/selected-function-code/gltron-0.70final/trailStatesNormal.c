	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	// glDisable(GL_TEXTURE_2D);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	{ 
		float black[] = { 0, 0, 0, 1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	}

	if(gSettingsCache.alpha_trails) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
