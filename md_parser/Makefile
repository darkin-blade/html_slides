SRCS    := $(shell find ./src/ -maxdepth 1 -name "*.cpp")
DEPS    := $(shell find ./include/ -maxdepth 1 -name "*.h") $(SRCS)
CFLAGS  := -Wall -Werror

run:    $(DEPS)
	g++ $(CFLAGS) $(SRCS) -o md_parser
	./md_parser
