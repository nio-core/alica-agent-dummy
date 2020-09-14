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

    auto allocationAuthority = message.initRoot<alica_msgs::AllocationAuthorityInfo>();

    auto senderID = allocationAuthority.initSenderId();
    senderID.setType(0);
    senderID.setValue(kj::StringPtr("sender").asBytes());

    auto entrypointRobots = allocationAuthority.initEntrypointRobots(1);
    for(auto entrypointRobot: entrypointRobots) {
        entrypointRobot.setEntrypoint(0);
        auto robots = entrypointRobot.initRobots(1);
        for(auto robot: robots) {
            robot.setType(1);
            robot.setValue(kj::StringPtr("robot").asBytes());
        }
    }

    allocationAuthority.setPlanId(1);
    allocationAuthority.setParentState(2);
    allocationAuthority.setPlanType(1);

    auto authority = allocationAuthority.initAuthority();
    authority.setType(2);
    authority.setValue(kj::StringPtr("authority").asBytes());

    std::cout << "LOG: Sending Allocation Authority Information to [ALLOC_AUTH]" << std::endl;
    publisher->send(message, "ALLOC_AUTH");
}

void Agent::sendEngineInfo() const
{
    capnp::MallocMessageBuilder message;

    auto engineInfo = message.initRoot<alica_msgs::AlicaEngineInfo>();

    auto agentsWithMe = engineInfo.initAgentIdsWithMe(3);
    for(auto agent: agentsWithMe) {
        agent.setType(1);
        agent.setValue(kj::StringPtr("agent").asBytes());
    }

    engineInfo.setCurrentPlan("MyPlan");
    engineInfo.setCurrentRole("MyRole");
    engineInfo.setCurrentState("MyState");
    engineInfo.setCurrentTask("MyTask");
    engineInfo.setMasterPlan("MasterPlan");

    auto senderId = engineInfo.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    std::cout << "LOG: Sending Alica Engine Info to [ENGINE_INFO]" << std::endl;
    publisher->send(message, "ENGINE_INFO");
}

void Agent::sendPlanTreeInfo() const
{
    capnp::MallocMessageBuilder message;

    auto planTreeInfo = message.initRoot<alica_msgs::PlanTreeInfo>();
    auto senderId = planTreeInfo.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    int stateIdCount = 1;
    auto stateIds = planTreeInfo.initStateIds(stateIdCount);
    for(int i = 0; i < stateIdCount; i++) {
        stateIds.set(i, i);
    }

    int epsCount = 2;
    auto eps = planTreeInfo.initSucceededEps(epsCount);
    for (int i = 0; i < epsCount; i++) {
        eps.set(i, i);
    }

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

    auto solverResult = message.initRoot<alica_msgs::SolverResult>();
    auto senderId = solverResult.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    int variableCount = 1;
    int valueCount = 3;
    auto variables = solverResult.initVars(variableCount);
    for(auto var : variables) {
        var.setId(0);
        auto values = var.initValue(valueCount);
        for(int i = 0; i < valueCount; i++) {
            values.set(i, i);
        }
    }

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
    auto syncTalk = message.initRoot<alica_msgs::SyncTalk>();
    auto senderId = syncTalk.initSenderId();
    senderId.setType(0);
    senderId.setValue(kj::StringPtr("sender").asBytes());

    int dataCount = 1;
    auto syncData = syncTalk.initSyncData(dataCount);
    for(auto data: syncData) {
        auto robotId = data.initRobotId();
        robotId.setType(1);
        robotId.setValue(kj::StringPtr("sender").asBytes());
        data.setTransitionId(0);
        data.setTransitionHolds(false);
        data.setAck(true);
    }

    std::cout << "LOG: Sending Sync Talk to [SYNC_TALK]" << std::endl;
    publisher->send(message, "SYNC_TALK");
}