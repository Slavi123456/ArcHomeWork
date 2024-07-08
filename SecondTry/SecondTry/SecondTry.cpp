//#include <iostream>
//#include <filesystem>
//#include <fstream>
//#include <string>
//#include <set>
//#pragma warning(disable: 4996) //for disabling warning about strcpy
//#include <cstring>  
//
//struct InOutArg {
//    char* inPath;
//    char* outPath;
//    ~InOutArg() {
//        delete[]inPath;
//        inPath = nullptr;
//        delete[]outPath;
//        outPath = nullptr;
//    }
//    /*std::string inPath;
//    std::string outPath;*/
//};
//
//void setInOut(int argc, char* argv[], InOutArg& inOut) {
//    for (size_t i = 1; i < argc; i += 2)
//    {
//        if (strcmp(argv[i], "-in") == 0) {
//            inOut.inPath = new char[strlen(argv[i + 1]) + 1];
//            strcpy(inOut.inPath, argv[i + 1]);
//            //strcpy_s(inOut.inPath, strlen(argv[i + 1]), argv[i + 1]);
//            //inOut.inPath = argv[i + 1];
//        }
//        if (strcmp(argv[i], "-out") == 0) {
//            inOut.outPath = new char[strlen(argv[i + 1]) + 1]; //BE CAREFUL: because you can give him size 3 
//                                                               //but will still work and will crash somewhere along the way 
//            strcpy(inOut.outPath, argv[i + 1]);
//            //strcpy_s(inOut.outPath, strlen(argv[i + 1]), argv[i + 1]);
//            //inOut.outPath = argv[i + 1];
//        }
//    }
//}
//
//int main(int argc, char* argv[]) {
//
//    InOutArg inOut;
//    setInOut(argc, argv, inOut);
//
//    std::ofstream ofs(inOut.outPath, std::ios::trunc); 
//    //here it break 
//    //"A breakpoint instruction (__debugbreak() statement or a similar call) was executed in SecondTry.exe."
//    
//    if (!ofs.is_open())
//    {
//        std::cerr << "Couldn't open file";
//        return -1;
//    }
//    //std::cout << ofs.flags();
//    int version = 0;
//    int playersSize = 3;
//    ofs << version << std::endl; //or here with same message
//    ofs << playersSize << std::endl;
//
//    return 0;
//}
