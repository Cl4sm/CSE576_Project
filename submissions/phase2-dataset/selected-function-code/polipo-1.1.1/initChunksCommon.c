static void
initChunksCommon()
{
#define ROUND_CHUNKS(a) a = (((unsigned long)(a) + CHUNK_SIZE - 1) / CHUNK_SIZE) * CHUNK_SIZE;
    int q;

    if(CHUNK_SIZE != 1 << log2_ceil(CHUNK_SIZE)) {
        do_log(L_ERROR, "CHUNK SIZE %d is not a power of two.\n", CHUNK_SIZE);
        exit(1);
    }

    ROUND_CHUNKS(chunkHighMark);
    ROUND_CHUNKS(chunkCriticalMark);
    ROUND_CHUNKS(chunkLowMark);

    if(chunkHighMark < 8 * CHUNK_SIZE) {
        int mem = physicalMemory();
        if(mem > 0)
            chunkHighMark = mem / 4;
        else
            chunkHighMark = 24 * MB;
        chunkHighMark = MIN(chunkHighMark, 24 * MB);
        chunkHighMark = MAX(chunkHighMark, 8 * CHUNK_SIZE);
    }

    if(chunkHighMark < MB / 2)
        fprintf(stderr,
                "Warning: little chunk memory (%d bytes)\n", chunkHighMark);

    q = 0;
    if(chunkLowMark <= 0) q = 1;
    if(chunkLowMark < 4 * CHUNK_SIZE ||
       chunkLowMark > chunkHighMark - 4 * CHUNK_SIZE) {
        chunkLowMark = MIN(chunkHighMark - 4 * CHUNK_SIZE,
                           chunkHighMark * 3 / 4);
        ROUND_CHUNKS(chunkLowMark);
        if(!q) do_log(L_WARN, "Inconsistent chunkLowMark -- setting to %d.\n",
                      chunkLowMark);
    }

    q = 0;
    if(chunkCriticalMark <= 0) q = 1;
    if(chunkCriticalMark >= chunkHighMark - 2 * CHUNK_SIZE ||
       chunkCriticalMark <= chunkLowMark + 2 * CHUNK_SIZE) {
        chunkCriticalMark =
            MIN(chunkHighMark - 2 * CHUNK_SIZE,
                chunkLowMark + (chunkHighMark - chunkLowMark) * 15 / 16);
        ROUND_CHUNKS(chunkCriticalMark);
        if(!q) do_log(L_WARN, "Inconsistent chunkCriticalMark -- "
                      "setting to %d.\n", chunkCriticalMark);
    }
#undef ROUND_CHUNKS
}