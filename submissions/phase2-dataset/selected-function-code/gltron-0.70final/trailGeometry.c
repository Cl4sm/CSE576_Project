void trailGeometry(Player *pPlayer, PlayerVisual* pV,
									 TrailMesh *pMesh,
									 int *pvOffset, int *piOffset) {
	Data *pData = pPlayer->data;
	int curVertex = *pvOffset, curIndex = *piOffset;
	int i;
	float fTotalLength = 0;
	float fSegLength;
	for(i = 0; i < pData->trailOffset; i++) {
		fSegLength = segment2_Length(pData->trails + i);
		if(i == 0 || cmpdir(pData->trails + i - 1, pData->trails + i)) {
			storeVertex(pMesh, curVertex, pData->trails + i, 0, 
									0, pData->trail_height, 
									fSegLength, fTotalLength);
			storeColor(pMesh, curVertex, pV, COLOR_TRAIL);
			curVertex += 2;
		}
			
		storeVertex(pMesh, curVertex, pData->trails + i, 1,
								0, pData->trail_height,
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pV, COLOR_TRAIL);
		curVertex += 2;

		storeIndices(pMesh, curIndex, curVertex - 4);
		curIndex += 6;

		fTotalLength += fSegLength;

	}
	{
		segment2 s;
		vec2Copy(&s.vStart, & pData->trails[pData->trailOffset].vStart);
		s.vDirection.v[0] = getSegmentEndX( pData, 1 ) - s.vStart.v[0];
		s.vDirection.v[1] = getSegmentEndY( pData, 1 ) - s.vStart.v[1];

		fSegLength = segment2_Length(&s);
		
		storeVertex(pMesh, curVertex, &s, 0,
								0, pData->trail_height, 
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pV, COLOR_TRAIL);
		curVertex += 2;
		
		storeVertex(pMesh, curVertex, &s, 1,
								0, pData->trail_height, 
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pV, COLOR_TRAIL);
		curVertex += 2;

		storeIndices(pMesh, curIndex, curVertex - 4);
		curIndex += 6;

		fTotalLength += fSegLength;

		vec2Add(&s.vStart, &s.vStart, &s.vDirection);
		s.vDirection.v[0] = getSegmentEndX( pData, 0 ) - s.vStart.v[0];
		s.vDirection.v[1] = getSegmentEndY( pData, 0 ) - s.vStart.v[1];
		fSegLength = segment2_Length(&s);

		storeVertex(pMesh, curVertex, &s, 0,
								0, pData->trail_height, 
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pV, COLOR_TRAIL);
		curVertex += 2;

		storeVertex(pMesh, curVertex, &s, 1,
								0, pData->trail_height, 
								fSegLength, fTotalLength);
		storeColor(pMesh, curVertex, pV, COLOR_BRIGHT);
		curVertex += 2;

		storeIndices(pMesh, curIndex, curVertex - 4);
		curIndex += 6;
	}

	pMesh->iUsed += curIndex - *piOffset;
	*piOffset = curIndex;
	*pvOffset = curVertex;
}
