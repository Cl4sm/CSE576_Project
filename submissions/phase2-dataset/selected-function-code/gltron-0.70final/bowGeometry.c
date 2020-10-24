void bowGeometry(Player *pPlayer, PlayerVisual *pV,
								 TrailMesh *pMesh, int *pvOffset, int *piOffset) {
	Data *pData = pPlayer->data;
	segment2 s;
	int bdist = PLAYER_IS_ACTIVE(pPlayer) ? 2 : 3;
	int i;
	int vOffset = *pvOffset; int iOffset = *piOffset;

	s.vStart.v[0] = getSegmentEndX( pData, 0 );
	s.vStart.v[1] = getSegmentEndY( pData, 0 );
	s.vDirection.v[0] = getSegmentEndX( pData, bdist ) - s.vStart.v[0];
	s.vDirection.v[1] = getSegmentEndY( pData, bdist ) - s.vStart.v[1];

	for(i = 0; i < 10; i++) {
		float t = i * 1.0f / 10;
		float fTop = sqrtf(1 - t*t);
		float fFloor = (t < 0.6f) ? 0 : 0.5f * (t - 0.6f);
		if(fTop < 0.3f) fTop = 0.3f;
		
		storeVertex(pMesh, vOffset, &s, t, 
								fFloor * pPlayer->data->trail_height, 
								fTop * pPlayer->data->trail_height, 
								DECAL_WIDTH, 0);
		storeColor(pMesh, vOffset, pV, COLOR_BRIGHT);
		vOffset += 2;
		if(i) {
			storeIndices(pMesh, iOffset, vOffset - 4);
			iOffset += 6;
		}
	}
	storeVertex(pMesh, vOffset, &s, 1, 
							0.2f * pData->trail_height, 0.3f * pData->trail_height, 
							DECAL_WIDTH, 0);
	storeColor(pMesh, vOffset, pV, COLOR_CYCLE);
	vOffset += 2;
	storeIndices(pMesh, iOffset, vOffset - 4);

	pMesh->iUsed += iOffset - *piOffset;
	*piOffset = iOffset;
	*pvOffset = vOffset;
}
