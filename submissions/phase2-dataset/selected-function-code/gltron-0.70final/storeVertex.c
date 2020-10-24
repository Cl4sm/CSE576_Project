void storeVertex(TrailMesh *pMesh, int offset, 
								 segment2 *s, float t, /* 0: start, 1: end */
								 float fFloor, float fTop, 
								 float fSegLength, float fTotalLength) {
	vec3 *pVertices = pMesh->pVertices + offset;
	vec3 *pNormals = pMesh->pNormals + offset;
	vec2 *pTexCoords = pMesh->pTexCoords + offset;
	vec3 v;
	vec2 uv;
	float fUStart;
	vec3 pvNormals[] = {
		{ { 1, 0, 0 } },
		{ { -1, 0, 0 } },
		{ { 0, 1, 0 } },
		{ { 0, -1, 0 } }
	};
	
	
	int iNormal;
	if(s->vDirection.v[0] == 0)
		// iNormal = (line->sx <= line->ex) ? 0 : 1;
		iNormal = 0;
	else
		// iNormal = (line->sy <= line->sy) ? 2 : 3;
		iNormal = 2;

	fUStart = (fTotalLength / DECAL_WIDTH) - floorf(fTotalLength / DECAL_WIDTH);
	
	v.v[0] = s->vStart.v[0] + t * s->vDirection.v[0];
	v.v[1] = s->vStart.v[1] + t * s->vDirection.v[1];
	v.v[2] = fFloor;
	uv.v[0] = fUStart + t * fSegLength / DECAL_WIDTH;

	uv.v[1] = 0;
	vec3Copy(pVertices, &v);
	vec3Copy(pNormals, pvNormals + iNormal);
	vec2Copy(pTexCoords, &uv);
			
	v.v[2] = fTop;
	uv.v[1] = 1;
	vec3Copy(pVertices + 1, &v);
	vec3Copy(pNormals + 1, pvNormals + iNormal);
	vec2Copy(pTexCoords + 1, &uv);
}
