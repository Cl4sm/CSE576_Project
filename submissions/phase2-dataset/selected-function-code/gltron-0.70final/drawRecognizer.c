  float dirx;
  vec2 p, v;

  glPushMatrix();

  /* transformations */
  getRecognizerPositionVelocity(&p, &v);
  dirx = getRecognizerAngle(&v);

  glTranslatef( p.v[0], p.v[1], RECOGNIZER_HEIGHT);
  glRotatef(dirx, 0, 0, 1); /* up */
  
  glScalef(rec_scale_factor, rec_scale_factor, rec_scale_factor); 
  
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glLightfv(GL_LIGHT2, GL_SPECULAR, rec_spec_color); 
  glEnable(GL_LIGHT2);

  glDisable(GL_BLEND);

  glEnable(GL_CULL_FACE);
  
  if (gSettingsCache.light_cycles) {
    glEnable(GL_LIGHTING);
  }
  
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);

  glEnable(GL_NORMALIZE);
  glColor3f(0.0, 0.0, 0.0);
  drawModel(recognizer, TRI_MESH);

  glDisable(GL_POLYGON_OFFSET_FILL);

  glDisable(GL_LIGHT2);
  glEnable(GL_LIGHT1);
  glDisable(GL_LIGHTING);

  glColor3fv(rec_outline_color);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);
  drawModel(recognizer_quad, QUAD_MESH);
  glDisable(GL_LINE_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glDisable(GL_CULL_FACE);
  
  glPopMatrix();
}  
