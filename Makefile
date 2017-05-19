TARGETS = CSV2CStruct \
		  ANSYSResultsMerger
          
all: $(TARGETS)

install: $(TARGETS)
	@for TARGET in $(TARGETS); do (cp $$TARGET ~/bin; chmod a+x ~/bin/$$TARGET); done

%: %.cc
	clang++ -stdlib=libc++ -std=c++11 -o $@ $<

clean:
	rm -f $(TARGETS)
