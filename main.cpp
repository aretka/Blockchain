#include <iostream>
#include <cstdint>
#include <sstream>
#include <cstdint>
#include <vector>
#include <ctime>
#include <cmath>

using namespace std;

void decimalToHex (int hash[], string &stringHash) {
    std::stringstream ss;
    string res;
    for(int i = 0; i < 4; i++) {
        ss << std::hex << hash[i];
        res = (ss.str());
        stringHash+=res;
        if ( res.length() < 8)
            for (int i=0; i < 8 - res.length(); i++)
                stringHash+=res[0];
        ss.str("");
    }
}

// hash function which creates 4 different 8hex hashes
string hashFunction (string input) {
    int sum = 0, multiplySum=1, sumOfBoth=0, asciiValue, hash[4] ={1, 2, 3, 4};
    int primeNumbers[16] = {2, 17, 41, 59, 7, 31, 67, 19, 83, 11, 47, 71, 5, 23, 53, 41};
    // creating some numbers by sqrt prime numbers
    double sqrtNum;
    int sqrtNumArray[16];
    for (int i=0; i<16; i++) {
        sqrtNum = sqrt(primeNumbers[i])*1000000;
        sqrtNumArray[i] = sqrtNum;
    }
    bool ifNull = false;
    if ( input == "")
        ifNull = true;
    if(ifNull) {
        sum += 128;
        multiplySum *= 128;
        sumOfBoth = sumOfBoth + sum + multiplySum;

        hash[0] = hash[0] ^ 128;
        hash[1] = hash[1] ^ sum;
        hash[2] = hash[2] ^ multiplySum;
        hash[3] = hash[3] ^ sumOfBoth;

        hash[0] = hash[0] * sqrtNumArray[0];
        hash[1] = hash[1] * sqrtNumArray[sum%4+4];
        hash[2] = hash[2] * sqrtNumArray[multiplySum%4+8];
        hash[3] = hash[3] * sqrtNumArray[sumOfBoth%4+12];

        for(int ii = 0; ii < 4; ii++) {
            hash[0] = hash[0] ^ sqrtNumArray[ii];
            hash[1] = hash[1] ^ sqrtNumArray[ii+4];
            hash[2] = hash[2] ^ sqrtNumArray[ii+8];
            hash[3] = hash[3] ^ sqrtNumArray[ii+12];
        }
    }
    else {
         for (int i = 0; i < input.length(); i++) {
            sum += (int)input[i];
            multiplySum *= ((int)input[i]);
            sumOfBoth = sumOfBoth + sum + multiplySum;

            hash[0] = hash[0] ^ (input[i]);
            hash[1] = hash[1] ^ sum;
            hash[2] = hash[2] ^ multiplySum;
            hash[3] = hash[3] ^ sumOfBoth;

            hash[0] = hash[0] * sqrtNumArray[((int)input[i])%4];
            hash[1] = hash[1] * sqrtNumArray[sum%4+4];
            hash[2] = hash[2] * sqrtNumArray[multiplySum%4+8];
            hash[3] = hash[3] * sqrtNumArray[sumOfBoth%4+12];

            for(int ii = 0; ii < (int)input[i]%4+1; ii++) {
                hash[0] = hash[0] ^ sqrtNumArray[ii];
                hash[1] = hash[1] ^ sqrtNumArray[ii+4];
                hash[2] = hash[2] ^ sqrtNumArray[ii+8];
                hash[3] = hash[3] ^ sqrtNumArray[ii+12];
            }
        }
    }
    string stringHash = "";
    decimalToHex (hash, stringHash);
    return stringHash;
}

struct User {
    string name;
    string public_key;
    int balance;
};

struct Transaction {
    string transactionID;
    string sender_public_key;
    string receiver_public_key;
    bool ifSuccessful;
    int amount;
};

struct Block {
// Block Header
    string prevHash, minedBlockHash;
    time_t timeStamp;
    string version;
    string merkelRootHash;
    uint32_t nonce;
    uint32_t difficultyTarget;
// Transaction part
    vector<Transaction> Transactions;

    Block() {
        timeStamp = time(nullptr);
        nonce = 0;
        difficultyTarget = 4;
        version = "v0.1";
    }
};

string calculateHash(Block);
Block mineBlock(Block);
Transaction addTransaction();

int main()
{
    Block newBlock;
    vector<Block> Blockchain;
    newBlock.prevHash = hashFunction("0");
    Blockchain.push_back(newBlock);

    //Blockchain[0].Transactions.push_back(addTransaction());
    //Blockchain.push_back(mineBlock(Blockchain.back()));

    return 0;
}

string calculateHash (Block b) {
    stringstream ss;
    ss << b.version << b.prevHash << b.timeStamp << b.nonce << b.difficultyTarget;
    return hashFunction(ss.str());
}

Block mineBlock (Block b) {
    char numOfNulls[b.difficultyTarget + 1];
    for(int i = 0; i< b.difficultyTarget; i++) {
        numOfNulls[i] = '1';
    }
    numOfNulls[b.difficultyTarget] = '\0';
    string str(numOfNulls);
    do {
        b.nonce++;
        b.minedBlockHash = calculateHash(b);
    }
    while(b.minedBlockHash.substr(0, b.difficultyTarget) != str);

    Block newBlock;
    cout << "Block mined: " << b.minedBlockHash;
    newBlock.prevHash = calculateHash(b);
    return newBlock;
}

Transaction addTransaction() {
    Transaction newTransaction;
    newTransaction.amount = 20;
    return newTransaction;
}
