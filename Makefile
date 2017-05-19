TARGETS = CSV2CStruct \
		  ANSYSResultsMerger
          
all: $(TARGETS)

%: %.cc
	clang++ -stdlib=libc++ -std=c++11 -o $@ $<

install:
	@for TARGET in $(TARGETS); do (cp $$TARGET ~/bin; chmod a+x ~/bin/$$TARGET); done

clean:
	rm -f $(TARGETS)
