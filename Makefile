CFLAGS ?= -Wall -O2 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64

all: readoc hsmp-msg
readoc: readoc.c version.h ; $(CC) $(CFLAGS) -I. $< -o $@
hsmp-msg: hsmp-msg.c ; $(CC) -Wall -O2 $< -o $@
install: ; bash install.sh
clean: ; rm -f readoc hsmp-msg
.PHONY: all install clean
