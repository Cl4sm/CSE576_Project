  /* states */

  if(gSettingsCache.use_stencil) {
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_GREATER, 1, 1);
    glEnable(GL_BLEND);
    glColor4fv(shadow_color);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glColor3f(0, 0, 0);
    glDisable(GL_BLEND);
  }

  /* transformation */

  glPushMatrix();
  glMultMatrixf(shadow_matrix);

  /* geometry */
	{
		int vOffset = 0;
		int iOffset = 0;

		TrailMesh mesh;
		mesh.pVertices = (vec3*) malloc(1000 * sizeof(vec3));
		mesh.pNormals = (vec3*) malloc(1000 * sizeof(vec3));
		mesh.pColors = (unsigned char*) malloc(1000 * 4 * sizeof(float));
		mesh.pTexCoords = (vec2*) malloc(1000 * sizeof(vec2));
		mesh.pIndices = (unsigned short*) malloc(1000 * 2);
		mesh.iUsed = 0;
		
		trailGeometry(p, pV, &mesh, &vOffset, &iOffset);
		bowGeometry(p, pV, &mesh, &vOffset, &iOffset);
		trailStatesShadowed();
		trailRender(&mesh);
		// no states restore, because we're drawing shadowed geometry

		free(mesh.pVertices);
		free(mesh.pNormals);
		free(mesh.pColors);
		free(mesh.pTexCoords);
		free(mesh.pIndices);
	}

  /* restore */

  if(gSettingsCache.use_stencil)
    glDisable(GL_STENCIL_TEST);

  glDisable(GL_BLEND);

  glPopMatrix();
}
