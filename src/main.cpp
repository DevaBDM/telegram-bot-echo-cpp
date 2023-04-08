#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <iostream>

#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

int main() {
    string token(std::getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());
    string webhookUrl("https://telegram-bot-echo-cpp-production.up.railway.app/5458048718:AAFQJak19F5-XUMDNrHkJql5TvGOFW-XCR8");
    printf("Webhook url: %s\n", webhookUrl.c_str());
    std::string port_s{std::getenv("PORT")};
    unsigned short port_i{static_cast<unsigned short>(std::stoi(port_s))};
    std::cout << "Port: " << port_i << '\n';

    Bot bot(token);
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());

        TgWebhookTcpServer webhookServer(port_i, bot);

        printf("Server starting\n");
        bot.getApi().setWebhook(webhookUrl);
        webhookServer.start();
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}
