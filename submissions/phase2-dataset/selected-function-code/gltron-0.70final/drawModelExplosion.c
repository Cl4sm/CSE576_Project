void drawModelExplosion(Mesh *pMesh, float fRadius) {
  int i, j, k;
#define EXP_VECTORS 10
  float vectors[][3] = {
    { 0.03f, -0.06f, -0.07f }, 
    { 0.04f, 0.08f, -0.03f }, 
    { 0.10f, -0.04f, -0.07f }, 
    { 0.06f, -0.09f, -0.10f }, 
    { -0.03f, -0.05f, 0.02f }, 
    { 0.07f, 0.08f, -0.00f }, 
    { 0.01f, -0.04f, 0.10f }, 
    { -0.01f, -0.07f, 0.09f }, 
    { 0.01f, -0.01f, -0.09f }, 
    { -0.04f, 0.04f, 0.02f }
  };

  for(i = 0; i < pMesh->nMaterials; i++) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
        pMesh->pMaterials[i].ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
        pMesh->pMaterials[i].diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,
        pMesh->pMaterials[i].specular);
    glMaterialf(GL_FRONT, GL_SHININESS,
        pMesh->pMaterials[i].shininess);

    for(j = 0; j < pMesh->pnFaces[i]; j++) {

      float *normal, *vertex;

      normal = pMesh->pNormals + 3 * pMesh->ppIndices[i][3 * j];

      glPushMatrix();
      glTranslatef(fRadius * (*(normal + 0) + vectors[j % EXP_VECTORS][0]),
          fRadius * (*(normal + 1) + vectors[j % EXP_VECTORS][1]),
          fabsf(fRadius * (*(normal + 2) + vectors[j % EXP_VECTORS][2]) ));
      glBegin(GL_TRIANGLES);
      for(k = 0; k < 3; k++) {
        normal = pMesh->pNormals + 3 * pMesh->ppIndices[i][3 * j + k];
        vertex = pMesh->pVertices + 3 * pMesh->ppIndices[i][3 * j + k];

        glNormal3fv(normal);
        glVertex3fv(vertex);
      }
      glEnd();
      glPopMatrix();
    }
    polycount += pMesh->pnFaces[i];
  }
}
