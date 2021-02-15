
CXXFLAGS:=-std=c++17 $(CXXEXTRA)

all: hana_string_repro

BOOST_INCDIR=/usr/local/include
BOOST_LIBDIR=/usr/local/lib

hana_string_repro.o: hana_string_repro.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^ -I$(BOOST_INCDIR) 

hana_string_repro: hana_string_repro.o
	$(CXX) $(LDFLAGS) -o $@ $<

test: hana_string_repro
	./hana_string_repro

clean:
	rm -f hana_string_repro hana_string_repro.o
