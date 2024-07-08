#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <set>
#include <map>

#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
////////////////////////////////////////////////////////////////////////////////////////////
//CONST
const int PLAYER_SIZE_GENERATED = 3;
const int PLAYER_LEVEL_CONTST = 3;
////////////////////////////////////////////////////////////////////////////////////////////
//STRUCT AND CLASSES
class Player {
public:
    int _xp = -1;
    int _level = -1;
    uint32_t _id = 0;
    std::string _name = "";
    //std::set<Player*> friends; //not sure why needs to be pointer when i have id

    std::set<uint32_t> friendsIds;

    int& getLevel() {
        return _level;
    }

    std::ostream& outputPlayer(std::ostream& os, const int version) const {
        switch (version)
        {
        case 0:
            os << _xp << std::endl;
            os << _name << std::endl;
            break;
        case 1:
            os << _xp << std::endl;
            os << _name << std::endl;
            os << _level << std::endl;
            break;
        case 2:
            os << _xp << std::endl;
            os << _name << std::endl;
            os << _level << std::endl;
            os << _id << std::endl;
            break;
        case 3:
            os << _xp << std::endl;
            os << _name << std::endl;
            os << _level << std::endl;
            os << _id << std::endl;
            for (const auto& element : friendsIds) {
                os << element << " ";
            }
            break;
        default:
            break;
        }

        return os;
    }
    std::ostream& outputPlayer(std::ostream& os, const int version, bool isForCout) const {
        os << "Player:" << std::endl;
        switch (version)
        {
        case 0:
            os << "Name: " << _name << std::endl;
            os<<"XP: " << _xp << std::endl;
            break;
        case 1:
            os << "Name: " << _name << std::endl;
            os << "XP: " << _xp << std::endl;
            os << "Level: " << _level << std::endl;
            break;
        case 2:
            os << "Name: " << _name << std::endl;
            os << "ID: " << _id << std::endl;
            os << "XP: " << _xp << std::endl;
            os << "Level: " << _level << std::endl;
            break;
        case 3:
            os << "Name: " << _name << std::endl;
            os << "ID: " << _id << std::endl;
            os << "XP: " << _xp << std::endl;
            os << "Level: " << _level << std::endl;
            os << "Friends id: ";
            for (const auto& element : friendsIds) {
                std::cout << element << " ";
            }
            break;
        default:
            break;
        }

        os << std::endl;
        return os;
    }
    std::istream& readPlayer(std::istream& is, const int version) {
        switch (version)
        {
        case 0:
            is >> _xp;
            is >> _name;
            break;
        case 1:
            is >> _xp;
            is >> _name;
            is >> _level;
            break;
        case 2:
            is >> _xp;
            is >> _name;
            is >> _level;
            is >> _id;
            break;
        case 3:
            is >> _xp;
            is >> _name;
            is >> _level;
            is >> _id;
            int setSize;
            is >> setSize;
            uint32_t friendId;
            for (size_t i = 0; i < setSize; i++)
            {
                is >> friendId;
                friendsIds.insert(friendId);
            }
            break;
        default:
            break;
        }
        return is;
    }
    std::istream& readPlayer(std::istream& is, const int version, bool isForCin) {
        switch (version)
        {
        case 0:
            printf("Xp = ");
            is >> _xp;
            printf("Name = ");
            is >> _name;
            break;
        case 1:
            printf("Xp = ");
            is >> _xp;
            printf("Name = ");
            is >> _name;
            printf("Level = ");
            is >> _level;
            break;
        case 2:
            printf("Xp = ");
            is >> _xp;
            printf("Name = ");
            is >> _name;
            printf("Level = ");
            is >> _level;
            printf("Id = ");
            is >> _id;
            break;
        case 3:
            printf("Xp = ");
            is >> _xp;
            printf("Name = ");
            is >> _name;
            printf("Level = ");
            is >> _level;
            printf("Id = ");
            is >> _id;
            int friendsCount;
            printf("Friends count = ");
            is >> friendsCount;
            printf("Friends id = ");
            uint32_t friendId;
            for (int i = 0; i < friendsCount; i++)
            {
                is >> friendId;
                friendsIds.insert(friendId);
            }
            break;
        default:
            break;
        }
        return is;
    }

    void playerClear() {
        _xp = -1;
        _level = -1;
        _id = 0;
        _name = "";
    }

    /////////////////////////////////////////////////
    // MUST FUNCTION
    //It a must to be made comparison of the elements
    bool operator<(const Player& other) const {
        return _name < other._name;
    }
};

struct InOutArg {
    std::string inPath;
    std::string outPath;
};
/////////////////////////////////////////////
//Enum
enum class States {
    undefined,
    Load,
    Print,
    Save,
    Add,
    Remove,
    Quit
};

////////////////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
std::set<Player> playersSet;
std::map<int, Player> playersMap;
uint32_t playersIdCounter = 0; //
States programState = States::undefined;


std::mutex mtx;
std::condition_variable cv;
bool isUsingConsole = false;

////////////////////////////////////////////////////////////////////////////////////////////
// NEEDED FUNCTIONS

//void fillFileOnVersion0() {
//    Player players[PLAYER_SIZE_GENERATED] = { {1, "Josh"}, {2, "Jann"}, {1, "John"} };
//    const char* filePath = "SAVE_VER0.txt";
//    std::ofstream ofs(filePath, std::ios::trunc);
//    if (!ofs.is_open())
//    {
//        std::cerr << "Couldn't open file";
//        return;
//    }
//
//    //std::cout << ofs.flags();
//    int version = 0;
//    int playersSize = 3;
//    ofs << version << std::endl;
//    ofs << playersSize << std::endl;
//
//    for (size_t i = 0; i < playersSize; i++)
//    {
//        players[i].outputPlayer(ofs, version);
//    }
//    ofs.close();
//
//}
//following the -in filePath1 -out filePath2 or -out ... -in ...
void setInOut(int argc, char* argv[], InOutArg& inOut) {
    for (size_t i = 1; i < argc; i += 2)
    {
        if (strcmp(argv[i], "-in") == 0) {
            /*inOut.inPath = new char[strlen(argv[i])];
            strcpy(inOut.inPath, argv[i + 1]);*/
            inOut.inPath = argv[i + 1];
        }
        if (strcmp(argv[i], "-out") == 0) {
            /*inOut.outPath = new char[strlen(argv[i])];
            strcpy(inOut.outPath, argv[i + 1]);*/
            inOut.outPath = argv[i + 1];
        }
    }
}
int getVersion(const std::string& outPath) {
    int versionPos = outPath.size() - 5;
    if (outPath[versionPos] >= '0' && outPath[versionPos] <= '9')
    {
        return outPath[versionPos] - '0';
    }
    else
    {
        return  -1;
    }
}
bool checkInOut(const InOutArg& inOut) {
    return (!inOut.inPath.empty()) && (!inOut.outPath.empty());
}
void fillMissingProperties(Player& newPlayer, const int version) {
    if (newPlayer._level == -1 && version >= 0)
    {
        newPlayer._level = newPlayer._xp / PLAYER_LEVEL_CONTST;

    }
    if (newPlayer._id == 0 && version >= 1)
    {
        newPlayer._id = playersIdCounter + 1;
        playersIdCounter++;
    }

}

void coutPlayerSet(const int version = 0) {
    for (auto it = playersSet.begin(); it != playersSet.end(); it++)
    {
        it->outputPlayer(std::cout, version, true); //why it needs to be const function to be able to access it??

    }

}
void coutPlayerMap(const int version = 0) {
    for (auto it = playersMap.begin(); it != playersMap.end(); it++)
    {
        //it is pair of <int, Player>
        it->second.outputPlayer(std::cout, version, true);
    }

}

//for delete
void setLevel(int& level, const int xp) {
    level = xp / PLAYER_LEVEL_CONTST;
}
void setId(uint32_t& id) {
    playersIdCounter++;
    id = playersIdCounter;
}

void playerAdd(const int version) {
    Player newPlayer;
    //readPlayer takes one because that's task condition
    if (version == 2)
    {
        newPlayer.readPlayer(std::cin, version - 1, true);//it needs check for valid properties
    }
    else
    {
        newPlayer.readPlayer(std::cin, version, true);//it needs check for valid properties
    }
    
    
    //Do i need functions to generate level and id
    if (version >= 2)
    {
        fillMissingProperties(newPlayer, version);
        std::pair<int, Player> newPair;
        newPair.first = newPlayer._id;
        newPair.second = newPlayer;

        playersMap.insert(newPair);
    }
    else {
        playersSet.insert(newPlayer);
    }

}
void playerRemove(const int version, const uint32_t id) {
    //it is estimated that it will be deleted only from map 
    playersMap.erase(id);
    
}
////////////////////////////////////////////////////////////////////////////////////////////
//ADDITIONAL FUNCTIONS
//FOR SET
void fillPlayerSetHardcoded(const int version) {
    //Do i need to make generated for every version or only the lattest
    Player players[PLAYER_SIZE_GENERATED] = { {1,0, 0, "Josh"}, {2,0,1,"Jann"}, {1,0,2,"John"} };
    /*switch (version)
    {
    case  0:
        for (size_t i = 0; i < PLAYER_SIZE_GENERATED; i++)
        {
            playersSet.insert(players[i]);
        }
        break;
    default:
        break;
    }*/

    for (size_t i = 0; i < PLAYER_SIZE_GENERATED; i++)
    {
        playersSet.insert(players[i]);
    }
}
void fillPlayerSet(const std::string& filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open())
    {
        std::cerr << "Couldn't read file";
    }

    Player newPlayer;
    int version = 0;
    int setSize = 0;
    ifs >> version;
    ifs >> setSize;

    for (size_t i = 0; i < setSize; i++)
    {
        newPlayer.readPlayer(ifs, version);
        fillMissingProperties(newPlayer, version);
        playersSet.insert(newPlayer);

        //Clearing for the next one
        newPlayer.playerClear();
    }
    ifs.close();
}
void writePlayerSet(const std::string& filePath, const int version = 0) {

    std::ofstream ofs(filePath);
    if (!ofs.is_open())
    {
        std::cerr << "Couldn't write file";
    }

    Player newPlayer;
    ofs << version << std::endl;
    ofs << playersSet.size() << std::endl;

    for (auto it = playersSet.begin(); it != playersSet.end(); it++)
    {
        /*if (it->_level == -1 && version == 1)
        //{
        //    setLevel((int&)it->_level, it->_xp);  //why do i can only use const member function in from the set's iterator
        //    //how does it worked casted as a int&
        }*/
        it->outputPlayer(ofs, version);
    }
    ofs.close();
}
/////////////////////////////////////////////
//FOR MAP
void fillPlayerMapHardcoded(const int version) {
    Player players[PLAYER_SIZE_GENERATED] = { {1,0, 0, "Josh"}, {2,0,1,"Jann"}, {1,0,2,"John"} };
    std::pair<int, Player> newPair;
    for (size_t i = 0; i < PLAYER_SIZE_GENERATED; i++)
    {
        newPair.first = players[i]._id;
        newPair.second = players[i];
        playersMap.insert(newPair);
    }
}
void fillPlayerMap(const std::string& filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open())
    {
        std::cerr << "Couldn't read file";
    }

    Player newPlayer;
    int version = 0;
    int setSize = 0;
    ifs >> version;
    ifs >> setSize;

    std::pair<int, Player> newPair;
    for (uint32_t i = 0; i < setSize; i++)
    {
        newPlayer.readPlayer(ifs, version);
        fillMissingProperties(newPlayer, version);

        newPair.first = playersIdCounter + 1; //BE CAREFUL: if you try adding with same key it won't add them to the map
        newPair.second = newPlayer;

        playersMap.insert(newPair);

        //Clearing for the next one
        newPlayer.playerClear();
        newPair.second.playerClear();
    }
    ifs.close();
}
void writePlayerMap(const std::string& filePath, const int version = 0) {

    std::ofstream ofs(filePath);
    if (!ofs.is_open())
    {
        std::cerr << "Couldn't write file";
    }

    ofs << version << std::endl;
    ofs << playersSet.size() << std::endl;

    for (auto it = playersMap.begin(); it != playersMap.end(); it++)
    {
        
        //not really need because it will aready have the level
        /*if (it->second._level == -1 && version >= 1)
        {
            setLevel(it->second._level, it->second._xp);
            
        }
        if (it->second._id == 0 && version >= 2)
        {
            setId(it->second._id);
        }*/
        it->second.outputPlayer(ofs, version);
    }
    ofs.close();
}

////////////////////////////////////////////////////////////////////////////////////////////
void background(const InOutArg& inOut, const int version) {
    std::this_thread::sleep_for(std::chrono::seconds(5));//because of the race condition - isUsingConsole
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] {return !isUsingConsole; });

            //Quiting the program if needed
            if (programState == States::Quit)
            {
                break;
            }
            
            //Variable for talking between threads 
            //To be sure that the variable is changed
            if (!isUsingConsole)
            {
                isUsingConsole = true;

                //First look of the task
                /*
                //i know it's not correct to do all that :D
                if (programState == States::Print)
                {
                    //how to make it look more elegant
                    //but i with the use of set and map

                    //task
                    if (inOut.inPath.empty())
                    {
                        if (version >= 2)
                        {
                            fillPlayerMapHardcoded(version);
                        }
                        else {
                            fillPlayerSetHardcoded(version);
                        }

                    }
                    else {
                        if (version >= 2)
                        {
                            fillPlayerMap(inOut.inPath);
                        }
                        else {
                            fillPlayerSet(inOut.inPath);
                        }

                    }

                    if (version >= 2)
                    {
                        writePlayerMap(inOut.outPath, version);
                        coutPlayerMap(version);
                    }
                    else {
                        writePlayerSet(inOut.outPath, version);
                        coutPlayerSet(version);
                    }
                }*/

                //Its still dependable by in out versions
                switch (programState)
                {
                case States::undefined:
                    break;
                case States::Load:
                    if (inOut.inPath.empty())
                    {
                        printf("inPath is missing");
                        return;
                    }
                    if (version >= 2)
                    {
                        fillPlayerMap(inOut.inPath);
                    }
                    else {
                        fillPlayerSet(inOut.inPath);
                    }
                    printf("Players loaded!\n");
                    break;
                case States::Print:
                    //only if there is not inPath 
                    /*if (inOut.inPath.empty());//how does this goes into the if body every time
                    {
                        printf("Hardcoded players \n");
                        if (version >= 2)
                        {
                            fillPlayerMapHardcoded(version);
                            coutPlayerMap(version);
                            playersMap.clear();
                            break;
                        }
                        else {
                            fillPlayerSetHardcoded(version);
                            coutPlayerSet(version);
                            playersSet.clear();
                            break;
                        }
                    }*/
                    if (version >= 2)
                    {
                        coutPlayerMap(version);
                    }
                    else {
                        coutPlayerSet(version);
                    }
                    break;
                case States::Save:
                    if (inOut.outPath.empty())
                    {
                        printf("outPath is missing");
                        return;
                    }
                    if (version >= 2)
                    {
                        writePlayerMap(inOut.outPath, version);
                    }
                    else {
                        writePlayerSet(inOut.outPath, version);
                    }
                    printf("Players saved!\n");
                    break;
                case States::Add:
                    playerAdd(getVersion(inOut.outPath));
                    printf("Players added! \n");
                    break;
                case States::Remove:
                    uint32_t removeId;
                    printf("Player id to remove: ");
                    std::cin >> removeId;
                    playerRemove(version, removeId);
                    printf("Players removed!\n");
                    break;
                default:
                    break;
                }
                
                //reset program variables
                isUsingConsole = false;
                programState = States::undefined;

                //No need when i have condition variable
                ////Little UI
                //std::cout << std::endl << "///////////////////////////////////////////////////" << std::endl;
                //std::cout << "BackgroundThread will sleep for 2 seconds or more" << std::endl;
                //std::cout << "///////////////////////////////////////////////////" << std::endl;

                //std::this_thread::sleep_for(std::chrono::seconds(2));

            }
        }
        
       
    }
}
void takeCommands() {
    while (true) {
        if (!isUsingConsole)
        {
            if (programState == States::Quit)
            {
                break;
            }

            //For talking between thread and avoid messy console
            isUsingConsole = true;

            //Getting wanted state of the user
            int state;
            std::cout << "What will be next command: " << std::endl;
            std::cout << "{Load = 1, Print = 2, Save = 3, Add = 4, Remove = 5, Quit = 6}: ";
            std::cin >> state;

            //Quiting the program
            programState = (States)state;
            

            //Printing the state for debugging 
            std::cout << "Program state: " << (int)programState << std::endl;

            //Little UI
            std::cout << std::endl << "///////////////////////////////////////////////////" << std::endl;
            std::cout << "CommandThread will sleep for 5 seconds or more" << std::endl;
            std::cout << "///////////////////////////////////////////////////" << std::endl;

            //Setting thread to sleep and giving the other one to write on the console
            isUsingConsole = false;
            cv.notify_all(); //to allow the other thread to work
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

    //std::filesystem::path;
    //InOutArg inOut = checkInOut(argc, argv); //lazy to write big six 
    InOutArg inOut;
    setInOut(argc, argv, inOut);
    //fillFileOnVersion0();
    int version = getVersion(inOut.outPath);
    //background(inOut, version);
    printf("Working with -out %s and -in %s \n", inOut.outPath.c_str(), inOut.inPath.c_str());

    //Seting threads for user input handling
    std::thread commandHandler(takeCommands);
    //Seting thread for program functionality 
    std::thread backgroundHandler(background, inOut, version);
    
    //Thread join
    commandHandler.join();
    backgroundHandler.join();
   
    //Last message
    printf("Quiting the program. Bye!");
    printf("Quiting the program. Bye!");
    return 0;
}


//Questions
//-How to avoid the periodic thread sleep //condition variable?? 
//-What does threa.detach do? 
//-What does thread.emplace_back?
//-How to read uint32_t from file

//Things to do
//-Better Formating for the textFiles //optional :DDDDD
//-And swaping the _xp and the name //optional :D

//Problems 
//-When loading players needs checks for missing arguments only then !!!
//-If you load second time the same file it adds them again to map/set //need func for already existing

//