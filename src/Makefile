CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic

all: yugioh

yugioh: main.o menu.o Card.o Deck.o Game.o HumanPlayer.o List.o MonsterZone.o Overhead.o PcPlayer.o Player.o
	$(LD) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o yugioh

deps:
	$(CXX) -MM *cpp > Makefile.d 
