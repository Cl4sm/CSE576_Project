static ChunkArenaPtr
findArena()
{
    ChunkArenaPtr arena = NULL;
    int i;

    for(i = 0; i < numArenas; i++) {
        arena = &(chunkArenas[i]);
        if(arena->bitmap != 0)
            break;
        else
            arena = NULL;
    }

    assert(arena != NULL);

    if(!arena->chunks) {
        void *p;
        p = alloc_arena(CHUNK_SIZE * ARENA_CHUNKS);
        if(p == MAP_FAILED) {
            do_log_error(L_ERROR, errno, "Couldn't allocate chunk");
            maybe_free_chunks(1, 1);
            return NULL;
        }
        arena->chunks = p;
    }
    return arena;
}