# ==================================================================================================
# Variables

# C++ compiler name
CXX = clang++

# Compiler flags
CFLAGS = -std=c++17 -fopenmp

# Debuguer flags
DFLAGS =

# Warning flags
WFLAGS = -Wall -Wextra
WFLAGS_EXTRA = -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization \
               -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs \
               -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
               -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef
# Linker flags
LFLAGS = -lstdc++fs -fopenmp -ljsoncpp

# Source files
SRC_DIR = src
SRC = $(shell find $(SRC_DIR) -type f -name '*.cpp')

# Dependency files
DEP_DIR = deps
DEP_FILES = $(SRC:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)

# Object files
BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Dummy warning files
WARNINGS = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.w)

# Executable name
EXEC = clustering

# ==================================================================================================
# Configuration

.PHONY: all release debug

# ==================================================================================================
# Main targets

all: release

debug: DFLAGS += -ggdb
debug: $(EXEC)

release: CFLAGS += -O3 -DNDEBUG
release: $(EXEC)

# ==================================================================================================
# Main rules

# Build executable from object files
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(DFLAGS) $(LFLAGS)

# Build object file from source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

# Create dependency files
$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -MM -MT '$(<:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)' $< -MF $@

# Include dependency files, without errors if they do not exist
NO_DEPS = clean clean-all warnings test
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NO_DEPS))))
	include $(DEP_FILES)
endif

# ==================================================================================================
# Warnings target, output all g++ warnings without compiling
warnings: CFLAGS += -fsyntax-only
warnings: WFLAGS += $(WFLAGS_EXTRA)
warnings: $(WARNINGS)

# Output warnings
$(BUILD_DIR)/%.w: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< $(CFLAGS) $(WFLAGS)

# ==================================================================================================
# Clean intermediate files (not final results like executables, documentation, packages,...)
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DEP_DIR)
	rm -rf logs maps
	rm -rf *~

# Clean everything
clean-all: clean
	rm -rf $(EXEC)
