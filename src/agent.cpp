#include <agent.h>

#include <AlicaEngineInfo.capnp.h>
#include <AllocationAuthorityInfo.capnp.h>
#include <PlanTreeInfo.capnp.h>
#include <RoleSwitch.capnp.h>
#include <SolverResult.capnp.h>
#include <SyncReady.capnp.h>
#include <SyncTalk.capnp.h>

Agent::Agent() : zmq_context(zmq_ctx_new())
{
    publisher = new capnzero::Publisher(zmq_context, capnzero::Protocol::UDP);
}

Agent::~Agent()
{
    delete publisher;
}

void Agent::connect(const std::string &peerAddress) {
    std::cout << "LOG: Connecting to peer with address" << peerAddress << std::endl;
    publisher->addAddress(peerAddress);
}

void Agent::update()
{
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
    senderID.setValue(kj::StringPtr("sender").asBytes());

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

    std::cout << "LOG: Sending Allocation Authority Information to [ALLOC_AUTH]" << std::endl;
    publisher->send(message, "ALLOC_AUTH");
}

void Agent::sendEngineInfo() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::AlicaEngineInfo::Builder engineInfo = message.initRoot<alica_msgs::AlicaEngineInfo>();

    capnp::List<capnzero::ID>::Builder agentsWithMe = engineInfo.initAgentIdsWithMe(1);
    capnzero::ID::Builder agentId = agentsWithMe[0];
    agentId.setType(1);
    agentId.setValue(kj::StringPtr("agent").asBytes());

    engineInfo.setCurrentPlan("MyPlan");
    engineInfo.setCurrentRole("MyRole");
    engineInfo.setCurrentState("MyState");
    engineInfo.setCurrentTask("MyTask");
    engineInfo.setMasterPlan("MasterPlan");

    capnzero::ID::Builder id = engineInfo.initSenderId();
    id.setType(0);
    id.setValue(kj::StringPtr("sender").asBytes());

    std::cout << "LOG: Sending Alica Engine Info to [ENGINE_INFO]" << std::endl;
    publisher->send(message, "ENGINE_INFO");
}

void Agent::sendPlanTreeInfo() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::PlanTreeInfo::Builder planTreeInfo = message.initRoot<alica_msgs::PlanTreeInfo>();
    capnzero::ID::Builder senderId = planTreeInfo.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    capnp::List<int64_t>::Builder stateIds = planTreeInfo.initStateIds(1);
    stateIds.set(0, 0);

    capnp::List<int64_t>::Builder eps = planTreeInfo.initSucceededEps(2);
    eps.set(0, 0);
    eps.set(1, 1);

    std::cout << "LOG: Sending Plan Tree Info to [PT_INFO]" << std::endl;
    publisher->send(message, "PT_INFO");
}

void Agent::sendRoleSwitch() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::RoleSwitch::Builder roleSwitch = message.initRoot<alica_msgs::RoleSwitch>();
    capnzero::ID::Builder senderId = roleSwitch.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    roleSwitch.setRoleId(0);
    roleSwitch.setType("Type");

    std::cout << "LOG: Sending Role Switch to [ROLE_SWITCH]" << std::endl;
    publisher->send(message, "ROLE_SWITCH");
}

void Agent::sendSolverResult() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::SolverResult::Builder solverResult = message.initRoot<alica_msgs::SolverResult>();
    capnzero::ID::Builder senderId = solverResult.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    capnp::List<alica_msgs::SolverVar>::Builder variables = solverResult.initVars(1);
    alica_msgs::SolverVar::Builder var = variables[0];
    var.setId(0);
    capnp::List<uint8_t>::Builder values = var.initValue(1);
    values.set(0, 1);

    std::cout << "LOG: Sending Solver Result to [SOLVER_RESULT]" << std::endl;
    publisher->send(message, "SOLVER_RESULT");
}

void Agent::sendSyncReady() const
{
    capnp::MallocMessageBuilder message;

    alica_msgs::SyncReady::Builder syncReady = message.initRoot<alica_msgs::SyncReady>();
    capnzero::ID::Builder senderId = syncReady.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    syncReady.setSynchronisationId(0);

    std::cout << "LOG: Sending Sync Ready to [SYNC_READY]" << std::endl;
    publisher->send(message, "SYNC_READY");
}

void Agent::sendSyncTalk() const
{
    capnp::MallocMessageBuilder message;
    alica_msgs::SyncTalk::Builder syncTalk = message.initRoot<alica_msgs::SyncTalk>();
    capnzero::ID::Builder senderId = syncTalk.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    capnp::List<alica_msgs::SyncData>::Builder syncData = syncTalk.initSyncData(1);
    alica_msgs::SyncData::Builder data = syncData[0];
    capnzero::ID::Builder robotId = data.initRobotId();
    robotId.setType(1);
    robotId.initValue(0);

    data.setTransitionId(0);
    data.setTransitionHolds(false);
    data.setAck(true);

    std::cout << "LOG: Sending Sync Talk to [SYNC_TALK]" << std::endl;
    publisher->send(message, "SYNC_TALK");
}