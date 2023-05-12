#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <time.h>
#include "Project.h"
#include <cstdio>



//выводит список пунктов меню, спрашивает у пользователя номер нужного пункта
//и возвращает его индекс
//если пользователь выбрал выход, то возвращает -1
int ShowMenu(const std::vector<std::string> & menu)
{
    while (0 == 0) {

        for (int index = 0; index < menu.size(); index++)
        {
            std::cout << "[" << (index + 1) << "] " << menu[index] << "\n";
        }
        std::cout << "write number of string or '0' to exit" << "\n";

        int targetIndex = 0;
        std::cin >> targetIndex;
        std::cout << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n" << "\n";

        targetIndex--;
        if (targetIndex < 0) {
            std::cout << "exit!\n";
            return -1;
        }

        if (targetIndex >= menu.size())
        {
            std::cout << "too big, try again\n";
            continue;
        }
        return targetIndex;
    }

}

namespace fs = std::filesystem;

class Diary
{
    std::vector<std::string> DiaryMenu
    {

        "add new note",
        "delete note ",
        "view list of notes",
    };
    fs::path Place;

    void collectNotesFiles(std::vector<std::string>& files)
    {
        fs::directory_iterator end_itr; //end-marker
        fs::directory_iterator iter(Place);
        while (iter != end_itr)
        {
            if (iter->status().type() == fs::file_type::regular)
            {
                files.push_back(iter->path().string());
            }
            iter++;
        }
    }

public:
    Diary(const fs::path& x) : Place(x)
    {}
    
    void WorkWithDiary()
    {
        int id = 0;
        while (id != -1)
        {
            id = ShowMenu(DiaryMenu);
            switch (id)
            {
            case 0:
                AskAndAddNote();
                break;
            case 1:
                Delete();
                break;
            case 2:
                ReadNotes();
                break;
            }
        }
}
    
    void AskAndAddNote()
    {
        std::string adding = "";
        //сверка с датой
        auto t = std::time(nullptr);
        tm tm = { 0 };
        localtime_s(&tm, &t);
            
        std::stringstream timestring;
        timestring << std::put_time(&tm, "%Y-%m-%d") << ".txt";
        
        fs::path filePath = Place / timestring.str();

        std::ofstream notefile;
        notefile.open(filePath.c_str(), std::ofstream::out | std::ofstream::app);


        int i = 0;
        std::cout << "please, enter your note" << "\n";
        while (true)
        {
            getline(std::cin, adding);
            notefile << adding << "\n";
            //добавить запись в файл

            if (adding.length() <= 0)
                i++;
            else
                i = 0;
            if (i >= 2)
                break;
        }
        std::cout << "---------------------" << "\n";

    }


    void ReadNotes()
    {
        std::vector<std::string> files;
        collectNotesFiles(files);
        if (files.size() == 0)
        {
            std::cout << "there is no notes" << "\n";
        }
        //вывести список папок пользователю и попросить выбрать
        int F = ShowMenu(files);
        if (F < 0)
            return;

        std::cout << "---------------------" << "\n";
        std::ifstream file;
        std::string x = "";
        file.open(files.at(F));
        if (file.is_open())
        {
            while (file.good())
            {
                std::getline(file, x);
                std::cout << x << "\n";
            }
        }
        std::cout << "---------------------" << "\n";
    }
    
    void Delete()
    {
        std::vector<std::string> files;
        collectNotesFiles(files);
        if (files.size() == 0)
        {
            std::cout << "there is nothing to delete" << "\n";
        }
        int F = ShowMenu(files);
        try {
            std::filesystem::remove(files.at(F));
        }
        catch (const std::filesystem::filesystem_error& err) {
            std::cout << "filesystem error: " << err.what() << '\n';
        }
        
        
        
        
        
        
        //std::remove(files.at(F));

    };


};
class DiaryManager
{
    fs::path _rootPath;

    std::vector<std::string> menu
    {
        "open diary",
            "create diary"
    };
    

public:
    DiaryManager(const std::string & rootFolder) 
        : _rootPath(rootFolder) 
    {}



    void Run()
    {

        while (true) {
            int index = ShowMenu(menu);
            switch (index)
            {
            case 0: //открыть дневник
                ChooseAndOpenDiary();
                break;
            case 1: //создать дневник
                CreateNewAndOpenDiary();
                break;
            default:
                return;
            }
        }

    }
    void CreateNewAndOpenDiary()
    {
        //спросить у пользователя имя папки-дневника
        std::cout << "how do you want name your diary?" << "\n";
        std::string x;
        std::cin >> x;
        fs::path targetPath = _rootPath / x;

        bool result = fs::create_directories(targetPath);
        if (result)
            std::cout << "Your Diary created: " << targetPath << "\n" << std::endl;
        
        //проверить, есть ли уже такая папка-дневник и если есть - выругаться и попросить ввести снова
        //создать дневник в папке и открыть его
    }

    void ChooseAndOpenDiary()
    {
        //прочитать список папок в корневом каталоге
        std::vector<std::string> folders;
        fs::directory_iterator end_itr; //end-marker
        fs::directory_iterator iter(_rootPath);
        while (iter != end_itr)
        {
            if (iter->status().type() == fs::file_type::directory)
            {
                folders.push_back(iter->path().string());
            }
            iter++;
        }
        if (folders.size() == 0)
        {
            std::cout << "there is no Diaries" << "\n";
        }
        //вывести список папок пользователю и попросить выбрать
        int F = ShowMenu(folders);
        if (F < 0)
            return;

        //открыть дневник в выбранном пользователем каталоге или
        Diary x(fs::path(folders.at(F)));
        //вернуться на уровень выше
        x.WorkWithDiary();
    }



};



int main()
{

    DiaryManager manager(".");
    manager.Run();
}
