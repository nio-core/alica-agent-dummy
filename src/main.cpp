#include <agent.h>
#include <thread>
#include <chrono>
#include <map>
#include <stdexcept>

std::map<std::string, std::string> parse_command_line_arguments(int argc, char *argv[])
{
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--monitor") == 0)
        {
            args.insert({"monitor", std::string(argv[++i])});
        }
        else if (strcmp(argv[i], "--id") == 0)
        {
            args.insert({"id", std::string(argv[++i])});
        }
        else if (strcmp(argv[i], "--update-interval") == 0)
        {
            args.insert({"update-interval", std::string(argv[++i])});
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
    std::cout << "Usage: sample-agent --id <ID> --monitor <HOST:PORT> [--update-interval <INTERVAL>]" << std::endl
              << "Options:" << std::endl
              << "\t\t--id <ID>\t\tThe ID of the agent" << std::endl
              << "\t\t--monitor <HOST:PORT>\t\tA monitor agent to which messages are sent" << std::endl
              << "\t\t--update-interval <INTERVAL>\t\tThe time in seconds between update messages are sent" << std::endl;
}

void validate_arguments(std::map<std::string, std::string> args) {
    try {
        args.at("id");
    } catch (const std::exception &e) {
        usage();
        throw std::runtime_error("ID missing");
    }
}

int determine_update_interval(std::map<std::string, std::string>& args)
{
    int default_interval = 10;
    try
    {
        return std::stoi(args.at("update-interval"));
    }
    catch(std::out_of_range&)
    {
        return default_interval;
    }
    catch(std::invalid_argument&)
    {
        return default_interval;
    }
}

int main(int argc, char *argv[])
{
    auto args = parse_command_line_arguments(argc, argv);
    validate_arguments(args);

    Agent agent(args.at("id"));
    agent.connect(args.at("monitor"));

    int interval = determine_update_interval(args);
    std::chrono::seconds sleep_time(interval);

    while (true)
    {
        agent.update();
        std::this_thread::sleep_for(sleep_time);
    }
}