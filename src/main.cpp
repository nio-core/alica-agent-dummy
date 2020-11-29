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
        else if (strcmp(argv[i], "--id") == 0)
        {
            args.insert({"id", std::string(argv[++i])});
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
    std::cout << "Usage: sample-agent --id <id> --target <host:port>" << std::endl
              << "Options:" << std::endl
            << "\t\t--id <id>\t\tThe ID of the agent" << std::endl
              << "\t\t--target <host:port>\t\tThe target agent to which messages are sent" << std::endl;
}

void validate_arguments(std::map<std::string, std::string> args) {
    try {
        args.at("id");
    } catch (const std::exception &e) {
        usage();
        throw std::runtime_error("ID missing");
    }

    try
    {
        args.at("target");
    }
    catch (const std::exception &e)
    {
        usage();
        throw std::runtime_error("Target missing");
    }
}

int main(int argc, char *argv[])
{
    auto args = parse_command_line_arguments(argc, argv);
    validate_arguments(args);

    Agent agent(args.at("id"));
    agent.connect(args.at("target"));

    while (true)
    {
        agent.update();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}