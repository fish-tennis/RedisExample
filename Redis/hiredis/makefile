######################################
# libFmScript
######################################
#目标（可执行文档）名称，库（譬如stdcx,iostr,mysql等），头文件路径
DESTINATION := libhiredis.a
LIBS := 
INCLUDES := ./
LDFLAGS :=-lm -ldl

AR = ar
RM := rm -f
#C,CC或CPP文件的后缀
PS=c
# GNU Make的隐含变量定义
CC=gcc
CFLAGS = -g -Wall -O3
CFLAGS += $(addprefix -I,$(INCLUDES))

#以下部分无需修改
SOURCE := $(shell find . -name "*.c" -printf "%p ")
OBJS := $(patsubst %.$(PS),%.o,$(SOURCE))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.$(PS),$(MISSING_DEPS)))

.PHONY : all deps objs clean rebuild

all : $(DESTINATION)

deps : $(DEPS)
	$(CC) -MM $(CFLAGS) $(SOURCE)


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
	$(AR) cqs $(DESTINATION) $(OBJS)
	#mv $(DESTINATION) ./../Lib/$(DESTINATION)
#结束