#pragma once

#include <capnzero/CapnZero.h>

class Agent
{
public:
    explicit Agent(const std::string& id);

    ~Agent();

    void connect(const std::string &peerAddress);

    void update();

private:
    std::string id;
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
