# Supaprastintos blokų grandinės (blockchain) kūrimas
## Duomenims saugoti naudojamos struktūros
### User
    struct User {
        string name;
        string public_key;
        int balance;
    };
### Transaction
    struct Transaction {
        string transactionID;
        string sender_public_key;
        string receiver_public_key;
        bool ifSuccessful;
        int amount;
    };
### Block
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
    }
 ## Programos veikimas
 Programa nuskaito sukurtus naudotojus bei transakcijas. Paleidžiamas kasimo procesas kurio metu keičiant nonce reikšmę yra ieškomas hash su tam tikru difficultyTarget verte(mano projekte ieškau 3 pirmus vienetus). Tada po 100 transakcijų yra įdedama į naują bloką ir taip vykdomas procejas kol į blokus įdedamos visos transakcijos.
