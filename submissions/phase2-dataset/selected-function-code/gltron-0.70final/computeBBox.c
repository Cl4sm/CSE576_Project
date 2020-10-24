  int i, j;
  vec3 vMin, vMax, vSize;

  vcopy(pMesh->pVertices, vMin.v);
  vcopy(pMesh->pVertices, vMax.v);

  for(i = 0; i < pMesh->nVertices; i++) {
    for(j = 0; j < 3; j++) {
      if(vMin.v[j] > pMesh->pVertices[3 * i + j])
	vMin.v[j] = pMesh->pVertices[3 * i + j];
      if(vMax.v[j] < pMesh->pVertices[3 * i + j])
	vMax.v[j] = pMesh->pVertices[3 * i + j];
    }
    /*
    if(
       vMin.v[0] <= pMesh->pVertices[3 * i + 0] &&
       vMin.v[1] <= pMesh->pVertices[3 * i + 1] &&
       vMin.v[2] <= pMesh->pVertices[3 * i + 2]
       )
      vcopy(pMesh->pVertices + 3 * i, vMin.v);
    if(
       vMax.v[0] >= pMesh->pVertices[3 * i + 0] &&
       vMax.v[1] >= pMesh->pVertices[3 * i + 1] &&
       vMax.v[2] >= pMesh->pVertices[3 * i + 2]
       )
      vcopy(pMesh->pVertices + 3 * i, vMax.v);
    */
  }
  
  vsub(vMax.v, vMin.v, vSize.v);
  vcopy(vMin.v, pMesh->BBox.vMin.v);
  vcopy(vSize.v, pMesh->BBox.vSize.v);
  pMesh->BBox.fRadius=length(pMesh->BBox.vSize.v)/10;
}
