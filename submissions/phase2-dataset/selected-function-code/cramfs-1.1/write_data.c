static unsigned int write_data(struct entry *entry, char *base, unsigned int offset)
{
	do {
		if (entry->path || entry->uncompressed) {
			if (entry->same) {
				set_data_offset(entry, base, entry->same->offset);
				entry->offset = entry->same->offset;
			}
			else {
				set_data_offset(entry, base, offset);
				entry->offset = offset;
				map_entry(entry);
				offset = do_compress(base, offset, entry->name, entry->uncompressed, entry->size);
				unmap_entry(entry);
			}
		}
		else if (entry->child)
			offset = write_data(entry->child, base, offset);
		entry=entry->next;
	} while (entry);
	return offset;
}
