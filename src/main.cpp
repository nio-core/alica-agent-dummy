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
        if (strcmp(argv[i], "--id") == 0)
        {
            args.insert({"id", std::string(argv[++i])});
        }
        else if (strcmp(argv[i], "--port") == 0)
        {
            args.insert({"port", std::string(argv[++i])});
        }
        else if (strcmp(argv[i], "--listener") == 0)
        {
            args.insert({"listener", std::string(argv[++i])});
        }
        else if (strcmp(argv[i], "--listenerTopic") == 0)
        {
            args.insert({"listenerTopic", std::string(argv[++i])});
        }
        else if (strcmp(argv[i], "--target") == 0)
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

void validate_arguments(std::map<std::string, std::string> args)
{
    if (args.empty())
    {
        throw std::runtime_error("No arguments supplied");
    }

    std::vector<std::string> missing_arguments;

    try
    {
        args.at("id");
    }
    catch (std::out_of_range)
    {
        missing_arguments.push_back("id");
    }

    try
    {
        args.at("port");
    }
    catch (std::out_of_range)
    {
        missing_arguments.push_back("port");
    }

    try
    {
        args.at("listener");

        try
        {
            args.at("listenerTopic");
        }
        catch (std::out_of_range)
        {
            missing_arguments.push_back("listenerTopic");
        }
    }
    catch (std::out_of_range)
    {
        try
        {
            args.at("listenerTopic");
            args.erase("listenerTopic");
            missing_arguments.push_back("listener");

            std::cerr << "Ignoring argument [listenerTopic] as no listener is specified" << std::endl;
        }
        catch (std::out_of_range)
        {
        }
    }

    std::stringstream missing_args;

    missing_args << "Missing arguments: ";

    for (std::string arg : missing_arguments)
    {
        missing_args << arg << ",";
    }

    std::string missing_arguments_string = missing_args.str();
    missing_arguments_string.erase(missing_arguments_string.end() - 1);

    if (!missing_arguments.empty())
    {
        throw std::runtime_error(missing_arguments_string);
    }
}

void usage()
{
    std::cout << "Usage: sample-agent options" << std::endl
              << "\tRequired options are:" << std::endl
              << "\t\t--id <int32>\t\tThe given id for an agent." << std::endl
              << "\t\t--port <int32>\t\tThe port on which the agent listens for messages." << std::endl
              << "\tOptional options are:" << std::endl
              << "\t\t--listener <host:port>\t\tThe address of an attached capnzero-monitoring listener." << std::endl
              << "\t\t--listenerTopic <string>\t\tThe topic on which the previously in --listener defined listener listens. REQUIRED IF LISTENER IS SET!" << std::endl
              << "\t\t--target <host:port>\t\tThe target agent to which messages are sent. If not specified, the agent runs in passive mode and listenes for messages." << std::endl;
}

int main(int argc, char *argv[])
{
    bool passive_mode = false;
    auto args = parse_command_line_arguments(argc, argv);

    try
    {
        validate_arguments(args);
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "Error in agent configuration" << std::endl
                  << "Messsage was: \"" << e.what() << "\"" << std::endl;

        usage();

        return 1;
    }

    Agent agent(std::stoi(args.at("id")), std::stoi(args.at("port")));

    try
    {
        args.at("listener");
        agent.attachListener(args.at("listener"), args.at("listenerTopic"));
    }
    catch (const std::exception &e)
    {
        std::cout << "No listener chosen, running in non-monitored mode" << std::endl;
    }

    try
    {
        args.at("target");
        passive_mode = false;
    }
    catch (const std::exception &e)
    {
        passive_mode = true;
        std::cout << "No target chosen, running in passive mode" << std::endl;
    }

    while (true)
    {
        if (!passive_mode)
        {
            agent.run(args.at("target"));
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}