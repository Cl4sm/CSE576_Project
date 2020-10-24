void
initChunks(void)
{
    int i;
    used_chunks = 0;
    initChunksCommon();
    pagesize = getpagesize();
    if((CHUNK_SIZE * ARENA_CHUNKS) % pagesize != 0) {
        do_log(L_ERROR,
               "The arena size %d (%d x %d) "
               "is not a multiple of the page size %d.\n",
                ARENA_CHUNKS * CHUNK_SIZE, ARENA_CHUNKS, CHUNK_SIZE, pagesize);
        abort();
    }
    numArenas = 
        (CHUNKS(chunkHighMark) + (ARENA_CHUNKS - 1)) / ARENA_CHUNKS;
    chunkArenas = malloc(numArenas * sizeof(ChunkArenaRec));
    if(chunkArenas == NULL) {
        do_log(L_ERROR, "Couldn't allocate chunk arenas.\n");
        exit (1);
    }
    for(i = 0; i < numArenas; i++) {
        chunkArenas[i].bitmap = EMPTY_BITMAP;
        chunkArenas[i].chunks = NULL;
    }
    currentArena = NULL;
}