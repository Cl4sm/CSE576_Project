  float color[] = { 0, 0, 0, 1 };
	float white[] = { 1, 1, 1, 1 };

	switch(type) {
	case COLOR_TRAIL:
		if(gSettingsCache.alpha_trails)
			memcpy(color, pV->pColorAlpha, 4 * sizeof(float));
		else
			memcpy(color, pV->pColorAlpha, 3 * sizeof(float));
		break;
	case COLOR_BRIGHT:
		memcpy(color, white, 3 * sizeof(float));
		break;
	case COLOR_CYCLE:
		memcpy(color, pV->pColorDiffuse, 3 * sizeof(float));
		break;
	}
	float2ubyte(pMesh->pColors + 4 * offset, color);
	float2ubyte(pMesh->pColors + 4 * (offset + 1), color);
}
