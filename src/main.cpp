#include <array>
#include <cstdint>
#include <ios>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <tgbot/types/GenericReply.h>
#include <tgbot/types/KeyboardButton.h>
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
using file_type=std::vector<std::vector<std::string>> ;
using line_type=std::vector<std::string>;

file_type region;
file_type zone;
file_type areaType;
file_type woreda;

char stdsRec[] = "stdtRecord.txt";
char delimeter='\t';
const int nColunm = 26;
/* std::string columnNameList{}; */

std::string columnName[nColunm]={
    "Student national ID",
    "Student institution ID",
    "First name",
    "First name in Amharic",
    "Father name",
    "Father name in Amharic",
    "Grandfather name",
    "Grandfather name in Amharic",
    "Last name",
    "Last name in Amharic",
    "Date of birth",
    "Place of birth",
    "Place of birth in Amharic",
    "Email",
    "Phone",
    "Gender",
    "Country",
    "Citizenship",
    "Region",
    "Zone",
    "Woreda",
    "Kebele",
    "Area type",
    "Student national examiniation ID",
    "Student tax IDentification number",
    "Student national system ID"};
std::string columnDescription[nColunm]={
    "your national ID number.\n<b>Example: </b>WLDU0000114 ",
    "Your university ID number.\n<b>Example: </b>\"WDU1300000\"",
    "Your name in english.\n<b>Example: </b>\"Tomas\"",
    "Your name in Amharic language.\n<b>Example: </b>\"ቶማስ\"",
    "Your father name in english.\n<b>Example: </b>\"Daniel\"",
    "Your father name in Amharic language.\n<b>Example: </b>\"ዳንኤል\"",
    "Ypur grandfather name in english.\n<b>Example: </b>\"Abebe\"",
    "Your grandfather name in Amharic.\n<b>Example: </b>\"አበበ\"",
    "Your last name in english.\n<b>Example: </b>\"Kebede\"",
    "Your last name in Amharic language.\n<b>Example: </b>\"ከበደ\"",
    "Enter with format of <b>month/day/year</b> in Ethiopian count",
    "Your birth place name in english.\n<b>Example: </b>\"Addis Ababa\"",
    "Your birth place name in Amharic language.\n<b>Example: </b>\"አዲስ አበባ\"",
    "Your email that we can reach you easly.\n<b>Example: </b>tommydani242@gmail.com",
    "<b>Example: </b> \"0923852925\"",
    "Choose \"<b>F</b>\" for female or choose \"<b>M</b>\" for male",
    "",
    "",
    "",
    "",
    "",
    "",
    "Pastoral(PS) places which are rural.\nNon pastoral(NPS) places which are towns or cities.",
    "",
    "",
    ""};
TgBot::Bot bot(token);

std::array<std::string, 20> knownIDList[20] {} ;

TgBot::ReplyKeyboardMarkup::Ptr cancelKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr UnknownWithCancelKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr sexKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr updateKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr regionKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr columnLitKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr areaTypeKey{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr updateWContinueKeyboard{new TgBot::ReplyKeyboardMarkup};
TgBot::ReplyKeyboardMarkup::Ptr keyboard{new TgBot::ReplyKeyboardMarkup};


std::string toCode(int,const std::string&);
void createKeyboardFromFile(const std::string& value,file_type& data,TgBot::ReplyKeyboardMarkup::Ptr& kb=keyboard,line_type::size_type datai=2, line_type::size_type buttonString=1,int columnL=4);

std::array<std::string,nColunm+1> columnNumberString{
    "0","1","2","3",
        "4","5","6","7",
        "8","9","10","11",
        "12","13","14","15",
        "16","17","18","19",
        "20","21","22","23",
        "24","25", "Cancel"
};

class M
{
    public:
        std::string storedMessage{"<pre>🌍School👨‍🎓of⚡️Electrical👩‍🎓and🫶Computer🎓Engineering🥸, WDU🇪🇹</pre>\n"};
        TgBot::GenericReply::Ptr keyboardStatus{nullptr};

        void send(const std::string& text)
        {
            storedMessage += "=> " + text + "\n<b>===================================</b>\n";
        }

        void send(const std::string& text,TgBot::GenericReply::Ptr K)
        {
            storedMessage += "=> " + text + "\n<b>===================================</b>\n";
            if(keyboardStatus == nullptr)
            {
                keyboardStatus=K;
            }
            else
                storedMessage+="\n Error: mulltiple keyboardStatus\n";
        }


        void clear()
        {
            storedMessage="<pre>🌍School👨‍🎓of⚡️Electrical👩‍🎓and🫶Computer🎓Engineering🥸, WDU🇪🇹</pre>\n";
            keyboardStatus=nullptr;
        }
};

M mess{};

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
                    mess.send( "<b>👇Choose your sex from buttons👉🔽</b>", sexKeyboard);
                    break;
                case 18:
                    mess.send("<b>👇Choose your region from buttons👉🔽</b>", regionKeyboard);
                    break;
                case 19:
                    createKeyboardFromFile(columnValue[18],zone);
                    mess.send("<b>👇Choose your zone from buttons👉🔽</b>", keyboard);
                    break;
                case 20:
                    createKeyboardFromFile(columnValue[19],woreda);
                    mess.send("<b>👇Choose your woreda from buttons👉🔽</b>", keyboard);
                    break;
                case 22:
                    mess.send("<b>👇Choose from buttons👉🔽</b>", areaTypeKey);
                    break;
                default:
                    mess.send("<b>👇You can choose Unknown if you don't know the value or don't understand the question.👉🔽</b>", UnknownWithCancelKeyboard);
            }
        }

        void askUser(int state)
        {
            mess.send("👉Enter your " + columnName[state] + '?' + '\n' + "<b>Discription: </b>" + columnDescription[state]);
            keyboardBot(state);
        }

        void finishedFunc()
        {
            mess.send("👍You have finished setting up your data.\nYou can update all or change specific value.\nchoose from buttons.",updateKeyboard);
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
                mess.send("<b>🛑You have canceled updating🛑</b>",updateKeyboard);
            if(updateFstate == 1)
                mess.send("<b>🛑Changing canceled🛑</b>",updateKeyboard);
            else if(updateFstate == 0)
                mess.send("<b>You have canceled your registeration! at 🛑" + columnName[state-1] + "🛑</b>\n" + "You can Continue updating by choosing <b>Continue</b> button",updateWContinueKeyboard);
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
                    std::cout << "ERROR: Beyond limit column number";
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
                    mess.send("<b>👇Choose value you want to change👉🔽</b>",columnLitKeyboard);
                    updateFstate=1;
                }
                else
                    finishedFunc();
            }
            else if (updateFstate == 1)
            {
                int exist=0;
                for(auto& ele:columnNumberString)
                {
                    if(ele==value)
                    {
                        exist=1;
                        break;
                    }
                }
                if(exist == 1)
                {
                    updateValue = std::stoi(value);
                    askUser(updateValue);
                    updateFstate = 2;
                }
                else
                {
                    mess.send("<b>👇Choose value you want to change👉🔽\nC👉🔽hoose from buttons👉🔽</b>",columnLitKeyboard);
                    updateFstate=1;
                }
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
            mess.send("Hey " + name + '\n' +  "⚡️This Bot is for WOLDIA university🤓electrical and computer engineering students👨.\n Join channel for material and uptodate informations.\n --> https://t.me/Electrical_and_Computer_E_WDU");
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
            for(int i{0};i<nColunm;i++)
                mess.storedMessage.append( "<b>(" + std::to_string(i) + ") " + columnName[i] +  ":</b> " + columnValue[i] + '\n');
        }

        void printDebug(std::int64_t chatID)
        {
            std::string output{"chatID: " + std::to_string(User::chatID) + ' '  + "ID: " + ID + ' ' + "chat name: " + name + '\n' + "username: " + userName + ' ' + "currentlyRolling: " + std::to_string(currentlyRolling) + '\n' + "state: " + std::to_string(state) + ' ' + "finished: " + std::to_string(finished) + '\n' + "updateFstate: " + std::to_string(updateFstate) + ' ' + "updateValue: " + std::to_string(updateValue) + '\n'};
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
        for(const auto& ele:buttonLayout){
        std::vector<TgBot::KeyboardButton::Ptr> row;
        for(const auto& rowele:ele){
            TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
            button->text = rowele;
            button->requestContact=false;
            button->requestLocation=false;
            row.push_back(button);
        }
        kb->keyboard.push_back(row);
    }
    kb->resizeKeyboard=true;
}

void createKeyboardFromFile(const std::string& value,file_type& data,TgBot::ReplyKeyboardMarkup::Ptr& kb,line_type::size_type datai,line_type::size_type buttonString,int columnL)
{
    kb->keyboard.clear();
    file_type buttonLayout;
    line_type keyBoardColumn;
    int ii{1};
    for(auto& ele:data)
    {
        if(ele[datai] == value)
        {
            if(ii < columnL)
            {
                keyBoardColumn.push_back(ele[buttonString]);
                ++ii;
            }
            else if ( ii == columnL ){
                keyBoardColumn.push_back(ele[buttonString]);
                buttonLayout.push_back(keyBoardColumn);
                keyBoardColumn.clear();
                ii=1;
            }
        }
    }
    if(!keyBoardColumn.empty())
    {
        buttonLayout.push_back(keyBoardColumn);
        keyBoardColumn.clear();
    }
    buttonLayout.push_back(line_type{"Cancel"});

    createKeyboard(buttonLayout,kb);
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
                if(it == data.end())
                {
                        userC.columnValue[i] = "Not set";
                }
                else
                {
                    userC.columnValue[i] = *(it++);
                }
            }
            users.push_back(userC);
        }
    }
    file.close();
}

void fileDataToVectorData(const std::string& fileS,file_type& data){
    std::ifstream file(fileS);
    std::string str;

    if(!file)	{
        std::cout<<"\nNo Record found !!";
        exit(3);
    }

    while (getline(file, str)) {
        if(str.length()>0)
        {
            line_type data2;
            split(str, data2);
            data.push_back(data2);
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
std::string code(const std::string& value,const file_type& list,file_type::size_type R=0,file_type::size_type S=1)
{
        for(auto& ele: list)
            if(ele[S] == value)
                return ele[R];
        return "Unknown";
}

std::string toCode(int state,const std::string& value)
{
    if (state == 18)
    {
        return code(value,region);
    }
    else if(state == 19)
    {
        return code(value,zone);
    }
    else if(state == 20)
    {
        return code(value,woreda);
    }
    else if(state == 22)
    {
        return code(value,areaType);
    }
    return value;
}

void sendALL(const std::int64_t& C,const std::string& S,TgBot::GenericReply::Ptr K)
{
    if( S != "" )
    {
        bot.getApi().sendMessage(C,S,false,0,K,"HTML");
        mess.clear();
    }
    /* else */
    /* { */
    /*         bot.getApi().sendMessage(C,"Message Error"); */
    /* } */
}

int main()
{
    fileDataToVector(users);
    fileDataToVectorData("region.txt", region);
    fileDataToVectorData("zone.txt", zone);
    fileDataToVectorData("areatype.txt",areaType);
    fileDataToVectorData("woreda.txt", woreda);
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
            }
            else if(value == "dddAAAfff")
            {
            bot.getApi().sendDocument(chatID, TgBot::InputFile::fromFile(stdsRec, "Text"));
            }
            else if( message->document != NULL && message->document->fileName == stdsRec)
            {
            TgBot::File::Ptr file = bot.getApi().getFile(message->document->fileId);
            std::ofstream out(stdsRec);
            out << bot.getApi().downloadFile(file->filePath);
            out.close();
            mess.send("DB has been updated u need to update vector too!!!");
            }
            else if (value == "aaappp")
            {
                fileDataToVector(users);
            }
            else if (value == "uuuppp")
            {
                users.clear();
                fileDataToVector(users);
            }

            else if (value == "sssxxx")
            {
                vectorToFile(users);
            }
            else
            {
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
            }
            sendALL(chatID,mess.storedMessage, mess.keyboardStatus);
    });

    signal(SIGINT, [](int s) {
            printf("SIGINT got %d\n",s);
            vectorToFile(users);
            bot.getApi().sendDocument(604585600, TgBot::InputFile::fromFile(stdsRec, "Text"));
            bot.getApi().sendMessage(604585600,"SIGINT GOT");
            exit(0);
            });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (std::exception& e) {
        vectorToFile(users);
        bot.getApi().sendDocument(604585600, TgBot::InputFile::fromFile(stdsRec, "Text"));
        bot.getApi().sendMessage(604585600,"Error: " + std::string{ e.what() });
    }

    return 0;
}
