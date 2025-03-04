CXX = g++
CXXFLAGS = -c -std=c++17
LDFLAGS = -lssl -lcrypto  # Link OpenSSL libraries

SRC = test.cpp block.cpp chain.cpp \
      Transaction/Transaction.cpp \
      Transaction/BlockchainNode.cpp \
      Transaction/TxInput.cpp \
      Transaction/TxOutput.cpp \
      Transaction/utxo.cpp \
      Transaction/UTXOSet.cpp

OBJ = $(SRC:.cpp=.o)

test: $(OBJ)
	$(CXX) $(OBJ) -o test $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJ) test
