compare_gpts(gpt_header *pgpt, gpt_header *agpt, uint64_t lastlba)
{
	int error_found = 0;
	if (!pgpt || !agpt)
		return;
	if (__le64_to_cpu(pgpt->my_lba) != __le64_to_cpu(agpt->alternate_lba)) {
		error_found++;
		fprintf(stderr, 
		       "GPT:Primary header LBA != Alt. header alternate_lba\n");
#ifdef DEBUG
		fprintf(stderr,  "GPT:%" PRIx64 " != %" PRIx64 "\n",
		       __le64_to_cpu(pgpt->my_lba),
                       __le64_to_cpu(agpt->alternate_lba));
#endif
	}
	if (__le64_to_cpu(pgpt->alternate_lba) != __le64_to_cpu(agpt->my_lba)) {
		error_found++;
		fprintf(stderr, 
		       "GPT:Primary header alternate_lba != Alt. header my_lba\n");
#ifdef DEBUG
		fprintf(stderr,  "GPT:%" PRIx64 " != %" PRIx64 "\n",
		       __le64_to_cpu(pgpt->alternate_lba),
                       __le64_to_cpu(agpt->my_lba));
#endif
	}
	if (__le64_to_cpu(pgpt->first_usable_lba) !=
            __le64_to_cpu(agpt->first_usable_lba)) {
		error_found++;
		fprintf(stderr,  "GPT:first_usable_lbas don't match.\n");
#ifdef DEBUG
		fprintf(stderr,  "GPT:%" PRIx64 " != %" PRIx64 "\n",
		       __le64_to_cpu(pgpt->first_usable_lba),
                       __le64_to_cpu(agpt->first_usable_lba));
#endif
	}
	if (__le64_to_cpu(pgpt->last_usable_lba) !=
            __le64_to_cpu(agpt->last_usable_lba)) {
		error_found++;
		fprintf(stderr,  "GPT:last_usable_lbas don't match.\n");
#ifdef DEBUG
		fprintf(stderr,  "GPT:%" PRIx64 " != %" PRIx64 "\n",
		       __le64_to_cpu(pgpt->last_usable_lba),
                       __le64_to_cpu(agpt->last_usable_lba));
#endif
	}
	if (efi_guidcmp(pgpt->disk_guid, agpt->disk_guid)) {
		error_found++;
		fprintf(stderr,  "GPT:disk_guids don't match.\n");
	}
	if (__le32_to_cpu(pgpt->num_partition_entries) !=
            __le32_to_cpu(agpt->num_partition_entries)) {
		error_found++;
		fprintf(stderr,  "GPT:num_partition_entries don't match: "
		       "0x%x != 0x%x\n",
		       __le32_to_cpu(pgpt->num_partition_entries),
		       __le32_to_cpu(agpt->num_partition_entries));
	}
	if (__le32_to_cpu(pgpt->sizeof_partition_entry) !=
            __le32_to_cpu(agpt->sizeof_partition_entry)) {
		error_found++;
		fprintf(stderr, 
		       "GPT:sizeof_partition_entry values don't match: "
		       "0x%x != 0x%x\n",
                       __le32_to_cpu(pgpt->sizeof_partition_entry),
		       __le32_to_cpu(agpt->sizeof_partition_entry));
	}
	if (__le32_to_cpu(pgpt->partition_entry_array_crc32) !=
            __le32_to_cpu(agpt->partition_entry_array_crc32)) {
		error_found++;
		fprintf(stderr, 
		       "GPT:partition_entry_array_crc32 values don't match: "
		       "0x%x != 0x%x\n",
                       __le32_to_cpu(pgpt->partition_entry_array_crc32),
		       __le32_to_cpu(agpt->partition_entry_array_crc32));
	}
	if (__le64_to_cpu(pgpt->alternate_lba) != lastlba) {
		error_found++;
		fprintf(stderr, 
		       "GPT:Primary header thinks Alt. header is not at the end of the disk.\n");
#ifdef DEBUG
		fprintf(stderr,  "GPT:%" PRIx64 " != %" PRIx64 "\n",
		       __le64_to_cpu(pgpt->alternate_lba), lastlba);
#endif
	}

	if (__le64_to_cpu(agpt->my_lba) != lastlba) {
		error_found++;
		fprintf(stderr, 
		       "GPT:Alternate GPT header not at the end of the disk.\n");
#ifdef DEBUG
		fprintf(stderr,  "GPT:%" PRIx64 " != %" PRIx64 "\n",
		       __le64_to_cpu(agpt->my_lba), lastlba);
#endif
	}

	if (error_found)
		fprintf(stderr, 
		       "GPT: Use GNU Parted to correct GPT errors.\n");
	return;
}
