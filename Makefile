.PHONY: all

$(EXEC_NAME):
	cmake --preset $(EXEC_NAME)
	cmake --build build/$(EXEC_NAME) --clean-first
