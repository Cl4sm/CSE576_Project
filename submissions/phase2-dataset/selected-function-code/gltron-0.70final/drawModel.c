  int i;
  int iFaceSize = 0;
  GLenum primitive = GL_TRIANGLES;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, pMesh->pVertices);
  glNormalPointer(GL_FLOAT, 0, pMesh->pNormals);

  switch(iType) {
  case TRI_MESH:
    primitive = GL_TRIANGLES;
    iFaceSize = 3;
    break;
  case QUAD_MESH:
    primitive = GL_QUADS;
    iFaceSize = 4;
    break;
  default:
    fprintf(stderr, "[fatal]: illegal mesh type\n");
    exit(1);
  }

  for(i = 0; i < pMesh->nMaterials; i++) {
    glMaterialfv(GL_FRONT, GL_AMBIENT,
		 pMesh->pMaterials[i].ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,
		 pMesh->pMaterials[i].diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,
		 pMesh->pMaterials[i].specular);
    glMaterialf(GL_FRONT, GL_SHININESS,
		pMesh->pMaterials[i].shininess);

    glDrawElements(primitive, iFaceSize * pMesh->pnFaces[i],
		   GL_UNSIGNED_SHORT, pMesh->ppIndices[i]);

    polycount += pMesh->pnFaces[i];
  }

}
