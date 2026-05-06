CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -I backend
LDFLAGS   := -pthread

TARGET    := backend/enigma_app

SRCS      := backend/main.cpp backend/Enigma.cpp backend/Rotor.cpp backend/Reflector.cpp backend/Crypto.cpp backend/Steckerbrett.cpp
OBJS      := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

backend/%.o: backend/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) Cipher_History.txt SecretMessage.txt

run: $(TARGET)
	@echo "Launching from backend/ to resolve ../data/ paths correctly..."
	cd backend && ./enigma_app

.PHONY: all clean run