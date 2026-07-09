CFLAGS ?= -Wall -O2
CPPFLAGS += -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -I.

all: readoc hsmp-msg
readoc: readoc.c version.h ; $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@
hsmp-msg: hsmp-msg.c ; $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@
install: ; bash install.sh
clean: ; rm -f readoc hsmp-msg
.PHONY: all install clean
