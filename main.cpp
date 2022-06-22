#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

// Transaction Data
struct TransactionData {
    double amount;
    string senderKey;
    string receiverKey;
    time_t timeStamp;
};

// Block Class
class Block {
private:
    int index;
    size_t blockHash;
    size_t previousHash;
    size_t generateHash();

public:
    // Constructor
    Block(int idx, TransactionData d, size_t prevHash);

    // Get Original Hash
    size_t getHash();

    // Get Previous Hash
    size_t getPreviousHash();

    // Transaction Data
    TransactionData data;

    // Validate Hash
    bool isHashValid();
};

// Constructor with params
Block::Block(int idx, TransactionData d, size_t prevHash) {
    index = idx;
    data = d;
    previousHash = prevHash;
    blockHash = generateHash();
}

// generate functions
size_t Block::generateHash() {
    hash<string> hash1;
    hash<size_t> hash2;
    hash<size_t> finalHash;
    string toHash = to_string(data.amount) + data.receiverKey + data.senderKey + to_string(data.timeStamp);

    return finalHash(hash1(toHash) + hash2(previousHash));
}

// Public Functions
size_t Block::getHash() {
    return blockHash;
}

size_t Block::getPreviousHash() {
    return previousHash;
}

bool Block::isHashValid() {
    return generateHash() == blockHash;
}

// Blockchaim Class
class BlockChain {
private:
    Block createGenesisBlock();

public:
    // Public Chain
    vector<Block> chain;

    // Constructor
    BlockChain();

    // Public Functions
    void addBlock(TransactionData data);
    bool isChainValid();

    // Contrived Example For Demo Only!
    Block *getLatestBlock();

    // Display Blocks
    void displayBlocks();
};

// Blockchain constructor
BlockChain::BlockChain() {
    Block genesis = createGenesisBlock();
    chain.push_back(genesis);
}

Block BlockChain::createGenesisBlock() {
    time_t current;
    TransactionData d;
    d.amount = 0;
    d.receiverKey = "None";
    d.senderKey = "None" ;
    d.timeStamp = time(&current);

    hash<int> hash1;
    Block genesis(0, d, hash1(0));
    return genesis;
}

// Bad! Only for Demo!
Block *BlockChain::getLatestBlock() {
    return &chain.back();
}

void BlockChain::addBlock(TransactionData d) {
    int index = (int)chain.size() - 1;
    Block newBlock(index, d, getLatestBlock()->getHash());
    chain.push_back(newBlock);
}

bool BlockChain::isChainValid() {
    vector<Block>::iterator it;
    int chainLen = (int)chain.size();

    for (it = chain.begin(); it != chain.end(); ++it) {
        Block currentBlock = *it;
        if (!currentBlock.isHashValid()) {
            // INVALID!
            return false;
        }

        if (chainLen > 1) {
            Block previousBlock  = *(it - 1);
            if (currentBlock.getPreviousHash() != previousBlock.getHash()) {
                // INVALID!
                return false;
            }
        }
    }

    return true;
}

void BlockChain::displayBlocks() {
    vector<Block>::iterator it;
    for (it = chain.begin(); it != chain.end(); ++it) {
        cout << it->getPreviousHash() << " ";
        cout << it->getHash() << " ";

        cout << to_string(it->data.amount) << " "; // Crashes with to_string.
        cout << it->data.senderKey << " ";
        cout << it->data.receiverKey << " ";
        cout << it->data.timeStamp << endl;
    }
}

int main()
{
    // Start Blockchain
    BlockChain AwesomeCoin;

    // Data for first added block
    TransactionData data1;
    time_t data1Time;
    data1.amount = 1.5;
    data1.receiverKey = "Joe";
    data1.senderKey = "Sally";
    data1.timeStamp = time(&data1Time);

    AwesomeCoin.addBlock(data1);

    cout << "Is chain valid" << endl << AwesomeCoin.isChainValid() << endl;
    AwesomeCoin.displayBlocks();

    TransactionData data2;
    time_t data2Time;
    data2.amount = 0.00013322;
    data2.receiverKey = "Martha";
    data2.senderKey = "Fred";
    data2.timeStamp = time(&data2Time);

    AwesomeCoin.addBlock(data2);

    cout << "Now is chain valid" << endl << AwesomeCoin.isChainValid() << endl;
    AwesomeCoin.displayBlocks();

    // Someone's getting sneaky
    Block *hackBlock = AwesomeCoin.getLatestBlock();
    hackBlock->data.amount = 10000;
    hackBlock->data.receiverKey = "Jon - from Crypto Enthusiasts";

    cout << "Now is chain valid" << endl << AwesomeCoin.isChainValid() << endl;
    AwesomeCoin.displayBlocks();

    return 0;
}
