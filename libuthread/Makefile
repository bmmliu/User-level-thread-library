# Target library
lib	:= libuthread.a
objs	:= queue.o uthread.o context.o preempt.o
CC	:= gcc
CFLAGS	:= -Wall -Werror
CFLAGS	+= -g

ifneq ($(V),1)
Q = @
endif
all: $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

## TODO: Phase 1.1

$(lib): $(objs)
	@echo "AR $@"
	$(Q)ar rcs $@ $^
%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)
clean:
	@echo "clean"
	$(Q)rm -f $(lib) $(objs) $(deps)
