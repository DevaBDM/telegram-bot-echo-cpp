#include <array>
#include <cstdint>
#include <ios>
#include <iostream>
#include <tgbot/types/ReplyKeyboardMarkup.h>
#include <utility>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <exception>

#include <tgbot/tgbot.h>

std::string token(std::getenv("TOKEN"));

char stdsRec[] = "stdtRecord.txt";
char delimeter='\t';
const int nColunm = 26;
/* std::string columnNameList{}; */

std::string columnName[nColunm]={"student national ID", "student institution ID", "first name", "first name Amh", "father name", "father name Amh", "grandfather name", "grandfather name Amh", "last name", "last name Amh", "date of birth", "place of birth", "place of birth Amh", "email", "phone", "gender", "country", "citizenship", "region", "zone", "kebele", "woreda", "area type", "student national examiniation ID", "student tax IDentification number", "student national system ID"};
std::string columnDescription[nColunm]={"student national ID", "student institution ID", "first name", "first name Amh", "father name", "father name Amh", "grandfather name", "grandfather name Amh", "last name", "last name Amh", "date of birth", "place of birth", "place of birth Amh", "email", "phone", "gender", "country", "citizenship", "region", "zone", "kebele", "woreda", "area type", "student national examiniation ID", "student tax IDentification number", "student national system ID"};
TgBot::Bot bot(token);

std::array<std::string, 20> knownIDList[20]
{

} ;

TgBot::ReplyKeyboardMarkup::Ptr cancelKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr UnknownWithCancelKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr sexKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr updateKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr regionKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr columnLitKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr areaTypeKey{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr updateWContinueKeyboard{new TgBot::ReplyKeyboardMarkup};


std::string toCode(int,std::string);

class User
{
    public:
        std::int64_t chatID{0};
        std::string ID = {"0"};
        std::string name={"0"};
        std::string userName{"0"};
        std::string columnValue[nColunm]={};

        int currentlyRolling=0;
        int state = {0};
        int finished=false;
        int updateFstate=0;
        int updateValue=0;

        void keyboardBot(int state)
        {
            switch (state) {
                case 15:
                    bot.getApi().sendMessage(chatID, "Choose from buttons", false, 0, sexKeyboard);
                    break;
                case 18:
                    bot.getApi().sendMessage(chatID, "Choose from buttons", false, 0, regionKeyboard);
                    break;
                case 22:
                    bot.getApi().sendMessage(chatID, "Choose from buttons", false, 0, areaTypeKey);
                    break;
                default:
                    bot.getApi().sendMessage(chatID, "You can choose Unknown", false, 0, UnknownWithCancelKeyboard);
            }

        }

        void askUser(int state)
        {
            bot.getApi().sendMessage(chatID, "Enter your " + columnName[state] + '\n' + "More discription" + columnDescription[state]);
            keyboardBot(state);
        }


        void finishedFunc()
        {
            bot.getApi().sendMessage(chatID, "You have finished setting up your data\nYou can update all or change specific value",false,0,updateKeyboard);
            currentlyRolling=0;
            finished=1;
            updateFstate=0;
            updateValue=0;
            state=0;

            print();
        }

        void canceledFunc()
        {
            if(updateFstate == 2)
                bot.getApi().sendMessage(chatID, "You have canceled updating",false,0,updateKeyboard);
            if(updateFstate == 1)
                bot.getApi().sendMessage(chatID, "Changing canceled",false,0,updateKeyboard);
            else if(updateFstate == 0)
                bot.getApi().sendMessage(chatID, "You have canceled your registeration! at " + columnName[state-1],false,0,updateWContinueKeyboard);
            currentlyRolling=0;
            finished=1;
            updateFstate=0;
            updateValue=0;

            print();
        }

        void ContcanceledFunc()
        {
            if(state >= nColunm)
            {
                if(state == nColunm)
                {
                    finishedFunc();
                }
                else
                    std::cout << "ERROR: beyond limit column number";
            }
            else
            {
                askUser(state-1);
            }

            currentlyRolling=1;
            finished=0;
            updateFstate=0;
            updateValue=0;
        }
        void contFunc(const std::string& value)
        {
            if(state >= nColunm)
            {
                if(state == nColunm)
                {
                    columnValue[state-1] = toCode(state-1,value);
                    finishedFunc();
                }
                else
                    std::cout << "ERROR: beyond limit column number";
            }
            else
            {
                columnValue[state-1] = toCode(state-1,value);
                askUser(state);
            }
        }

        void updateFunc(const std::string& value)
        {
            if(updateFstate == 0)
            {
                if(value == "Update all")
                {
                    startRegistoration();
                }
                else if ( value == "Change specific" )
                {
                    bot.getApi().sendMessage(chatID,"\n choose from value you want to change",false,0,columnLitKeyboard);
                    updateFstate=1;
                }
                else
                    finishedFunc();
            }
            else if (updateFstate == 1)
            {
                updateValue = std::stoi(value);
                askUser(updateValue);
                updateFstate = 2;
            }
            else if (updateFstate == 2 )
            {
                columnValue[updateValue] = toCode(updateValue,value);
                finishedFunc();
            }
        }

        void startRegistoration()
        {
            currentlyRolling=1;
            state=0;
            askUser(0);
        }

        void newUser()
        {
            bot.getApi().sendMessage(chatID, "This is telegram bot for woldia ECEg students for registeration.\nyou can register using this bot.\n");
            startRegistoration();
        }

        void doStuffs(const std::string& value)
        {
            if (value == "Cancel")
            {
                canceledFunc();
                return;
            } else if ( value == "Continue" )
            {
                ContcanceledFunc();
                return;
            }
            if(currentlyRolling == 0 && finished == 0)
                newUser();
            else if (currentlyRolling == 1 && finished == 0)
                contFunc(value);
            else if (currentlyRolling == 0 && finished == 1)
                updateFunc(value);
            else if (currentlyRolling == 1 && finished == 1)
                contFunc(value);
            state++;
        }

        void print()
        {
            std::string output{"chat name: " + name + "\n\n"};
            for(int i{0};i<nColunm;i++)
                output.append( '(' + std::to_string(i) + ") " + columnName[i] +  "\t\t------->\t\t" + columnValue[i] + '\n');
            bot.getApi().sendMessage(chatID,output);

        }

        void printDebug(std::int64_t chatID)
        {
            std::string output{"chatID: " + std::to_string(chatID) + ' '  + "ID: " + ID + ' ' + "chat name: " + name + '\n' + "username: " + userName + ' ' + "currentlyRolling: " + std::to_string(currentlyRolling) + '\n' + "state: " + std::to_string(state) + ' ' + "finished: " + std::to_string(finished) + '\n' + "updateFstate: " + std::to_string(updateFstate) + ' ' + "updateValue: " + std::to_string(updateValue) + '\n'};
            for(int i{0};i<nColunm;i++)
                output.append( columnName[i] +  "\t=>\t" + columnValue[i] + '\n');
            bot.getApi().sendMessage(chatID,output);

        }

        void saveToFile(std::ofstream& out)
        {
            out << chatID << '\t'  << ID << '\t' << name << '\t'  << userName << '\t' << currentlyRolling << '\t' << state << '\t' << finished << '\t' << updateFstate << '\t' << updateValue;
            for(int i{0};i<nColunm;i++)
                out << '\t' << columnValue[i] ;
        }
};

void createKeyboard(const std::vector<std::vector<std::string>>& buttonLayout, TgBot::ReplyKeyboardMarkup::Ptr& kb)
{
    for (size_t i = 0; i < buttonLayout.size(); ++i) {
        std::vector<TgBot::KeyboardButton::Ptr> row;
        for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
            TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
            button->text = buttonLayout[i][j];
            row.push_back(button);
        }
        kb->keyboard.push_back(row);
    }
    kb->resizeKeyboard=true;
}

std::vector<User> users;


void split(const std::string &s,  std::vector<std::string> &elems)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delimeter))
    {
        elems.push_back(item);
    }
}

void fileDataToVector(std::vector<User>& users){
    std::ifstream file(stdsRec);
    std::string str;

    if(!file)	{
        std::cout<<"\nNo Record found !!";
    }

    while (getline(file, str)) {
        if(str.length()>0)
        {
            std::vector<std::string> data;
            split(str, data);
            auto it{data.begin()};
            User userC{};
            userC.chatID = std::stoi(*(it++));
            userC.ID = *(it++);
            userC.name = *(it++);
            userC.userName = *(it++);
            userC.currentlyRolling = std::stoi(*(it++));
            userC.state = std::stoi(*(it++));
            userC.finished = std::stoi(*(it++));
            userC.updateFstate = std::stoi(*(it++));
            userC.updateValue = std::stoi(*(it++));
            for (int i{0}; i<nColunm;i++)
            {
                userC.columnValue[i] = *(it++);
            }
            users.push_back(userC);
        }
    }
    file.close();
}

void displayAllStdRecord(std::vector<User>& users,std::int64_t chatID)
{
    for (auto& ele : users) {
        ele.printDebug(chatID);
        std::cout << '\n';
    }
}

void vectorToFile(std::vector<User>& users)
{
    std::ofstream out(stdsRec,std::ofstream::trunc);
    for (auto& ele : users) {
        ele.saveToFile(out);
        out << '\n';
    }
    out.close();
}

std::string toCode(int state,std::string value)
{
    if (state == 18)
    {
        std::array<std::pair<std::string, std::string>,12> regionsList
        {
            std::make_pair("ADD","Addis Ababa"),
            std::make_pair("AFA","Afar"),
            std::make_pair("AMH","Amhara"),
            std::make_pair("BEN","Benishangul-Gumuz"),
            std::make_pair("DIR","Dire Dawa"),
            std::make_pair("GAM","Gambela"),
            std::make_pair("HAR","Harari"),
            std::make_pair("ORO","Oromia"),
            std::make_pair("SID","Sidama"),
            std::make_pair("SNNP","SNNPR"),
            std::make_pair("SOM","Ethio Somali"),
            std::make_pair("TIG","Tigray")
        };
        for(auto& ele: regionsList)
            if(ele.second == value)
                return ele.first;
    }
    else if(state == 22)
    {
        std::array<std::pair<std::string, std::string>,2> areaTypeList
        {
            std::make_pair("NPS","Non pastoral"),
            std::make_pair("PS","Pastoral")
        };
        for(auto& ele: areaTypeList)
            if(ele.second == value)
                return ele.first;
    }

    return value;
}

int main()
{
    fileDataToVector(users);
    /* for(int i{0};i<nColunm;i++) */
    /*    columnNameList.append('(' + std::to_string(i) + ") " + columnName[i] + '\n'); */

    createKeyboard({{"Cancel"}}, cancelKeyboard);
    createKeyboard({{"Unknown","Cancel"}}, UnknownWithCancelKeyboard);
    createKeyboard({{"F","M"},{"Cancel"}}, sexKeyboard);
    createKeyboard({{"Update all","Change specific"}}, updateKeyboard);
    createKeyboard({{"Update all","Change specific"},{"Continue"}}, updateWContinueKeyboard);
    createKeyboard({
            {"Addis Ababa", "Afar", "Amhara"},
            {"Benishangul-Gumuz", "Dire Dawa"},
            {"Gambela", "Harari", "Oromia", "Sidama"},
            {"SNNPR", "Ethio Somali", "Tigray"},
            {"Cancel"}
            }, regionKeyboard);
    createKeyboard({
            { "0","1","2","3" },
            {"4","5","6","7"},
            {"8","9","10","11",},
            {"12","13","14","15"},
            {"16","17","18","19"},
            {"20","21","22","23"},
            {"24","25"},
            {"Cancel"}
            }, columnLitKeyboard);
    createKeyboard({{"PS","NPS"},{"Cancel"}},areaTypeKey);


    bot.getEvents().onAnyMessage([](TgBot::Message::Ptr message) {
            std::string value = message->text.c_str();
            auto chatID = message->chat->id;
            std::string name = message->chat->firstName + ' ' + message->chat->lastName;
            std::string username = message->chat->username;
            if(value == "dddAAA")
            {
            displayAllStdRecord(users,chatID);
            return;
            }
            if(value == "dddAAAfff")
            {
            bot.getApi().sendDocument(chatID, TgBot::InputFile::fromFile(stdsRec, "Text"));
            return;
            }

            if( message->document != NULL && message->document->fileName == stdsRec)
            {
            TgBot::File::Ptr file = bot.getApi().getFile(message->document->fileId);
            std::ofstream out(stdsRec);
            out << bot.getApi().downloadFile(file->filePath);
            out.close();
            bot.getApi().sendMessage(chatID,"DB has been updated u need to update vector too!!!");
            return;
            }

            if (value == "aaappp")
            {
                fileDataToVector(users);
                return;
            }

            if (value == "uuuppp")
            {
                users.clear();
                fileDataToVector(users);
                return;
            }

            if (value == "sssxxx")
            {
                vectorToFile(users);
                return;
            }

            int flag=0;
            for (auto& ele : users) {
                if (ele.chatID == chatID)
                {
                    ele.userName = username;
                    ele.name = name;
                    ele.doStuffs(value);
                    flag=1;
                }
            }

            if(!flag)
            {
                User* userN{new User};
                userN->chatID = chatID;
                userN->userName = username;
                userN->name = name;
                userN->doStuffs(value);
                users.push_back(*userN);
            }
            vectorToFile(users);
    });

    signal(SIGINT, [](int s) {
            printf("SIGINT got %d\n",s);
            exit(0);
            });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}

