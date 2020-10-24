void *
maybe_get_chunk()
{
    unsigned i;
    ChunkArenaPtr arena = NULL;

    if(currentArena && currentArena->bitmap != 0) {
        arena = currentArena;
    } else {
        if(used_chunks >= CHUNKS(chunkHighMark))
            return NULL;

        arena = findArena();
        if(!arena)
            return NULL;
        currentArena = arena;
    }
    i = BITMAP_FFS(arena->bitmap) - 1;
    arena->bitmap &= ~BITMAP_BIT(i);
    used_chunks++;
    return arena->chunks + CHUNK_SIZE * i;
}