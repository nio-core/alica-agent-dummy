#pragma once

#include <monitoredpublisher.h>
#include <monitoredsubscriber.h>

class Agent
{
public:
    Agent(int id, int localPort);

    ~Agent();

    void attachListener(const std::string &listenerAddress, const std::string &listenerTopic);

    void run(const std::string &destinationHost);

private:
    void *zmq_context;
    MonitoredPublisher *publisher;
    MonitoredSubscriber *subscriber;

    void sendAllocationAuthorityInfo() const;
    void sendEngineInfo() const;
    void sendPlanTreeInfo() const;
    void sendRoleSwitch() const;
    void sendSolverResult() const;
    void sendSyncData() const;
    void sendSyncReady() const;
    void sendSyncTalk() const;
};
