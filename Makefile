include Config.mk
vpath %.h $(INCLUDES_DIR)
vpath %.c $(SOURCES_DIR)


ALL_OBJS := $(patsubst $(BUILD_DIR)/%.o,$(SOURCES_DIR)/%.c,$(SOURCES))


.PHONY: all clean test run

all: $(PROGRAM)

$(PROGRAM): $(ALL_OBJS)
	$(CC) $(BASE_CFLAGS) $(EXTRA_FLAGS) $^ -o $@

%.o: %.c

clean:
	@rm -rf $(BUILD_DIR)/*.o $(PROGRAM)

test:
	@echo "Tests not implemented."

run:
	@$(PROGRAM)
