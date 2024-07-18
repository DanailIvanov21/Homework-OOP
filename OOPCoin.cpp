#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>

#pragma warning (disable : 4996)
const size_t MAX_SIZE = 64;
unsigned computeHash(const unsigned char* memory, int length) {
    unsigned hash = 0xbeaf;

    for (int c = 0; c < length; c++) {
        hash += memory[c];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

struct User {
    unsigned id;
    char name[128];
};

struct Transaction {
    unsigned sender;
    unsigned receiver;
    double coins;
    long long time;
};

struct TransactionBlock {
    unsigned id;
    unsigned prevBlockId;
    unsigned prevBlockHash;
    int validTransactions;
    Transaction transactions[16];
};


struct Money {
    
    double money; // parite na vseki potrebitel
    unsigned id; // id na userite
    
};
struct OOPCoin
{
    User* users = nullptr;
    TransactionBlock* blocks = nullptr;
    Money* Coins = nullptr;
    size_t sizeOfCoins = 0; // razmera na masiva za pari;
    size_t capacityCoins = 8;
    size_t capacityUsers = 8;
    size_t capacityBlocks = 8;
    size_t sizeUsers = 0;
    size_t sizeBlocks = 0;
    size_t currentBlocks = 0; //tekusht blok
    size_t sizeTransaction = 0;
   
    unsigned currentUser = 0; // zashtoto shte go izpolzvam pri create za da syzdam tranzacciq s tekushtoto id 
    unsigned counter = 0;// promenliva s koqto ste zadawame unikalni indentifikatori na users
    unsigned counterBloks = 0; // zadavame id na bloks


};
void resizeUsers(size_t capacity,size_t sizeUsers, User*& users )
{
    User* newUser = new(std::nothrow)  User[capacity];
    for (size_t i = 0; i < sizeUsers; i++)
    {
        newUser[i] = users[i];
    }
    if (sizeUsers != 0)
    {
        delete[] users;
    }
    users = newUser;
}

void resizeBlocks(size_t capacity, size_t sizeBlock, TransactionBlock*& blocks)
{
    TransactionBlock* newBlocks = new (std::nothrow) TransactionBlock[capacity];
        if (!newBlocks)
        {
            std::cout << "false";
        }
    for (size_t i = 0; i < sizeBlock; i++)
    {
        newBlocks[i] = blocks[i];
    }
    if (sizeBlock != 0)
    {
    
    delete[] blocks;
    }
    blocks = newBlocks;
}

void resizeMoney(size_t capacity, size_t sizeMoney, Money*& Coins)
{
    Money* newCoins = new (std::nothrow) Money[capacity];
    for (size_t i = 0; i < sizeMoney; i++)
    {
        newCoins[i] = Coins[i];
    }
    if (sizeMoney != 0)
    {
        delete[] Coins;
    }
    Coins = newCoins;
}



void initUser(OOPCoin& oopCoin, const char* nameOfUser)
{
    unsigned tmp = oopCoin.sizeUsers;
    oopCoin.counter++;
  
    std::strcpy(oopCoin.users[tmp].name, nameOfUser);
    oopCoin.users[tmp].id = oopCoin.counter;
    oopCoin.sizeUsers++;
    oopCoin.currentUser = oopCoin.counter;

}

void initBlocks(OOPCoin& oopCoin)
{
    if (oopCoin.sizeBlocks == 0)
    {// purvi blok

        size_t tmp = oopCoin.sizeBlocks;
        oopCoin.counterBloks++;
        oopCoin.blocks[tmp].id = oopCoin.counterBloks;
        oopCoin.blocks[tmp].prevBlockId = oopCoin.counterBloks;
        oopCoin.blocks[tmp].validTransactions = 0;
        oopCoin.currentBlocks = oopCoin.sizeBlocks;
        oopCoin.sizeTransaction = 0;

        oopCoin.sizeBlocks++;
    }
    else {
        size_t tmp = oopCoin.sizeBlocks;
        oopCoin.counterBloks++;
        oopCoin.currentBlocks = oopCoin.sizeBlocks;
        oopCoin.blocks[oopCoin.sizeBlocks].id = oopCoin.counterBloks;
        oopCoin.blocks[oopCoin.sizeBlocks].prevBlockId = oopCoin.currentBlocks;
        oopCoin.blocks[oopCoin.sizeBlocks].prevBlockHash = computeHash((const unsigned char*)&(oopCoin.blocks[oopCoin.currentBlocks - 1]), sizeof(oopCoin.blocks[oopCoin.currentBlocks - 1]));
        oopCoin.blocks[oopCoin.sizeBlocks].validTransactions = 0;

        
        oopCoin.sizeTransaction = 0;


        oopCoin.sizeBlocks++;

    }
}

void sendCoins(OOPCoin& oopCoin, unsigned sender, unsigned receiver, double coins, long long time)
{
   
            Transaction newTr;
            newTr.sender = sender;
            newTr.receiver = receiver;
            newTr.coins = coins * 420;
            newTr.time = time;
            if (oopCoin.sizeBlocks == 0)
            {
                initBlocks(oopCoin);
            }
            if (oopCoin.blocks[oopCoin.currentBlocks].validTransactions >= 16)
            {
                if (oopCoin.sizeBlocks >= oopCoin.capacityBlocks)
                {
                    oopCoin.capacityBlocks *= 2;
                    resizeBlocks(oopCoin.capacityBlocks, oopCoin.sizeBlocks, oopCoin.blocks);

                }
                initBlocks(oopCoin);

            }

            if (oopCoin.sizeOfCoins >= oopCoin.capacityCoins)
            {
                oopCoin.capacityCoins *= 2;
                resizeMoney(oopCoin.capacityCoins, oopCoin.sizeOfCoins, oopCoin.Coins);
            }
            else if (oopCoin.sizeOfCoins == 0)
            {
                resizeMoney(oopCoin.capacityCoins, oopCoin.sizeOfCoins, oopCoin.Coins);
                oopCoin.Coins[oopCoin.sizeOfCoins].money = 0; // parite na sistemniq potrebitel
                oopCoin.Coins[oopCoin.sizeOfCoins].id = 0; // aidito na systemniq e 0
                oopCoin.sizeOfCoins++; // veche imame edin potrebitel s pari i tova e sistemniq potrebitel;
            }


            // dali ima takiva ID ta
            if (newTr.sender == 0)// samo kogato syzdavame nov User
            {

                //primerno  Parite na User 1 s id 1 sa zapisani v Coins[1];
                oopCoin.Coins[oopCoin.sizeOfCoins].money = newTr.coins; // taka zapazvame parite na poluchatelq
                oopCoin.Coins[oopCoin.sizeOfCoins].id = newTr.receiver; // taka zapazvame id na poluchatelq
                oopCoin.sizeOfCoins++;

                oopCoin.blocks[oopCoin.currentBlocks].validTransactions++;

            }
            else { // prosto izvikvame funkciqta s podadeni dve aidita
                if (oopCoin.Coins[newTr.sender].money >= newTr.coins)
                {
                    oopCoin.Coins[newTr.receiver].money += newTr.coins;
                    oopCoin.Coins[newTr.sender].money -= newTr.coins;

                    oopCoin.blocks[oopCoin.currentBlocks].validTransactions++;

                }
                else {
                    std::cout << "This transaction is wrong, miss enough coins" << std::endl;
                    return;
                }
            }
            oopCoin.blocks[oopCoin.currentBlocks].transactions[oopCoin.sizeTransaction] = newTr;
            oopCoin.sizeTransaction++;
      
    
}


void createUser(OOPCoin & oopCoin, const char* nameOfUser, double coin)
{
    
    if (oopCoin.sizeUsers >= oopCoin.capacityUsers)
    {
        oopCoin.capacityUsers *= 2;
        resizeUsers(oopCoin.capacityUsers, oopCoin.sizeUsers, oopCoin.users);
    }
    else if (oopCoin.sizeUsers == 0)
    {
        oopCoin.users = new(std::nothrow) User[oopCoin.capacityUsers];
        //resizeUsers(oopCoin.capacityUsers, oopCoin.sizeUsers, oopCoin.users);
    }
    
    initUser(oopCoin, nameOfUser);// slojih ima i id na user
    
    if (oopCoin.sizeBlocks == 0)
    {
        resizeBlocks(oopCoin.capacityBlocks, oopCoin.sizeBlocks, oopCoin.blocks);
    }
    
    long long time = std::time(nullptr);// vremeto na transakciqta
    unsigned sender = 0;
    sendCoins(oopCoin,sender,oopCoin.currentUser, coin,time);
    // tova e ako shte se uvelichavat blokovete?
    
    
    


}




void removeUser(OOPCoin& oopCoin, const char* nameOfUser, User* users)
{
    
    size_t validId = 0;
    bool flag = false;
    for (size_t i = 0; i < oopCoin.sizeUsers; i++)
    {
        if (strcmp(users[i].name, nameOfUser) == 0)
        {
            validId = i;
            flag = true;
        }
    }

    if (flag) // shte se maha user
    {
        for (size_t k = 0; k < oopCoin.sizeUsers; k++)
        {
            if (k == validId)
            {
                size_t num = users[k].id;
                long long time = std::time(nullptr);// vremeto na transakciqta
                double money = oopCoin.Coins[num].money / 420; // vzimame parite na potrebitelq koito iskame da mahnem
                sendCoins(oopCoin, num, 0,money,time);// pravim transakciq v koqto parite na potrebitelq 
                //koito iskame da mahnem gi prshtame na sistemniq potrebitel s id = 0;
               

                for (size_t t = validId; t < oopCoin.sizeUsers - 1; t++)
                {
                    users[t] = users[t + 1];
                }
                oopCoin.sizeUsers--;
                break;
            }
            
        }
        
    }
    else {
        std::cout << "Nqma user s tova ima i ne mahame nishto!" << std::endl;
    }

  
}

bool verifyTransactions(OOPCoin& oopCoin)
{

    if (oopCoin.sizeBlocks - 1 == 0)
    {
        return true; // ako imame samo edin blok
    }
    //ako imame poveche
    for (size_t i = 0; i < oopCoin.sizeBlocks - 1; i++)
    {
        if (oopCoin.blocks[i].id == oopCoin.blocks[i + 1].prevBlockId)
        {
            if (computeHash((const unsigned char*)&(oopCoin.blocks[i]), sizeof(oopCoin.blocks[i])) == oopCoin.blocks[i + 1].prevBlockHash)
            {
                continue;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;



}


void wealthiestUsers(OOPCoin& oopCoin,int number,std::ofstream& BiggestUser)
{
    int num = oopCoin.sizeOfCoins;
    Money* wealthiest  = new (std::nothrow) Money[num+1];
    Money* tmp = new  (std::nothrow) Money[2];
    for (size_t i = 0; i < oopCoin.sizeOfCoins; i++)
    {
        wealthiest[i] = oopCoin.Coins[i];
    }
   
    if (number < oopCoin.sizeOfCoins) // po malko zahstoto na poziciq 0 e sistemniq i zaradi tova ne e ravno
    {


        
        unsigned id = 0;
        double money = 0;
        bool flag = false;
        for (int i = 1; i < oopCoin.sizeOfCoins; i++)
        {
            flag = false;
            money = wealthiest[i].money;
            for (int k = i+1; k < oopCoin.sizeOfCoins; k++)
            {
               
                if (wealthiest[k].money > money)
                {
                    money = wealthiest[k].money;
                    id = k;
                    flag = true;
                }
            }
            if (flag)
            {
                tmp[0] = wealthiest[i];
                wealthiest[i] = wealthiest[id];
                wealthiest[id] = tmp[0];
            }

        }
    }
    else {
        std::cout << "Nedostatuchen broi potrebiteli!" << std::endl;
        return;
    }

    // veche masivut wealthiest e sortiran i zapochvam da zapisvam kolkoto usuri sum podal che iskam
    
    
    
    

    for (int i = 1; i < number + 1; i++)
    {
        BiggestUser << oopCoin.users[wealthiest[i].id - 1].name << ' ' << wealthiest[i].money << std::endl;

    }
    BiggestUser.close();
    
    delete[] tmp;
    delete[] wealthiest;
}


void biggestBlocks(OOPCoin& oopCoin, int number,std::ofstream& BiggestBlocks)
{
    int num = oopCoin.sizeBlocks;
    TransactionBlock* biggest = new (std::nothrow) TransactionBlock[num + 1];
    TransactionBlock* tmp = new (std::nothrow) TransactionBlock[2];
    for (size_t i = 0; i < oopCoin.sizeBlocks; i++)
    {
        biggest[i] = oopCoin.blocks[i];
    }

    if (number <= oopCoin.sizeBlocks) // po malko zahstoto na poziciq 0 e sistemniq i zaradi tova ne e ravno
    {



        unsigned id = 0;
        
        double moneyCurrentBlock = 0; //parite na ediniqblok
        double moneyNextBlock = 0;// parite na sledvashtiqt block
        bool flag = false;
        for (int i = 0; i < oopCoin.sizeBlocks; i++)
        {
            flag = false;
            for (int l = 0; l < biggest[i].validTransactions; l++)
            {
               
                moneyCurrentBlock += biggest[i].transactions[l].coins;
            }
            
            for (int k = i + 1; k < oopCoin.sizeBlocks; k++)
            {
                for (int j = 0; j < biggest[k].validTransactions; j++)
                {

                    moneyNextBlock += biggest[k].transactions[j].coins;
                }

                if (moneyNextBlock > moneyCurrentBlock)
                {
                    moneyCurrentBlock = moneyNextBlock;
                    id = k;
                    flag = true;
                }
            }
            if (flag)
            {
                tmp[0] = biggest[i];
                biggest[i] = biggest[id];
                biggest[id] = tmp[0];
            }
        }
    }
    else {
        std::cout << "Nedostatuchen broi blokove!" << std::endl;
        return;
    }

    // veche masivut biggest e sortiran i zapochvam da zapisvam kolkoto blokove sum podal che iskam



    
    double money = 0;
    for (int i = 0; i < number ; i++)
    {
            for (int l = 0; l < biggest[i].validTransactions; l++)
            {
                  money += biggest[i].transactions[l].coins;
            }


        BiggestBlocks << money << ' ' << biggest[i].id << std::endl;

    }
    BiggestBlocks.close();
    

    
    delete[] tmp;
    delete[] biggest;
}


bool readUsers(OOPCoin& oopCoin, std::ifstream& file)
{
    size_t size = 0;
    file.read((char*)&size, sizeof(size));
    if (!file)
    {
        return false;
    }
    if (size < 0)
    {
        std::cout << "Wrong" << std::endl;
        return false;
    }

    User* tmp = new(std::nothrow)  User[size + 1];
    if (!tmp)
    {
        std::cout << "Greshno zadelqne na pamet" << std::endl;
        return false;
    }

    for (size_t i = 0; i < size; i++)
    {
        file.read((char*)(tmp + i), sizeof(User));
        if (!file)
        {
            delete[] tmp;
            return false;
        }
    }

    oopCoin.sizeUsers = size; 
    
    oopCoin.capacityUsers = size + 1;
    delete[] oopCoin.users;
    oopCoin.users = tmp;
    oopCoin.counter = oopCoin.users[size - 1].id;
    return true;

}


bool readBlocks(OOPCoin& oopCoin, std::ifstream& file)
{
    int size = 0;
    file.read((char*)&size, sizeof(size));
    if (size < 0 || !file)
    {
        std::cout << "Wrong" << std::endl;
        return false;
    }

    TransactionBlock* tmp = new (std::nothrow) TransactionBlock[size + 1];
    if (!tmp)
    {
        std::cout << "Greshno zadelena pamet!" << std::endl;
        return false;
    }

    for (int i = 0; i < size; i++)
    {
        file.read((char*)(tmp + i), sizeof(TransactionBlock));
        if (!file)
        {
            std::cout << "Wrong!" << std::endl;
            delete[] tmp;
            return false;
        }
    }

    oopCoin.sizeBlocks = size;
    oopCoin.capacityBlocks = size + 1;
    delete[] oopCoin.blocks;
    oopCoin.blocks = tmp;
    oopCoin.currentBlocks = oopCoin.sizeBlocks - 1;
    oopCoin.sizeTransaction = oopCoin.blocks[oopCoin.currentBlocks].validTransactions;
    oopCoin.counterBloks = oopCoin.sizeBlocks;
    return true;
    
}


bool readMoney(OOPCoin& oopCoin, std::ifstream& file)
{
    size_t size = 0;
    file.read((char*)&size, sizeof(size));
    if (!file)
    {
        return false;
    }
    if (size < 0)
    {
        std::cout << "Wrong" << std::endl;
        return false;
    }

    Money* tmp = new (std::nothrow) Money[size + 1];
    if (!tmp)
    {
        std::cout << "Greshno zadelqne na pamet" << std::endl;
        return false;
    }

    for (size_t i = 0; i < size; i++)
    {
        file.read((char*)(tmp + i), sizeof(Money));
        if (!file)
        {
            delete[] tmp;
            return false;
        }
    }

    oopCoin.sizeOfCoins = size;
    oopCoin.capacityCoins = size + 1;

    
    delete[] oopCoin.Coins;
    oopCoin.Coins = tmp;
    
    return true;

}


bool writeUsers(OOPCoin& oopCoin, std::ofstream& file) 
{
    file.write((const char*)&oopCoin.sizeUsers, sizeof(oopCoin.sizeUsers));
    if (!file)
    {
        std::cout << "Wrong" << std::endl;
        return false;
    }
    file.write((const char*)&oopCoin.users, sizeof(User) * oopCoin.sizeUsers);
    if (file)
    {
        return true;
        
    }
    return false;
}

bool writeBlocks(OOPCoin& oopCoin, std::ofstream& file) 
{
    file.write((const char*)&oopCoin.sizeBlocks, sizeof(oopCoin.sizeBlocks));
    if (!file)
    {
        std::cout << "Wrong" << std::endl;
        return false;
    }
    file.write((const char*)&oopCoin.blocks, sizeof(TransactionBlock) * oopCoin.sizeBlocks);
    if (file)
    {
        return true;

    }
    return false;
}

bool writeMoney(OOPCoin& oopCoin, std::ofstream& file) 
{
    file.write((const char*)&oopCoin.sizeOfCoins, sizeof(oopCoin.sizeOfCoins));
    if (!file)
    {
        std::cout << "Wrong" << std::endl;
        return false;
    }
    file.write((const char*)&oopCoin.Coins, sizeof(Money) * oopCoin.sizeOfCoins);
    if (file)
    {
        return true;

    }
    return false;
}





int main()
{
    
    OOPCoin oopCoin;

    bool flag = true;
    std::ifstream file("users.dat", std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Error while opening the file!" << std::endl;
        
    }
    else {
        if (readUsers(oopCoin, file))
        {
            std::cout << "Read is completed!" << std::endl;
        }
        else {
            std::cout << "Read isn't completed!" << std::endl;
        }
        file.close();
    }

    std::ifstream file1("blocks.dat", std::ios::binary);
    if (!file1.is_open())
    {
        std::cout << "Error while opening the file!" << std::endl;

    }
    else {
        if (readBlocks(oopCoin, file1))
        {
            std::cout << "Read is completed!" << std::endl;
        }
        else {
            std::cout << "Read isn't completed!" << std::endl;
        }
        file1.close();
    }

    std::ifstream file3("money.dat", std::ios::binary);
    if (!file3.is_open())
    {
        std::cout << "Error while opening the file!" << std::endl;

    }
    else {
        if (readMoney(oopCoin, file3))
        {
            std::cout << "Read is completed!" << std::endl;
        }
        else {
            std::cout << "Read isn't completed!" << std::endl;
        }
        file3.close();
    }



    char command[MAX_SIZE];

    do
    {
        std::cout << "Write commands for OOP Coin: " << std::endl;
        std::cin >> command;

        if (strcmp(command, "create-user") ==0)
        {
            std::cout << " Enter the name of the user you want to add" << std::endl;
            char nameOfUser[MAX_SIZE];
            double coin = 0;
            //std::cin.getline(nameOfUser, MAX_SIZE);
            std::cin >> nameOfUser;
            std::cout << "Enter the coin: " << std::endl;
            std::cin >> coin;
            createUser(oopCoin, nameOfUser, coin);
            std::cout << std::endl;
        }
        else if (strcmp(command, "remove-user") == 0)
        {
            std::cout << " Enter the name of the user you want to remove" << std::endl;
            char nameOfUser[MAX_SIZE];
            
            std::cin >> nameOfUser;
            removeUser(oopCoin,nameOfUser, oopCoin.users);
            std::cout << std::endl;
        }
        else if (strcmp(command, "send-coins") == 0)
        {
            std::cout << "Enter the id of the sender and receiver" << std::endl;
            unsigned sender = 0;
            unsigned receiver = 0;
            std::cin >> sender;
            std::cin >> receiver;

            std::cout << "Enter the coins you want to transfer " << std::endl;
            double coin = 0;
            std::cin >> coin;
            long long time = std::time(nullptr);
            sendCoins(oopCoin,sender, receiver, coin, time);
        }
        else if (strcmp(command, "welthiest-users")== 0)
        {
            int number;
            std::cout << "Kolko bogati potrebiteli iskame da zapazim vuv fail" << std::endl;
            std::cin >> number;
            //std::cout << "Name of file: " << std::endl;
            char nameOfUser[MAX_SIZE];
           
            //std::cin >> nameOfUser;
           
            std::sprintf(nameOfUser, "welthiestUser_%ld.txt", std::time(nullptr));
            
            std::ofstream welthiestUser(nameOfUser);
            if (!welthiestUser.is_open())
            {
                std::cout << "File can't be opened!" << std::endl;
                return -1;
            }
            
            wealthiestUsers(oopCoin, number,welthiestUser);
            

            std::ifstream fileUsers(nameOfUser);
            if (!fileUsers.is_open())
            {
                std::cout << "File can't be opened!" << std::endl;
                
            }
            // pri cheteneto ot tekstoviq fail ako sa mnogo potrebitelite v nqkoi sluchai cikula e bezkraen,
            // no si raboti iprintira pravilno neshatat pri drugi opiti
            char buffer[MAX_SIZE];
            while (!fileUsers.eof())
            {
                fileUsers.getline(buffer, MAX_SIZE);
                std::cout << buffer << std::endl;
            }

            fileUsers.close();


        }
        else if (strcmp(command, "biggest-blocks")== 0)
        {
            int number;
            std::cout << "Kolko bogati blockove iskame da zapazim vuv fail" << std::endl;
            std::cin >> number;
            //std::cout << "Name of file: " << std::endl;
            char nameOfUser[MAX_SIZE] = { '\0' };

            //std::cin >> nameOfUser;

            std::sprintf(nameOfUser, "biggestBlock_%ld.txt", std::time(nullptr));

            std::ofstream BiggestBlocks(nameOfUser);
            if (!BiggestBlocks.is_open())
            {
                std::cout << "File can't be opened!" << std::endl;
                return -1;
            }

            biggestBlocks(oopCoin, number, BiggestBlocks);
              
            // pri cheteneto ot tekstoviq fail ako sa mnogo potrebitelite v nqkoi sluchai cikula e bezkraen,
            // no si raboti iprintira pravilno neshatat pri drugi opiti
              std::ifstream fileBlocks(nameOfUser);
              if (!fileBlocks.is_open())
              {
                  std::cout << "File can't be opened!" << std::endl;

              }

              char buffer[MAX_SIZE];
              while (!fileBlocks.eof())
              {
                  fileBlocks.getline(buffer, MAX_SIZE);
                  std::cout << buffer << std::endl;
              }

              fileBlocks.close();




        }
        else if (strcmp(command, "verify-transactions") == 0)
        {
            if (verifyTransactions(oopCoin))
            {
                std::cout << "Verify transactions is completed!" << std::endl;
            }
            else {
                std::cout << "Wrong verifyTransactions!" << std::endl;
            }
        }
        else if(strcmp(command, "exit") == 0)
        {
           flag = false;

           std::ofstream oFile("users.dat", std::ios::binary);
           std::ofstream oFile2("blocks.dat", std::ios::binary);
           std::ofstream oFile3("money.dat", std::ios::binary);
           if (!oFile.is_open())
           {
               std::cout << "Error while opening the file!" << std::endl;

           }
           else {
               if (writeUsers(oopCoin, oFile))
               {
                   std::cout << "Write is completed!" << std::endl;
               }
               else {
                   std::cout << "Write isn't completed!" << std::endl;
               }
               oFile.close();
           }

           if (!oFile2.is_open())
           {
               std::cout << "Error while opening the file!" << std::endl;

           }
           else {
               if (writeBlocks(oopCoin, oFile2))
               {
                   std::cout << "Write is completed!" << std::endl;
               }
               else {
                   std::cout << "Write isn't completed!" << std::endl;
               }
               oFile2.close();
           }

           if (!oFile3.is_open())
           {
               std::cout << "Error while opening the file!" << std::endl;

           }
           else {
               if (writeMoney(oopCoin, oFile3))
               {
                   std::cout << "Write is completed!" << std::endl;
               }
               else {
                   std::cout << "Write isn't completed!" << std::endl;
               }
               oFile3.close();
           }
        } 
    } while (flag);

    delete[] oopCoin.users;
    delete[] oopCoin.blocks;
    delete[] oopCoin.Coins;

    
}
