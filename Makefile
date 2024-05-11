SUBDIR = src

all: $(SUBDIR)

$(SUBDIR):
	$(MAKE) -C $@

.PHONY: all $(SUBDIR)