static int
is_gpt_valid(int fd, uint64_t lba,
             gpt_header ** gpt, gpt_entry ** ptes)
{
	int rc = 0;		/* default to not valid */
	uint32_t crc, origcrc;

	if (!gpt || !ptes)
                return 0;
	if (!(*gpt = alloc_read_gpt_header(fd, lba)))
		return 0;

	/* Check the GUID Partition Table signature */
	if (__le64_to_cpu((*gpt)->signature) != GPT_HEADER_SIGNATURE) {
		/* 
		   printf("GUID Partition Table Header signature is wrong: %" PRIx64" != %" PRIx64 "\n",
		   __le64_to_cpu((*gpt)->signature), GUID_PT_HEADER_SIGNATURE);
		 */
		free(*gpt);
		*gpt = NULL;
		return rc;
	}

	/* Check the GUID Partition Table Header CRC */
	origcrc = __le32_to_cpu((*gpt)->header_crc32);
	(*gpt)->header_crc32 = 0;
	crc = efi_crc32(*gpt, __le32_to_cpu((*gpt)->header_size));
	if (crc != origcrc) {
		// printf( "GPTH CRC check failed, %x != %x.\n", origcrc, crc);
		(*gpt)->header_crc32 = __cpu_to_le32(origcrc);
		free(*gpt);
		*gpt = NULL;
		return 0;
	}
	(*gpt)->header_crc32 = __cpu_to_le32(origcrc);

	/* Check that the my_lba entry points to the LBA
	 * that contains the GPT we read */
	if (__le64_to_cpu((*gpt)->my_lba) != lba) {
		/*
		printf( "my_lba % PRIx64 "x != lba %"PRIx64 "x.\n",
		 		__le64_to_cpu((*gpt)->my_lba), lba);
		 */
		free(*gpt);
		*gpt = NULL;
		return 0;
	}

	/* Check that sizeof_partition_entry has the correct value */
	if (__le32_to_cpu((*gpt)->sizeof_partition_entry) != sizeof(gpt_entry)) {
		// printf("GUID partition entry size check failed.\n");
		free(*gpt);
		*gpt = NULL;
		return 0;
	}


	/* Check that sizeof_partition_entry has the correct value */
	if (__le32_to_cpu((*gpt)->sizeof_partition_entry) != sizeof(gpt_entry)) {
		// printf("GUID partition entry size check failed.\n");
		free(*gpt);
		*gpt = NULL;
		return 0;
	}


	if (!(*ptes = alloc_read_gpt_entries(fd, *gpt))) {
		free(*gpt);
		*gpt = NULL;
		return 0;
	}

	/* Check the GUID Partition Entry Array CRC */
	crc = efi_crc32(*ptes,
                        __le32_to_cpu((*gpt)->num_partition_entries) *
			__le32_to_cpu((*gpt)->sizeof_partition_entry));
	if (crc != __le32_to_cpu((*gpt)->partition_entry_array_crc32)) {
		// printf("GUID Partitition Entry Array CRC check failed.\n");
		free(*gpt);
		*gpt = NULL;
		free(*ptes);
		*ptes = NULL;
		return 0;
	}

	/* We're done, all's well */
	return 1;
}
