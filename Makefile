# Variables for easier maintenance
CC = cc
TARGET = copy-public-functions-into-header
INCLUDES = easy-args/includes/easyargs.h
TEST_SRC = ./test/source.c
TEST_HDR = ./test/source.h
TEST_OUT = ./test/actual_output.h
TEST_GOLDEN = ./test/expected_output.h

# Default target
build: $(TARGET)

$(TARGET): main.c $(INCLUDES)
	$(CC) main.c -o $(TARGET)

test: $(TARGET)
	@echo "Running tests..."
	@./$(TARGET) $(TEST_SRC) $(TEST_HDR) > $(TEST_OUT)
	@if diff $(TEST_GOLDEN) $(TEST_OUT) > /dev/null; then \
		echo "$(GREEN)Test Passed: Output matches $(TEST_GOLDEN)$(NC)"; \
		rm $(TEST_OUT); \
	else \
		echo "$(RED)Test Failed: Output differs from $(TEST_GOLDEN)$(NC)"; \
		diff -u $(TEST_GOLDEN) $(TEST_OUT); \
		exit 1; \
	fi

clean:
	rm -f $(TARGET) $(TEST_OUT)

.PHONY: build test clean