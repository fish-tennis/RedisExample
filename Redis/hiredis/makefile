######################################
# libFmScript
######################################
#Ŀ�꣨��ִ���ĵ������ƣ��⣨Ʃ��stdcx,iostr,mysql�ȣ���ͷ�ļ�·��
DESTINATION := libhiredis.a
LIBS := 
INCLUDES := ./
LDFLAGS :=-lm -ldl

AR = ar
RM := rm -f
#C,CC��CPP�ļ��ĺ�׺
PS=c
# GNU Make��������������
CC=gcc
CFLAGS = -g -Wall -O3
CFLAGS += $(addprefix -I,$(INCLUDES))

#���²��������޸�
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
#����