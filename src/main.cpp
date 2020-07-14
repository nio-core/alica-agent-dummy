#include <agent.h>
#include <thread>
#include <chrono>
#include <map>
#include <exception>

std::map<std::string, std::string> parse_command_line_arguments(int argc, char *argv[])
{
    std::map<std::string, std::string> args;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--target") == 0)
        {
            args.insert({"target", std::string(argv[++i])});
        }
        else
        {
            std::cerr << "Ignoring argument: [" << argv[i] << "]" << std::endl;
        }
    }

    return args;
}

void usage()
{
    std::cout << "Usage: sample-agent --target <host:port>" << std::endl
              << "Options:" << std::endl
              << "\t\t--target <host:port>\t\tThe target agent to which messages are sent" << std::endl;
}

int main(int argc, char *argv[])
{
    auto args = parse_command_line_arguments(argc, argv);
    Agent agent;

    try
    {
        args.at("target");
    }
    catch (const std::exception &e)
    {
        std::cerr << "No target chosen" << std::endl;
        usage();
        return 2;
    }

    while (true)
    {
        agent.run(args.at("target"));
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}