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

void handleMessage(capnp::FlatArrayMessageReader &message)
{
    const std::string msg = message.getRoot<capnzero::String>().getString().cStr();
    std::cout << "Got message " << msg << std::endl;
}

Agent::Agent(int id, int localPort) : zmq_context(zmq_ctx_new())
{
    publisher = new MonitoredPublisher(std::to_string(id) + "/0", zmq_context, capnzero::Protocol::UDP);
    std::cout << "SETUP: Starting publisher " << std::to_string(id) << "/0" << std::endl;

    subscriber = new MonitoredSubscriber(std::to_string(id) + "/1", zmq_context, capnzero::Protocol::UDP);
    std::cout << "SETUP: Starting subscriber " << std::to_string(id) << "/1" << std::endl;
    subscriber->addAddress("*:" + std::to_string(localPort));

    subscriber->setTopic("ALLOC_AUTH");
    std::cout << "SETUP: Subscribing to topic [ALLOC_AUTH]" << std::endl;

    subscriber->setTopic("ENGINE_INFO");
    std::cout << "SETUP: Subscribing to topic [ENGINE_INFO]" << std::endl;

    subscriber->setTopic("PT_INFO");
    std::cout << "SETUP: Subscribing to topic [PT_INFO]" << std::endl;

    subscriber->setTopic("ROLE_SWITCH");
    std::cout << "SETUP: Subscribing to topic [ROLE_SWITCH]" << std::endl;

    subscriber->setTopic("SOLVER_RESULT");
    std::cout << "SETUP: Subscribing to topic [SOLVER_RESULT]" << std::endl;

    subscriber->setTopic("SYNC_READY");
    std::cout << "SETUP: Subscribing to topic [SYNC_READY]" << std::endl;

    subscriber->setTopic("SYNC_TALK");
    std::cout << "SETUP: Subscribing to topic [SYNC_TALK]" << std::endl;

    subscriber->subscribe(handleMessage);
}

Agent::~Agent()
{
}

void Agent::attachListener(const std::string &listenerAddress, const std::string &listenerTopic)
{
    std::cout << "LOG: Connecting to listener at " << listenerAddress << " with topic [" << listenerTopic << "]" << std::endl;
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
    sendPlanTreeInfo();
    sendRoleSwitch();
    sendSolverResult();
    sendSyncReady();
    sendSyncTalk();
}

void Agent::sendAllocationAuthorityInfo() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::AllocationAuthorityInfo::Builder allocationAuthority = message.initRoot<alica_msgs::AllocationAuthorityInfo>();

    capnzero::ID::Builder senderID = allocationAuthority.initSenderId();
    senderID.setType(0);
    senderID.initValue(0);

    capnp::List<alica_msgs::EntrypointRobots>::Builder entrypointRobots = allocationAuthority.initEntrypointRobots(1);
    alica_msgs::EntrypointRobots::Builder robot = entrypointRobots[0];
    robot.setEntrypoint(0);
    robot.initRobots(0);

    allocationAuthority.setPlanId(1);
    allocationAuthority.setParentState(2);
    allocationAuthority.setPlanType(1);
    capnzero::ID::Builder authority = allocationAuthority.initAuthority();
    authority.setType(0);
    authority.initValue(0);

    const std::string msg = message.getRoot<alica_msgs::AllocationAuthorityInfo>().toString().flatten().cStr();

    std::cout << "LOG: Sending Allocation Authority Information to [ALLOC_AUTH]: " << msg << std::endl;
    publisher->send(msg, "ALLOC_AUTH");
}

void Agent::sendEngineInfo() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::AlicaEngineInfo::Builder engineInfo = message.initRoot<alica_msgs::AlicaEngineInfo>();

    capnp::List<capnzero::ID>::Builder agentsWithMe = engineInfo.initAgentIdsWithMe(1);
    capnzero::ID::Builder agentId = agentsWithMe[0];
    agentId.setType(0);
    agentId.initValue(0);

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

void Agent::sendPlanTreeInfo() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::PlanTreeInfo::Builder planTreeInfo = message.initRoot<alica_msgs::PlanTreeInfo>();
    capnzero::ID::Builder senderId = planTreeInfo.initSenderId();
    senderId.setType(0);
    senderId.initValue(0);

    capnp::List<int64_t>::Builder stateIds = planTreeInfo.initStateIds(1);
    stateIds.set(0, 0);

    capnp::List<int64_t>::Builder eps = planTreeInfo.initSucceededEps(2);
    eps.set(0, 0);
    eps.set(1, 1);

    const std::string msg = message.getRoot<alica_msgs::PlanTreeInfo>().toString().flatten().cStr();

    std::cout << "LOG: Sending Plan Tree Info to [PT_INFO]: " << msg << std::endl;

    publisher->send(msg, "PT_INFO");
}

void Agent::sendRoleSwitch() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::RoleSwitch::Builder roleSwitch = message.initRoot<alica_msgs::RoleSwitch>();
    capnzero::ID::Builder senderId = roleSwitch.initSenderId();
    senderId.setType(0);
    senderId.initValue(0);

    roleSwitch.setRoleId(0);

    const std::string msg = message.getRoot<alica_msgs::RoleSwitch>().toString().flatten().cStr();

    std::cout << "LOG: Sending Role Switch to [ROLE_SWITCH]: " << msg << std::endl;
    publisher->send(msg, "ROLE_SWITCH");
}

void Agent::sendSolverResult() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::SolverResult::Builder solverResult = message.initRoot<alica_msgs::SolverResult>();
    capnzero::ID::Builder senderId = solverResult.initSenderId();
    senderId.setType(0);
    senderId.initValue(0);

    capnp::List<alica_msgs::SolverVar>::Builder variables = solverResult.initVars(1);
    alica_msgs::SolverVar::Builder var = variables[0];
    var.setId(0);
    capnp::List<uint8_t>::Builder values = var.initValue(1);
    values.set(0, 1);

    const std::string msg = message.getRoot<alica_msgs::SolverResult>().toString().flatten().cStr();

    std::cout << "LOG: Sending Solver Result to [SOLVER_RESULT]: " << msg << std::endl;
    publisher->send(msg, "SOLVER_RESULT");
}

void Agent::sendSyncReady() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::SyncReady::Builder syncReady = message.initRoot<alica_msgs::SyncReady>();
    capnzero::ID::Builder senderId = syncReady.initSenderId();
    senderId.setType(0);
    senderId.initValue(0);

    syncReady.setSynchronisationId(0);

    const std::string msg = message.getRoot<alica_msgs::SyncReady>().toString().flatten().cStr();

    std::cout << "LOG: Sending Sync Ready to [SYNC_READY]: " << msg << std::endl;
    publisher->send(msg, "SYNC_READY");
}

void Agent::sendSyncTalk() const
{
    capnp::MallocMessageBuilder message;
    alica_msgs::SyncTalk::Builder syncTalk = message.initRoot<alica_msgs::SyncTalk>();
    capnzero::ID::Builder senderId = syncTalk.initSenderId();
    senderId.setType(0);
    senderId.initValue(0);

    capnp::List<alica_msgs::SyncData>::Builder syncData = syncTalk.initSyncData(1);
    alica_msgs::SyncData::Builder data = syncData[0];
    capnzero::ID::Builder robotId = data.initRobotId();
    robotId.setType(1);
    robotId.initValue(0);

    data.setTransitionId(0);
    data.setTransitionHolds(false);
    data.setAck(true);

    const std::string msg = message.getRoot<alica_msgs::SyncTalk>().toString().flatten().cStr();

    std::cout << "LOG: Sending Sync Talk to [SYNC_TALK]: " << msg << std::endl;
    publisher->send(msg, "SYNC_TALK");
}