void readMaterialLibraryFromFile(char *filename, Mesh *pMesh) {
  Material *pMaterials; 
  int iMaterial;

  char buf[BUF_SIZE];
  char name[BUF_SIZE];
  char *path;
  gzFile f;

  int i;

  path = getPath(PATH_DATA, filename);
  if(path == NULL) {
    fprintf(stderr, "** could not optain path to file '%s'\n", filename);
    return;
  }

  if((f = gzopen(path, "r")) == 0) {
    fprintf(stderr, "** could not open file '%s'\n", filename);
    free(path);
    return;
  }
  free(path);

  pMaterials = malloc( MAX_MATERIALS * sizeof( Material ) );
  iMaterial = -1;

  while( gzgets(f, buf, sizeof(buf)) ) {
    switch(buf[0]) {
    case '#': break;
    case 'n':
      iMaterial++;
      loadDefaultMaterial(pMaterials + iMaterial);

      sscanf(buf, "newmtl %s ", name);
      pMaterials[iMaterial].name = malloc( strlen(name) + 1 );
      strcpy( pMaterials[iMaterial].name, name );

      break;
    default:
      if(sscanf(buf, " Ka %f %f %f ", 
		pMaterials[iMaterial].ambient + 0,
		pMaterials[iMaterial].ambient + 1,
		pMaterials[iMaterial].ambient + 2) == 3) {
	// nothing
      } else if(sscanf(buf, " Kd %f %f %f ", 
		pMaterials[iMaterial].diffuse + 0,
		pMaterials[iMaterial].diffuse + 1,
		pMaterials[iMaterial].diffuse + 2) == 3) {
	// nothing
      } else if(sscanf(buf, " Ks %f %f %f ", 
		pMaterials[iMaterial].specular + 0,
		pMaterials[iMaterial].specular + 1,
		pMaterials[iMaterial].specular + 2) == 3) {
	// nothing
      } else if(sscanf(buf, " Ns %f ", 
		       & (pMaterials[iMaterial].shininess) ) == 1 ) {
	// nothing
      } else {
	/* FIXME: "disabled error message" */
	/* fprintf(stderr, "+++ unparsed material property: %s", buf); */
      }
    }
  }
	
  pMesh->nMaterials = iMaterial + 1;
  pMesh->pMaterials = malloc( sizeof(Material) *  pMesh->nMaterials );
  for(i = 0; i < pMesh->nMaterials; i++) {
    memcpy( pMesh->pMaterials + i, pMaterials + i, sizeof(Material) );
  }

  free(pMaterials);
}
