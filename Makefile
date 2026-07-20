# SPDX-License-Identifier: GPL-2.0-only
CFLAGS ?= -Wall -O2
CPPFLAGS += -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -I.

all: readoc hsmp-msg tpmi-uncore
readoc: readoc.c version.h ; $(CC) $(CPPFLAGS) -std=gnu99 $(CFLAGS) $(LDFLAGS) $< -o $@
hsmp-msg: hsmp-msg.c ; $(CC) $(CPPFLAGS) -std=gnu99 $(CFLAGS) $(LDFLAGS) $< -o $@
tpmi-uncore: tpmi-uncore.c ; $(CC) $(CPPFLAGS) -std=gnu99 $(CFLAGS) $(LDFLAGS) $< -o $@
install: ; bash install.sh
clean: ; rm -f readoc hsmp-msg tpmi-uncore
.PHONY: all install clean
