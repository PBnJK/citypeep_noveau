# Makes the game!

CURRDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

LOGPATH := build/INFO.TXT

all:
	@echo "build 1997.$(shell date +'%m.%d %H:%M:%S')" > $(LOGPATH)
	$(MAKE) -C $(CURRDIR)src
	mv $(CURRDIR)src/citypeep.ps-exe $(CURRDIR)build/bin
	mv $(CURRDIR)src/citypeep.map $(CURRDIR)build/bin
	mv $(CURRDIR)src/citypeep.elf $(CURRDIR)build/bin
	mkpsxiso -y -o build/iso/citypeep.bin -c build/iso/citypeep.cue build/iso.xml

clean:
	$(RM) $(CURRDIR)src/*.o
	$(RM) $(CURRDIR)src/*.dep

.PHONY: all
