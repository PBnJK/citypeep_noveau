# Makes the game!

CURRDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

all:
	$(MAKE) -C $(CURRDIR)src
	mv $(CURRDIR)src/citypeep.ps-exe $(CURRDIR)build
	mv $(CURRDIR)src/citypeep.map $(CURRDIR)build
	mv $(CURRDIR)src/citypeep.elf $(CURRDIR)build

clean:
	$(RM) $(CURRDIR)src/*.o
	$(RM) $(CURRDIR)src/*.dep

.PHONY: all
