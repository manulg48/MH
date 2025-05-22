BUILD_DIR = build
EXECUTABLE = main

all: $(BUILD_DIR)
	@cmake -DCMAKE_BUILD_TYPE=Debug -S . -B $(BUILD_DIR)
	@make -C $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	@./$(BUILD_DIR)/$(EXECUTABLE)

clean:
	@rm -rf $(BUILD_DIR)