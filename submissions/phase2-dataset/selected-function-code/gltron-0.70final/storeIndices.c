void storeIndices(TrailMesh *pMesh, int indexOffset, int vertexOffset) {
	unsigned short ppBase[2][6] = {
		{ 0, 2, 1, 2, 3, 1 },
		{ 0, 1, 2, 1, 3, 2 }
	};
	int i;
	int winding;

	if(pMesh->pVertices[vertexOffset].v[0] ==
		 pMesh->pVertices[vertexOffset + 2].v[0])
		winding = (pMesh->pVertices[vertexOffset].v[1] <= 
							 pMesh->pVertices[vertexOffset + 2].v[1]) ? 0 : 1;
	else
		winding = (pMesh->pVertices[vertexOffset].v[0] < 
							 pMesh->pVertices[vertexOffset + 2].v[0]) ? 1 : 0;
	
	for(i = 0; i < 6; i++) {
		pMesh->pIndices[i + indexOffset] = ppBase[winding][i] + vertexOffset;
	}
}
