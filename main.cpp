#include <bits/stdc++.h>
#include <bitcoin/bitcoin.hpp>

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
        timeStamp = rand()%100;
        nonce = 0;
        difficultyTarget = 4;
        version = "v0.1";
        minedBlockHash = "";
    }
};

bc::hash_digest create_merkle(bc::hash_list&);
string calculateHash(Block);
bool mineBlock(Block&, string);

int main()
{
    Block genesisBlock;
    User newUser;
    Transaction newTransaction;
    vector<Block> Blockchain;
    vector<User> Users;
    vector<Transaction> Transactions;
    genesisBlock.prevHash = hashFunction("0");
    Blockchain.push_back(genesisBlock);



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
    bool userFound1=false, userFound2=false, ifTransactionVerified = false, ifHashFound;
    int userFound2Int;
    bc::hash_list tx_hashes;
    for (int i=0; i<100; i++) {
        Block newBlock1;
        Block newBlock2;
        Block newBlock3;
        Block newBlock4;
        Block newBlock5;

        // transaction validation
        for(int ii=0; ii<100; ii++) {
            data.clear();
            transactionHash.clear();
            userFound1=false;
            userFound2=false;
            ifTransactionVerified=false;

            for(int j=0; j<1000; j++) {
                if(Users[j].public_key == Transactions[ii].sender_public_key) {
                    //checking if sender does not exceed its own balance
                    if (Users[j].balance-Transactions[ii].amount < 0)
                    {
                        if (userFound2)
                            Users[userFound2Int].balance-=Transactions[ii].amount;
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

            char arr[65];
            Transactions[ii].transactionID.copy(arr, 65);
            tx_hashes.push_back(bc::hash_literal(arr));
        }

        // counting merkelHash using libbitcoin
        const bc::hash_digest merkle_root = create_merkle(tx_hashes);
        string merkleHash = bc::encode_base16(merkle_root);
    
        // searching for appropriate hash
        if(mineBlock(newBlock1, Blockchain[i].minedBlockHash)) {
            for (int j=0; j<100; j++) {
                newBlock1.Transactions.push_back(Transactions[j]);
            }
            newBlock1.merkelRootHash = merkleHash;
            Blockchain.push_back(newBlock1);
        }
        else if (mineBlock(newBlock2, Blockchain[i].minedBlockHash)) {
            for (int j=0; j<100; j++) {
                newBlock2.Transactions.push_back(Transactions[j]);
            }
            newBlock2.merkelRootHash = merkleHash;
            Blockchain.push_back(newBlock2);
        }

        else if (mineBlock(newBlock3, Blockchain[i].minedBlockHash)){
            for (int j=0; j<100; j++) {
                newBlock3.Transactions.push_back(Transactions[j]);
            }
            newBlock3.merkelRootHash = merkleHash;
            Blockchain.push_back(newBlock3);
        }
        else if (mineBlock(newBlock4, Blockchain[i].minedBlockHash)){
            for (int j=0; j<100; j++) {
                newBlock4.Transactions.push_back(Transactions[j]);
            }
            newBlock4.merkelRootHash = merkleHash;
            Blockchain.push_back(newBlock4);
        }
        else if (mineBlock(newBlock5, Blockchain[i].minedBlockHash)){
            for (int j=0; j<100; j++) {
                newBlock5.Transactions.push_back(Transactions[j]);
            }
            newBlock5.merkelRootHash = merkleHash;
            Blockchain.push_back(newBlock5);
        }
        else {
            cout << "Nebuvo rasta tinkamo hasho nei vienam mineriui";
            break;
        } 
        // deleting
        itr1 = Transactions.begin();
        itr2 = Transactions.begin() + 100;
        Transactions.erase(itr1, itr2);
    }

    ofstream merkelOut("merkelHashes.txt");
    for (int i=0; i<5; i++) {
        merkelOut << Blockchain[i].merkelRootHash <<'\n';
    }
    merkelOut.close();

    return 0;
}

bc::hash_digest create_merkle(bc::hash_list& merkle)
{
 if (merkle.empty())
    return bc::null_hash;
 else if (merkle.size() == 1)
    return merkle[0];
 while (merkle.size() > 1)
 {
    if (merkle.size() % 2 != 0)
        merkle.push_back(merkle.back());
    assert(merkle.size() % 2 == 0);
    bc::hash_list new_merkle;
    for (auto it = merkle.begin(); it != merkle.end(); it += 2)
    {
        bc::data_chunk concat_data(bc::hash_size * 2);
        auto concat = bc::serializer<
        decltype(concat_data.begin())>(concat_data.begin());
        concat.write_hash(*it);
        concat.write_hash(*(it + 1));
        bc::hash_digest new_root = bc::bitcoin_hash(concat_data);
        new_merkle.push_back(new_root);
    }
    merkle = new_merkle;
    /* printing current hash list
    std::cout << "Current merkle hash list:" << std::endl;
    for (const auto& hash: merkle)
    std::cout << " " << bc::encode_base16(hash) << std::endl;
    std::cout << std::endl;
    */
 }
 return merkle[0];
}

string calculateHash (Block b) {
    stringstream ss;
    ss << b.version << b.prevHash << b.merkelRootHash<< b.timeStamp << b.nonce << b.difficultyTarget;
    return hashFunction(ss.str());
}

bool mineBlock (Block &b1, string previousHash) {
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
            b1.minedBlockHash = "";
            cout << "Hash not found" << endl;
            return false;
    }
    else {
        cout << "After " << b1.nonce << " iterations ";
        cout << "Block mined: " << b1.minedBlockHash <<'\n';
        b1.prevHash = previousHash;
        return true;
    }
}

