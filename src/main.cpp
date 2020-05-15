#include <agent.h>
#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
    if (argc < 15)
    {
        std::cerr << "Missing id and port!" << std::endl
                  << "Usage: ./sample-agent "
                  << "--id <int32> "
                  << "--port <int32> "
                  << "--listenerHost <string> "
                  << "--listenerPort <int32> "
                  << "--listenerTopic <string> "
                  << "--targetHost <string> "
                  << "--targetPort <int32>"
                  << std::endl;

        return 1;
    }

    int id = std::atoi(argv[2]);
    int port = std::atoi(argv[4]);
    std::string listenerHost = std::string(argv[6]);
    int listenerPort = std::atoi(argv[8]);
    std::string listenerTopic = std::string(argv[10]);
    std::string targetHost = std::string(argv[12]);
    int targetPort = std::atoi(argv[14]);

    if (id == 0 && strcmp(argv[2], "0") != 0)
    {
        std::cerr << "Error: ID " << argv[2] << " not valid!";
        return 2;
    }

    if (port == 0)
    {
        std::cerr << "Error: Port " << argv[4] << " not valid!";
        return 3;
    }

    if (listenerPort == 0 && strcmp(argv[8], "0") != 0)
    {
        std::cerr << "Error: Listener port " << argv[8] << " not valid!";
        return 4;
    }

    if (targetPort == 0 && strcmp(argv[14], "0") != 0)
    {
        std::cerr << "Error: Target port " << argv[14] << " not valid!";
        return 5;
    }

    Agent agent(id, port);

    agent.attachListener(listenerHost + ":" + std::to_string(listenerPort), listenerTopic);

    while (true)
    {
        agent.run(targetHost + ":" + std::to_string(targetPort));
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}