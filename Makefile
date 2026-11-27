CXX      = clang++
CXXFLAGS = -std=c++20 -O2 -Wall -I.
LDFLAGS  = -framework CoreFoundation -framework CoreAudio -framework AudioToolbox
LDLIBS = -lncurses

SRC = main.cpp miniaudio.cpp deck.cpp playback.cpp tui.cpp
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)


networkDJ: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

run:
	networkDJ
	./networkDJ

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f networkDJ $(OBJ) $(DEP)

-include $(DEP)

.PHONY: run clean