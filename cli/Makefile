TARGET ?= bdgetchaper
CXX ?= g++
CXXFLAGS += -O2 -Wall -Wextra -Werror $(shell pkgconf --cflags libbluray)
LIBS += $(shell pkgconf --libs libbluray)

PREFIX ?= /usr/local

$(TARGET): main.o bdgetchap.o
	$(CXX) $(LIBS) -o $(TARGET) main.o bdgetchap.o

main.o: main.cpp ../common/bdgetchap.h
	$(CXX) $(CXXFLAGS) -c main.cpp

bdgetchap.o: ../common/bdgetchap.cpp
	$(CXX) $(CXXFLAGS) -c ../common/bdgetchap.cpp

install: $(TARGET)
	cp $(TARGET) $(PREFIX)/bin
	chmod +x $(PREFIX)/bin/$(TARGET)

uninstall:
	rm $(PREFIX)/bin/$(TARGET)

.phony clean:
	rm -f *.o

dist-clean: clean
	rm -f $(TARGET)