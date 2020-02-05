# Target library
lib       := libuthread.a
objs      := queue.o
CC        := gcc
CFLAGS    := -Wall - Wextra -Werror
CFLAGS    += -g

ifneq ($(V),1)
Q = @
endif

all: $(lib)

# Dep tracking *must* be below the 'all' rule
deps := $(patsubst %.o,%.d,$(objs))
-include$(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

## TODO: Phase 1.1
$(lib): $(objs)
	@echo "AR $@"
	$(Q) ar rcs $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
	@echo "clean"
	$(Q)rm -f $(lib) $(objs) $(deps)

