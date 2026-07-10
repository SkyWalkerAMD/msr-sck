// SPDX-License-Identifier: GPL-2.0-only
/*
 * readoc.c - MSR reader for sckoc (socket/overclock monitor)
 *
 * Reads a Model-Specific Register from /dev/cpu/N/msr.
 *
 * Part of sckoc. Original implementation, GPL-2.0-only.
 * Copyright (C) 2026 SkyWalkerAMD
 *
 * Reads one 64-bit MSR at the given register number on a chosen CPU,
 * optionally extracting a [high:low] bitfield, and prints it as
 * unsigned decimal (default) or hexadecimal.
 *
 * This is a deliberately small, read-only helper: it never writes MSRs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <inttypes.h>

#include "version.h"

enum out_fmt { FMT_UDEC, FMT_HEX, FMT_HEX_UPPER };

static void usage(const char *prog)
{
	fprintf(stderr,
		"Usage: %s [-p cpu] [-f high:low] [-u|-x|-X] regno\n"
		"  -p cpu        CPU number to read from (default 0)\n"
		"  -f high:low   extract bitfield [high:low] only\n"
		"  -u            unsigned decimal output (default)\n"
		"  -x            hexadecimal output (lower case)\n"
		"  -X            hexadecimal output (upper case)\n"
		"  -V            print version\n"
		"  -h            print this help\n",
		prog);
}

int main(int argc, char *argv[])
{
	int cpu = 0;
	unsigned hi = 63, lo = 0;
	enum out_fmt fmt = FMT_UDEC;
	int c;

	while ((c = getopt(argc, argv, "p:f:uxXVh")) != -1) {
		switch (c) {
		case 'p': {
			char *end;
			long v = strtol(optarg, &end, 0);
			if (*end || v < 0 || v > 8191) {
				usage(argv[0]);
				return 127;
			}
			cpu = (int)v;
			break;
		}
		case 'f':
			if (sscanf(optarg, "%u:%u", &hi, &lo) != 2 ||
			    hi > 63 || lo > hi) {
				usage(argv[0]);
				return 127;
			}
			break;
		case 'u': fmt = FMT_UDEC;      break;
		case 'x': fmt = FMT_HEX;       break;
		case 'X': fmt = FMT_HEX_UPPER; break;
		case 'V':
			fprintf(stderr, "readoc %s\n", VERSION_STRING);
			return 0;
		case 'h':
			usage(argv[0]);
			return 0;
		default:
			usage(argv[0]);
			return 127;
		}
	}

	if (optind != argc - 1) {
		usage(argv[0]);
		return 127;
	}

	uint32_t reg = (uint32_t)strtoul(argv[optind], NULL, 0);

	char path[64];
	snprintf(path, sizeof(path), "/dev/cpu/%d/msr", cpu);

	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "readoc: cannot open %s: %s\n",
			path, strerror(errno));
		return 2;
	}

	uint64_t val;
	if (pread(fd, &val, sizeof(val), reg) != (ssize_t)sizeof(val)) {
		fprintf(stderr, "readoc: cannot read MSR 0x%" PRIx32
			" on cpu %d: %s\n", reg, cpu, strerror(errno));
		close(fd);
		return 4;
	}
	close(fd);

	/* extract bitfield if narrower than the full 64 bits */
	unsigned width = hi - lo + 1;
	if (width < 64) {
		val >>= lo;
		val &= (UINT64_C(1) << width) - 1;
	}

	switch (fmt) {
	case FMT_HEX:       printf("%" PRIx64 "\n", val); break;
	case FMT_HEX_UPPER: printf("%" PRIX64 "\n", val); break;
	case FMT_UDEC:
	default:            printf("%" PRIu64 "\n", val); break;
	}

	return 0;
}
