TARGET_EXEC ?= skeleton.out

BUILD_DIR ?= Build
SRC_DIRS ?= Source

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
CXX := g++
CC := g++

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

########
#SDL options
SDL_CFLAGS := $(shell sdl2-config --cflags)
GLM_CFLAGS := -I../glm/
SDL_LDFLAGS := $(shell sdl2-config --libs)

CPPFLAGS ?= $(INC_FLAGS) -pipe -Wall -Wno-switch -g -g3 -ggdb -O3 -fopenmp -flto

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(SDL_LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -g $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ $(SDL_CFLAGS) $(GLM_CFLAGS)


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
