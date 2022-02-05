#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

// Define a class „MessageQueue“ which has the public methods send and receive. 

template <class T>
class MessageQueue
{
public:
    T receive();
    void send(T &&msg);
    std::deque<T> _queue;
private:
    std::condition_variable _condition;
    std::mutex _messageMutex;
};

enum class TrafficLightPhase {red, green};

class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    ~TrafficLight();

    // getters / setters

    // typical behaviour methods
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();

private:
    // typical behaviour methods
    void cycleThroughPhases();

    // private member variables
    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _phaseQueue; 
};

#endif