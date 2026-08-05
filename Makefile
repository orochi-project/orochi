.PHONY: all clean run

all clean:
	$(MAKE) -C src $@

run:
	$(MAKE) -C src run EXT=gbc
