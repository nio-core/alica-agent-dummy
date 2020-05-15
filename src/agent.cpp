#include <agent.h>

#include <networksocketeventlistener.h>
#include <relayeventproxy.h>
#include <alica_msgs/AlicaEngineInfo.capnp.h>
#include <alica_msgs/AllocationAuthorityInfo.capnp.h>
#include <alica_msgs/PlanTreeInfo.capnp.h>
#include <alica_msgs/RoleSwitch.capnp.h>
#include <alica_msgs/SolverResult.capnp.h>
#include <alica_msgs/SyncReady.capnp.h>
#include <alica_msgs/SyncTalk.capnp.h>

void handleAllocationAuthority(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [ALLOCATION_AUTHORITY]: " << message.getRoot<alica_msgs::AllocationAuthorityInfo>().toString().flatten().cStr() << std::endl;
}

void hanldeEngineInfo(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [ENGINE_INFO]: " << message.getRoot<capnzero::String>().getString().cStr() << std::endl;
}

void handleEntryPointRobots(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [ENTRYPOINT_ROBOTS]: " << message.getRoot<alica_msgs::EntrypointRobots>().toString().flatten().cStr() << std::endl;
}

void handlePlanTreeInfo(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [PLAN_TREE_INFO]: " << message.getRoot<alica_msgs::PlanTreeInfo>().toString().flatten().cStr() << std::endl;
}

void handleRoleSwitch(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [ROLE_SWITCH]: " << message.getRoot<alica_msgs::RoleSwitch>().toString().flatten().cStr() << std::endl;
}

void handleSolverResult(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [SOLVER_RESULT]: " << message.getRoot<alica_msgs::SolverResult>().toString().flatten().cStr() << std::endl;
}

void handleSyncData(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [SYNC_DATA]: " << message.getRoot<alica_msgs::SyncData>().toString().flatten().cStr() << std::endl;
}

void handleSyncReady(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [SYNC_READY]: " << message.getRoot<alica_msgs::SyncReady>().toString().flatten().cStr() << std::endl;
}

void handleSyncTalk(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [SYNC_TALK]: " << message.getRoot<alica_msgs::SyncTalk>().toString().flatten().cStr() << std::endl;
}

Agent::Agent(int id, int localPort) : zmq_context(zmq_ctx_new())
{
    publisher = new MonitoredPublisher(std::to_string(id) + "/0", zmq_context, capnzero::Protocol::UDP);
    std::cout << "SETUP: Starting publisher " << std::to_string(id) << "/0" << std::endl;

    allocationAuthoritySubscriber = new MonitoredSubscriber(std::to_string(id) + "/1", zmq_context, capnzero::Protocol::UDP);
    allocationAuthoritySubscriber->addAddress("*:" + std::to_string(localPort));
    allocationAuthoritySubscriber->setTopic("ALLOC_AUTH");
    allocationAuthoritySubscriber->subscribe(handleAllocationAuthority);
    std::cout << "SETUP: Starting Allocation Authority Subscriber " << std::to_string(id) << "/1"
              << " on Topic [ALLOC_AUTH]" << std::endl;

    engineInfoSubscriber = new MonitoredSubscriber(std::to_string(id) + "/2", zmq_context, capnzero::Protocol::UDP);
    engineInfoSubscriber->addAddress("*:" + std::to_string(localPort));
    engineInfoSubscriber->setTopic("ENGINE_INFO");
    engineInfoSubscriber->subscribe(hanldeEngineInfo);
    std::cout << "SETUP: Engine Info Subscriber " << std::to_string(id) << "/2"
              << " on Topic [ENGINE_INFO]" << std::endl;

    entryPointRobotsSubscriber = new MonitoredSubscriber(std::to_string(id) + "/3", zmq_context, capnzero::Protocol::UDP);
    entryPointRobotsSubscriber->addAddress("*:" + std::to_string(localPort));
    entryPointRobotsSubscriber->setTopic("EP_ROBOTS");
    entryPointRobotsSubscriber->subscribe(handleEntryPointRobots);
    std::cout << "SETUP: Starting Entry Point Robots Subscirber " << std::to_string(id) << "/3"
              << " on Topic [EP_ROBOTS]" << std::endl;

    planTreeInfoSubscriber = new MonitoredSubscriber(std::to_string(id) + "/4", zmq_context, capnzero::Protocol::UDP);
    planTreeInfoSubscriber->addAddress("*:" + std::to_string(localPort));
    planTreeInfoSubscriber->setTopic("PT_INFO");
    planTreeInfoSubscriber->subscribe(handlePlanTreeInfo);
    std::cout << "SETUP: Starting Plan Tree Info Subscriber " << std::to_string(id) << "/4"
              << " on Topic [PT_INFO]" << std::endl;

    roleSwitchSubscriber = new MonitoredSubscriber(std::to_string(id) + "/5", zmq_context, capnzero::Protocol::UDP);
    roleSwitchSubscriber->addAddress("*:" + std::to_string(localPort));
    roleSwitchSubscriber->setTopic("ROLE_SWITCH");
    roleSwitchSubscriber->subscribe(handleRoleSwitch);
    std::cout << "SETUP: Starting Role Switch Subscriber " << std::to_string(id) << "/5"
              << " on Topic [ROLE_SWITCH]" << std::endl;

    solverResultSubscriber = new MonitoredSubscriber(std::to_string(id) + "/6", zmq_context, capnzero::Protocol::UDP);
    solverResultSubscriber->addAddress("*:" + std::to_string(localPort));
    solverResultSubscriber->setTopic("SOLVER_RESULT");
    solverResultSubscriber->subscribe(handleSolverResult);
    std::cout << "SETUP: Starting Solver Result Subscriber " << std::to_string(id) << "/6"
              << " on Topic [SOLVER_RESULT]" << std::endl;

    syncDataSubscriber = new MonitoredSubscriber(std::to_string(id) + "/7", zmq_context, capnzero::Protocol::UDP);
    syncDataSubscriber->addAddress("*:" + std::to_string(localPort));
    syncDataSubscriber->setTopic("SYNC_DATA");
    syncDataSubscriber->subscribe(handleSyncReady);
    std::cout << "SETUP: Starting Sync Data Subscriber " << std::to_string(id) << "/7"
              << " on Topic [SYNC_DATA]" << std::endl;

    syncReadySubscriber = new MonitoredSubscriber(std::to_string(id) + "/8", zmq_context, capnzero::Protocol::UDP);
    syncReadySubscriber->addAddress("*:" + std::to_string(localPort));
    syncReadySubscriber->setTopic("SYNC_READY");
    syncReadySubscriber->subscribe(handleSyncReady);
    std::cout << "SETUP: Starting Sync Ready Subscriber " << std::to_string(id) << "/8"
              << " on Topic [SYNC_READY]" << std::endl;

    syncTalkSubscriber = new MonitoredSubscriber(std::to_string(id) + "/9", zmq_context, capnzero::Protocol::UDP);
    syncTalkSubscriber->addAddress("*:" + std::to_string(localPort));
    syncTalkSubscriber->setTopic("SYNC_TALK");
    syncTalkSubscriber->subscribe(handleSyncTalk);
    std::cout << "SETUP: Starting Sync Talk Subscriber " << std::to_string(id) << "/9"
              << " on Topic [SYNC_TALK]" << std::endl;
}

Agent::~Agent()
{
    /*zmq_ctx_term(zmq_context);
    zmq_ctx_destroy(zmq_context);

    delete publisher;
    delete allocationAuthoritySubscriber;
    delete engineInfoSubscriber;
    delete entryPointRobotsSubscriber;
    delete planTreeInfoSubscriber;
    delete roleSwitchSubscriber;
    delete solverResultSubscriber;
    delete syncDataSubscriber;
    delete syncReadySubscriber;
    delete syncTalkSubscriber;*/
}

void Agent::attachListener(const std::string &listenerAddress, const std::string &listenerTopic)
{
    MonitorConfiguration configuration(listenerAddress, listenerTopic);

    EventProxy *proxy = new RelayEventProxy(zmq_context, configuration);
    EventListener *listener = new NetworkSocketEventListener(proxy);

    publisher->attachEventListener(listener);
}

void Agent::run(const std::string &destination)
{
    std::cout << "LOG: Connecting publisher to " << destination << std::endl;
    publisher->addAddress(destination);

    sendAllocationAuthorityInfo();

    sendEngineInfo();

    sendEntryPointRobots();

    sendPlanTreeInfo();

    sendRoleSwitch();

    sendSolverResult();

    sendSyncData();

    sendSyncReady();

    sendSyncTalk();
}

void Agent::sendAllocationAuthorityInfo() const
{
    std::cout << "LOG: Sending Allocation Authority Information" << std::endl;
}

void Agent::sendEngineInfo() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::AlicaEngineInfo::Builder engineInfo = message.initRoot<alica_msgs::AlicaEngineInfo>();

    capnp::List<capnzero::ID>::Builder agentsWithMe = engineInfo.initAgentIdsWithMe(1);
    capnzero::ID::Builder agentId = agentsWithMe[0];
    agentId.setType(0);
    agentId.setValue(capnp::Data::Reader());

    engineInfo.setCurrentPlan("MyPlan");
    engineInfo.setCurrentRole("MyRole");
    engineInfo.setCurrentState("MyState");
    engineInfo.setCurrentTask("MyTask");
    engineInfo.setMasterPlan("MasterPlan");

    capnzero::ID::Builder id = engineInfo.initSenderId();
    id.setType(1);
    id.setValue(capnp::Data::Reader());

    const std::string msg = message.getRoot<alica_msgs::AlicaEngineInfo>().toString().flatten().cStr();

    std::cout << "LOG: Sending Alica Engine Info to [ENGINE_INFO]: " << msg << std::endl;
    publisher->send(msg, "ENGINE_INFO");
}

void Agent::sendEntryPointRobots() const
{
    std::cout << "LOG: Sending Entry Point Robots" << std::endl;
}

void Agent::sendPlanTreeInfo() const
{
    std::cout << "LOG: Sending Plan Tree Info" << std::endl;
}

void Agent::sendRoleSwitch() const
{
    std::cout << "LOG: Sending Role Switch" << std::endl;
}

void Agent::sendSolverResult() const
{
    std::cout << "LOG: Sending Solver Result" << std::endl;
}

void Agent::sendSyncData() const
{
    std::cout << "LOG: Sending Sync Data" << std::endl;
}

void Agent::sendSyncReady() const
{
    std::cout << "LOG: Sending Sync Ready" << std::endl;
}

void Agent::sendSyncTalk() const
{
    std::cout << "LOG: Sending Sync Talk" << std::endl;
}