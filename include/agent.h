#pragma once

#include <capnzero/CapnZero.h>

class Agent
{
public:
    Agent();

    ~Agent();

    void run(const std::string &destinationHost);

private:
    void *zmq_context;
    capnzero::Publisher *publisher;

    void sendAllocationAuthorityInfo() const;
    void sendEngineInfo() const;
    void sendPlanTreeInfo() const;
    void sendRoleSwitch() const;
    void sendSolverResult() const;
    void sendSyncReady() const;
    void sendSyncTalk() const;
};
