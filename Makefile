TARGETS_SUBDIRS := src
TARGETS := all clean

TEST_TARGETS := test
TEST_TARGETS_SUBDIRS := test

$(TARGETS): $(TARGETS_SUBDIRS)

$(TARGETS_SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(TEST_TARGETS): $(TEST_TARGETS_SUBDIRS)

$(TEST_TARGETS_SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TARGETS) $(TARGETS_SUBDIRS) $(TEST_TARGETS) $(TEST_TARGETS_SUBDIRS)
