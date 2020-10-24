  int i;
  float left[3], right[3];
  float zunit[3] = {0, 0, 1};
  
  float vectors[NUM_SPIRES][3] = {
    {  1.00f,  0.20f,  0.00f  },
    {  0.80f,  0.25f,  0.00f  },
    {  0.90f,  0.50f,  0.00f  },
    {  0.70f,  0.50f,  0.00f  },
    {  0.52f,  0.45f,  0.00f  },
    {  0.65f,  0.75f,  0.00f  },
    {  0.42f,  0.68f,  0.00f  },
    {  0.40f,  1.02f,  0.00f  },
    {  0.20f,  0.90f,  0.00f  },
    {  0.08f,  0.65f,  0.00f  },
    {  0.00f,  1.00f,  0.00f  }, /* vertical spire */
    { -0.08f,  0.65f,  0.00f  },
    { -0.20f,  0.90f,  0.00f  },
    { -0.40f,  1.02f,  0.00f  },
    { -0.42f,  0.68f,  0.00f  },
    { -0.65f,  0.75f,  0.00f  },
    { -0.52f,  0.45f,  0.00f  },
    { -0.70f,  0.50f,  0.00f  },
    { -0.90f,  0.50f,  0.00f  },
    { -0.80f,  0.30f,  0.00f  },
    { -1.00f,  0.20f,  0.00f  }
  };

  glColor3f(1, 1, 1);
  glVertex3f(0, 0, 0);
 
  glBlendFunc(GL_ONE, GL_ONE);

  glBegin(GL_TRIANGLES);
  
  for (i=0; i < NUM_SPIRES; i++) {
    normcrossprod(vectors[i], zunit, right);
    normcrossprod(zunit, vectors[i], left);

    vmul(right, SPIRE_WIDTH);
    vmul(left, SPIRE_WIDTH);

    glColor4f(1,1,1,0.0);
    glVertex3fv(right);
    glVertex3f(radius * vectors[i][0], radius * vectors[i][1], 0.0);
    glVertex3fv(left);
  } 
  
  glEnd();
}
