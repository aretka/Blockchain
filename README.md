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
 Programa nuskaito sukurtus naudotojus bei transakcijas. Programos pradzioje yra sukuriami 5 blokai. Tada po 100 transakcijų yra įdedama į naujai sukurtus blokus. Įdėjus transakcijas yra sudaromas merkelRootHash, kuris yra sudaromas suhashuojant iš eilės hashuojant po 2 transakcijų elementus. Paleidžiamas kasimo procesas, kurio metu keičiant nonce reikšmę yra ieškomas hash su tam tikru difficultyTarget verte(mano projekte ieškau 3 pirmus vienetus) bei ribotu bandymų skaičiumi (100000 hash'ų) ir taip įvykdomas proof of work. Vėliau bandymai atliekami pakeitus difficulty target į 4.
 
 ### 1 Kasimo proceso bandymas (difficultyTarget = 3; maximum number of iterations = 100000)
 
 |Block number|Generated hash|Number of iterations|
|--------------|-------------|---------|
|1|111c7843910d155f76feb99abc7292b0|4500|
|2|111052ceeac9e53094f9bb733eab0a12|1252|
|3|111f066a73c2c3b14bd674b8156e2359|5442|
|4|1118124e6814f41c87e11b1650dbc7ad|3292|
|5|1114113f8bc5d2b122d0aaf2ad26286d|228|

### 2 Kasimo proceso bandymas (difficultyTarget = 4; maximum number of iterations = 100000)

 |Block number|Generated hash|Number of iterations|
|--------------|-------------|---------|
|1|1111e069378bc85fc58a352ffbf91bd5|5266|
|2|Hash not found|100000|
|3|Hash not found|100000|
|4|111101aac4c64c1f1f6bc361dc73e75b|11979|
|5|1111faa943c2f5e5c35a0b2c4d8b072e|6106|
