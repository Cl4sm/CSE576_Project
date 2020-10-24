void SetMaterialColor(Mesh *pMesh, char *name, ColorType eType,
		      float pColor[4]) {
  int i;

  for(i = 0; i < pMesh->nMaterials; i++) {
    if(name == NULL || strstr(pMesh->pMaterials[i].name, name) ==
       pMesh->pMaterials[i].name) {
      switch(eType) {
      case eAmbient:
	memcpy( pMesh->pMaterials[i].ambient, pColor, sizeof(float[4]) );
	break;
      case eDiffuse:
	memcpy( pMesh->pMaterials[i].diffuse, pColor, sizeof(float[4]) );
	break;
      case eSpecular:
	memcpy( pMesh->pMaterials[i].specular, pColor, sizeof(float[4]) );
	break;
      }
    }
  }
}
