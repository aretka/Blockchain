#include <bits/stdc++.h>

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
            if (multiplySum == 0)
                multiplySum=1*((int)input[i]);

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
        minedBlockHash = "";
    }
};

string calculateHash(Block);
void mineBlock(Block&, Block&);

int main()
{
    Block newBlock1;
    User newUser;
    Transaction newTransaction;
    vector<Block> Blockchain;
    vector<User> Users;
    vector<Transaction> Transactions;
    newBlock1.prevHash = hashFunction("0");

    Blockchain.push_back(newBlock1);
    Block newBlock2;
    Blockchain.push_back(newBlock2);
    Block newBlock3;
    Blockchain.push_back(newBlock3);
    Block newBlock4;
    Blockchain.push_back(newBlock4);
    Block newBlock5;
    Blockchain.push_back(newBlock5);
    Blockchain.push_back(newBlock5);

    // reading 1000 users
    ifstream usersInput("users.txt");
    for (int i=0; i<1000; i++) {
        usersInput >> newUser.name >> newUser.public_key >> newUser.balance;
        Users.push_back(newUser);
    }

    // reading 10000 transactions
    ifstream transactionsInput("transactions.txt");
    for (int i=0; i<10000; i++) {
        transactionsInput >> newTransaction.transactionID >> newTransaction.sender_public_key >> newTransaction.receiver_public_key >> newTransaction.amount;
        Transactions.push_back(newTransaction);
    }
    vector<Transaction>::iterator itr1, itr2;

    //importing 100 transactions to each mined block
    string merkelData = "", transactionHash = "", data = "";
    bool userFound1=false, userFound2=false, ifTransactionVerified = false;
    int userFound2Int, merkelCounter = 100;
    vector<string> transactionHashes;

    for (int i=0; i<5; i++) {
        merkelCounter = 100;
        merkelData.clear();

        for(int ii=0; ii<100; ii++) {
            data.clear();
            transactionHash.clear();
            userFound1=false;
            userFound2=false;
            ifTransactionVerified=false;
            transactionHashes.push_back(Transactions[ii].transactionID);
            for(int j=0; j<1000; j++) {
                if(Users[j].public_key == Transactions[ii].sender_public_key) {
                    //checking if sender does not exceed its own balance
                    if (Users[j].balance-Transactions[ii].amount < 0)
                    {
                        if (userFound2)
                            Users[userFound2Int].balance+=Transactions[ii].amount;
                        break;
                    }
                    userFound1 = true;
                    Users[j].balance-=Transactions[ii].amount;

                }
                else if(Users[j].public_key == Transactions[ii].receiver_public_key) {
                    Users[j].balance+=Transactions[ii].amount;
                    userFound2Int = j;
                    userFound2 = true;
                }
                if(userFound1==true && userFound2==true)
                     break;
            }

            // checking if transaction hash is the same as transactionID
            data = Transactions[ii].sender_public_key + Transactions[ii].receiver_public_key + std::to_string(Transactions[ii].amount);
            transactionHash = hashFunction(data);
            ifTransactionVerified = (Transactions[ii].transactionID == transactionHash);

            if(userFound1 && userFound2 && ifTransactionVerified)
                Transactions[ii].ifSuccessful = true;
            else
                Transactions[ii].ifSuccessful = false;
            Blockchain[i].Transactions.push_back(Transactions[ii]);
        }

        // counting merkelHash
        while (merkelCounter >=2) {
            for (int q=0; q<merkelCounter/2; q++) {
                transactionHashes[q] = hashFunction(transactionHashes[q*2] + transactionHashes[q*2+1]);
            }
            if (merkelCounter%2==1)
            {
                transactionHashes[merkelCounter/2]=transactionHashes[merkelCounter];
                merkelCounter/=2;
                merkelCounter++;
            }
            else {
                merkelCounter/=2;
            }
        }


        // deleting
        itr1 = Transactions.begin();
        itr2 = Transactions.begin() + 100;
        Transactions.erase(itr1, itr2);
        Blockchain[i].merkelRootHash = transactionHashes[0];
        transactionHashes.clear();
        // searching for appropriate hash

        mineBlock(Blockchain[i], Blockchain[i+1]);
    }

    ofstream merkelOut("merkelHashes.txt");
    for (int i=0; i<5; i++) {
        merkelOut << Blockchain[i].merkelRootHash <<'\n';
    }
    merkelOut.close();

    ofstream usersOut("usersOut.txt");
    for (int i=0; i<1000; i++) {
        usersOut << Users[i].balance << '\n';
    }
    usersOut.close();

    ofstream blockTransactionsOut("bTXS.txt");
    for (int i=0; i<5; i++) {
        blockTransactionsOut << "Block " << i+1 << '\n';
        for (int ii=0; ii<100; ii++) {
            blockTransactionsOut << Blockchain[i].Transactions[ii].transactionID << " " << Blockchain[i].Transactions[ii].ifSuccessful << '\n';
        }
    }
    blockTransactionsOut.close();

    return 0;
}

string calculateHash (Block b) {
    stringstream ss;
    ss << b.version << b.prevHash << b.merkelRootHash<< b.timeStamp << b.nonce << b.difficultyTarget;
    return hashFunction(ss.str());
}

void mineBlock (Block &b1, Block &b2) {
    char numOfNulls[b1.difficultyTarget + 1];
    for(int i = 0; i< b1.difficultyTarget; i++) {
        numOfNulls[i] = '1';
    }
    numOfNulls[b1.difficultyTarget] = '\0';
    string str(numOfNulls);
    while(b1.minedBlockHash.substr(0, b1.difficultyTarget) != str && b1.nonce < 100000) {
        b1.nonce++;
        b1.minedBlockHash = calculateHash(b1);
    };

    if (b1.nonce >= 100000) {
            cout << b1.nonce << " ";
            b1.minedBlockHash = "";
            cout << "Hash not found for this block" << endl;
    }
    else {
        cout << "After " << b1.nonce << " iterations ";
        cout << "Block mined: " << b1.minedBlockHash <<'\n';
        b2.prevHash = b1.minedBlockHash;
    }
}

