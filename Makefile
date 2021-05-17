SUBDIR = ./src ./example
MAKE = make

all: subdir

subdir:
	@for dir in $(SUBDIR); \
	do \
	$(MAKE) -C $$dir; \
	done

clean: 
	@for dir in $(SUBDIR); \
	do \
	$(MAKE) -C $$dir clean; \
	done

