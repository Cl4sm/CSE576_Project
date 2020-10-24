void
dispose_chunk(void *chunk)
{
    ChunkArenaPtr arena = NULL;
    unsigned i;

    assert(chunk != NULL);

    if(currentArena && CHUNK_IN_ARENA(chunk, currentArena)) {
        arena = currentArena;
    } else {
        for(i = 0; i < numArenas; i++) {
            arena = &(chunkArenas[i]);
            if(CHUNK_IN_ARENA(chunk, arena))
                break;
        }
        assert(arena != NULL);
        currentArena = arena;
    }

    i = CHUNK_ARENA_INDEX(chunk, arena);
    arena->bitmap |= BITMAP_BIT(i);
    used_chunks--;
}