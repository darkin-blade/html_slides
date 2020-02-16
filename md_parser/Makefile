SRCS    := $(shell find ./src/ -maxdepth 1 -name "*.cpp")
INC     := -I./include
CFLAGS  := -Wall -Werror

run:    
	g++ $(CFLAGS) $(INC) $(SRCS) -o md_parser
	./md_parser
