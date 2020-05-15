#pragma once

#include <capnzero/CapnZero.h>

void handleAllocationAuthority(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [ALLOCATION_AUTHORITY]: " << message.getRoot<alica_msgs::AllocationAuthorityInfo>().toString().flatten().cStr() << std::endl;
}

void hanldeEngineInfo(capnp::FlatArrayMessageReader &message)
{
    std::cout << "Got message on [ENGINE_INFO]: " << message.getRoot<alica_msgs::AlicaEngineInfo>().toString().flatten().cStr() << std::endl;
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