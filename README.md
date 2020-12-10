# Merkle function implementation

## Kaip sukompiliuoti bei paleisti programą

Prieš pradedant kompiliuoti, libbitcoin turi būti instaliuotas.  
Jei libbitcoin tool instaliuotas tai reikia paleisti komandas parodytas žemiau:

    g++ -std=c++ -o main main.cpp $(pkg-config --cflags --libs libbitcoin)
    ./main
    
## Programos veikimas

Programa veikia kaip ir v0.2. Atliekamas maininimo procesas, naudojant savo sukurtą hash algoritmą, ir jeigu atranda hash'ą, laikoma kad blokas iškastas. Atrenkama 100 transakcijų kurios yra patvirtintos, suskaičiuojamas merkle_root hash'as naudojant create_merkle funkciją ir tada patvirtintos transakcijos sudedamos į bloką, o blokas pridedamas į blockchain.
