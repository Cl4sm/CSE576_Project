Mesh* readMeshFromFile(const char *filename, MeshType iType) {
  // allocate some buffers
  // vertices, normals

  vec3 *pVertices = malloc( sizeof(vec3) * MAX_VERTICES );
  vec3 *pNormals = malloc( sizeof(vec3) * MAX_NORMALS );

  quadFace *pqFaces = NULL;
  face *pFaces = NULL;
  int iFaceSize = 0;

  Mesh *pMesh = malloc( sizeof(Mesh) );
  int iGroup = 0;
  int iVertex = 0, iNormal = 0, iFace = 0;

  gzFile f;
  char buf[BUF_SIZE];

  int i, j, k;


  switch(iType) {
  case TRI_MESH:
    pFaces = malloc( sizeof(face) * MAX_FACES );
    iFaceSize = 3;
    break;
  case QUAD_MESH:
    pqFaces = malloc( sizeof(quadFace) * MAX_FACES );
    iFaceSize = 4;
    break;
  default:
    fprintf(stderr, "[fatal]: illegal mesh type\n");
    exit(1);
  }

  if((f = gzopen(filename, "r")) == 0) {
    fprintf(stderr, "*** could not open file '%s'\n", filename);
    return NULL;
  }

  while( gzgets(f, buf, sizeof(buf)) ) {
    switch(buf[0]) {
    case 'm': // material library
      readMaterialLibrary(buf, pMesh);
      break;
    case 'u': // material  name
      setMaterial(buf, pMesh, &iGroup);
      break;
    case 'v': // vertex, normal, texture coordinate
      switch(buf[1]) {
      case ' ':
	readVector(buf + 1, pVertices + iVertex);
	iVertex++;
	break;
      case 'n': // normal
	readVector(buf + 2, pNormals + iNormal);
	iNormal++;
	break;
      case 't': // texture
	break; // ignore textures;
      }
      break;
    case 'f': // face (can produce multiple faces)
      switch(iType) {
      case TRI_MESH:
	readTriFace(buf, pFaces, &iFace, iGroup);
	break;
      case QUAD_MESH:
	readQuadFace(buf, pqFaces, &iFace, iGroup);
	break;
      }
      break;
    }
  }
  
  gzclose(f);

  // printf("vertices: %d, normals: %d, faces: %d\n", iVertex, iNormal, iFace);

  // count each material
  pMesh->pnFaces = malloc( sizeof(int) * pMesh->nMaterials );
  for(i = 0; i < pMesh->nMaterials; i++) {
    pMesh->pnFaces[ i ] = 0;
  }

  switch(iType) {
  case TRI_MESH:
    for(i = 0; i < iFace; i++) {
      pMesh->pnFaces[ pFaces[i].material ] += 1;
    }
    break;
  case QUAD_MESH:
    for(i = 0; i < iFace; i++) {
      pMesh->pnFaces[ pqFaces[i].material ] += 1;
    }
    break;
  }


  // combine vectors & normals for each vertex, doubling where necessary

  // initialize lookup[ vertex ][ normal ] table
  {
    int nVertices = 0;
    int **lookup = malloc( sizeof(int*) * iVertex );

    for(i = 0; i < iVertex; i++) {
      lookup[i] = malloc( sizeof(int) * iNormal );
      for(j = 0; j < iNormal; j++) {
	lookup[i][j] = -1;
      }
    }
  
    switch(iType) {
    case TRI_MESH:
      for(i = 0; i < iFace; i++) {
	for(j = 0; j < iFaceSize; j++) {
	  int vertex = pFaces[i].vertex[j] - 1;
	  int normal = pFaces[i].normal[j] - 1;
	  if( lookup[ vertex ][ normal ] == -1 ) {
	    lookup[ vertex ][ normal ] = nVertices;
	    nVertices++;
	  }
	}
      }
      break;
    case QUAD_MESH:
      for(i = 0; i < iFace; i++) {
	for(j = 0; j < iFaceSize; j++) {
	  int vertex = pqFaces[i].vertex[j] - 1;
	  int normal = pqFaces[i].normal[j] - 1;
	  if( lookup[ vertex ][ normal ] == -1 ) {
	    lookup[ vertex ][ normal ] = nVertices;
	    nVertices++;
	  }
	}
      }
      break;
    }
  
    // now that we know everything, build vertexarray based mesh
    // copy normals & vertices indexed by lookup-table
    pMesh->nVertices = nVertices;
    pMesh->pVertices = malloc( sizeof(GLfloat) * 3 * nVertices );
    pMesh->pNormals = malloc( sizeof(GLfloat) * 3 * nVertices );
    for(i = 0; i < iVertex; i++) {
      for(j = 0; j < iNormal; j++) {
	int vertex = lookup[ i ][ j ];
	if(vertex != -1 ) {
	  for(k = 0; k < 3; k++) {
	    *(pMesh->pVertices + 3 * vertex + k) = pVertices[ i ].v[k];
	    *(pMesh->pNormals + 3 * vertex + k) = pNormals[ j ].v[k];
	  }
	}
      }
    }

    // build indices (per Material)
    {
      int *face;
      face = malloc( sizeof(int) * pMesh->nMaterials );
      pMesh->ppIndices = malloc( sizeof(GLshort*) * pMesh->nMaterials );
      for(i = 0; i < pMesh->nMaterials; i++) {
	pMesh->ppIndices[i] = 
	  malloc( sizeof(GLshort) * iFaceSize * pMesh->pnFaces[i] );
	face[i] = 0;
      }
      switch(iType) {
      case TRI_MESH:
	for(i = 0; i < iFace; i++) {
	  int material = pFaces[i].material;
	  for(j = 0; j < iFaceSize; j++) {
	    int vertex = pFaces[i].vertex[j] - 1;
	    int normal = pFaces[i].normal[j] - 1;
	    pMesh->ppIndices[ material ][ iFaceSize * face[ material ] + j ] = 
	      lookup[ vertex ][ normal ];
	  }
	  face[ material ] = face[ material] + 1;
	}
	break;
      case QUAD_MESH:
	for(i = 0; i < iFace; i++) {
	  int material = pqFaces[i].material;
	  for(j = 0; j < iFaceSize; j++) {
	    int vertex = pqFaces[i].vertex[j] - 1;
	    int normal = pqFaces[i].normal[j] - 1;
	    pMesh->ppIndices[ material ][ iFaceSize * face[ material ] + j ] = 
	      lookup[ vertex ][ normal ];
	  }
	  face[ material ] = face[ material] + 1;
	}
	break;
      }
      free(face);
    }
    // printf("[scenegraph] vertices: %d, faces: %d\n", nVertices, iFace);

    free(lookup);
    free(pVertices);
    free(pNormals);
    switch(iType) {
    case TRI_MESH:
      free(pFaces);
      break;
    case QUAD_MESH:
      free(pqFaces);
      break;
    }
  }
  computeBBox(pMesh);

  return pMesh;
}
