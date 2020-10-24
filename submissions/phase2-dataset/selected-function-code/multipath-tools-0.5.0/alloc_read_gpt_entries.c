alloc_read_gpt_entries(int fd, gpt_header * gpt)
{
	gpt_entry *pte;
        size_t count = __le32_to_cpu(gpt->num_partition_entries) *
                __le32_to_cpu(gpt->sizeof_partition_entry);

        if (!count) return NULL;

	pte = (gpt_entry *)malloc(count);
	if (!pte)
		return NULL;
	memset(pte, 0, count);

	if (!read_lba(fd, __le64_to_cpu(gpt->partition_entry_lba), pte,
                      count)) {
		free(pte);
		return NULL;
	}
	return pte;
}
