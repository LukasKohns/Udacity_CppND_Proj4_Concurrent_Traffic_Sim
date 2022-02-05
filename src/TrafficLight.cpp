#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // wait for and receive new messages and pull them from the queue
    // The received object is then returned by the receive function. 
    std::unique_lock<std::mutex> uLock(_messageMutex);
    _condition.wait(uLock, [this] {return !_queue.empty();});

    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> guardLock(_messageMutex);
    _queue.clear();
    _queue.emplace_back(msg);
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

TrafficLight::~TrafficLight(){}

void TrafficLight::waitForGreen()
{
    // Once it receives TrafficLightPhase::green, the method returns.
    while(true){
        TrafficLightPhase currPhase = _phaseQueue.receive();
        if (currPhase == TrafficLightPhase::green){
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}


void TrafficLight::simulate()
{
    // „cycleThroughPhases“ started in a thread when the public method „simulate“ is called. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this)); 
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // Random cycle time generation https://stackoverflow.com/questions/1340729/how-do-you-generate-a-random-double-uniformly-distributed-between-0-and-1-from-c
    // I used a faster engine because the randomness of this number seems less important to me (not at all to be honest)
    static std::random_device rd;
    static std::minstd_rand engine(rd());
    static std::uniform_real_distribution<> uniformTimes(4000, 6000);
    double cycleDuration = uniformTimes(engine);

    // Time for comparison (Idea taken from Vehicle.cpp)
    std::chrono::time_point<std::chrono::system_clock> currentCycleStart;
    currentCycleStart = std::chrono::system_clock::now();
    double timeGoneBy;
    
    while (true) {
        // check if cycle should be over
        timeGoneBy = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - currentCycleStart).count();
        if (timeGoneBy >= cycleDuration) {
            // switch TrafficLight
            if (_currentPhase == TrafficLightPhase::red){
                _currentPhase = TrafficLightPhase::green;
            } else {
                _currentPhase = TrafficLightPhase::red;
            }

            // send update method
            _phaseQueue.send(std::move(_currentPhase));

            // get new cycleDuration and currentCycleStart-time
            cycleDuration = uniformTimes(engine);
            currentCycleStart = std::chrono::system_clock::now();
        }


        // sleep for 1ms as defined in task
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

