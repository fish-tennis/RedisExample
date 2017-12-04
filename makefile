######################################
# RedisExample
######################################
DESTINATION := RedisExample
LIBS := 
INCLUDES := ./
LDFLAGS := -lm -ldl -lrt

RM := rm -f
#C,CC或CPP文件的后缀
PS=cpp
PSC=c
# GNU Make的隐含变量定义
CC=g++
CPPFLAGS = -g -Wall -O0 -DNDEBUG -std=c++11
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD

#以下部分无需修改
SOURCE := $(shell find . -name "*.cpp" -printf "%p ")
CSOURCE := $(shell find . -name "*.c" -printf "%p ")
OBJS := $(patsubst %.$(PS),%.o,$(SOURCE))
OBJS += $(patsubst %.$(PSC),%.o,$(CSOURCE))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.$(PS),$(MISSING_DEPS)))

.PHONY : all deps objs clean rebuild

all : $(DESTINATION)

deps : $(DEPS)
	$(CC) -MM -MMD $(SOURCE)
	$(CC) -MM -MMD $(CSOURCE)


objs : $(OBJS)

clean :
	find . -name "*.o" -type f -print -exec rm -rf {} \;
	find . -name "*.d" -type f -print -exec rm -rf {} \;
	@$(RM) $(DESTINATION)

rebuild: clean all 

ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
	@$(RM) $(patsubst %.d,%.o,$@)
endif

-include $(DEPS)

$(DESTINATION) : $(OBJS)
	$(CC) -o $(DESTINATION) $(OBJS) $(LDFLAGS)
	#mv $(DESTINATION) ./$(DESTINATION)
