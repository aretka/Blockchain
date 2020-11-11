# Supaprastintos blokų grandinės (blockchain) kūrimas
## Duomenims saugoti naudojamos struktūros
User
  struct User {
      string name;
      string public_key;
      int balance;
  };
Transaction
  struct Transaction {
      string transactionID;
      string sender_public_key;
      string receiver_public_key;
      bool ifSuccessful;
      int amount;
  };
Block
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
