#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}


void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Mostly Done, see sending of message
    // Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.

    // Random cycle time generation https://stackoverflow.com/questions/1340729/how-do-you-generate-a-random-double-uniformly-distributed-between-0-and-1-from-c
    // I used a faster engine because the randomness of this number seems less important to me (not at all to be honest)
    std::random_device rd;
    std::minstd_rand engine(rd());
    std::uniform_int_distribution<> uniformTimes(4000, 6000);
    long cycleDuration = uniformTimes(engine);

    // Time for comparison (Idea taken from Vehicle.cpp)
    std::chrono::time_point<std::chrono::system_clock> currentCycleStart;
    currentCycleStart = std::chrono::system_clock::now();
    
    while (true) {
        // check if cycle should be over
        long timeGoneBy = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - currentCycleStart).count();
        if (timeGoneBy >= cycleDuration) {
            // switch TrafficLight
            if (_currentPhase == TrafficLightPhase::red){
                _currentPhase = TrafficLightPhase::green;
            } else {
                _currentPhase = TrafficLightPhase::red;
            }

            // send update method
            //###################################################################
            // This is not yet finalized as the message queue is not yet set up
            //#################################################################
            //_queue.send(std::move_currentPhase);

            // get new cycleDuration and currentCycleStart-time
            cycleDuration = uniformTimes(engine);
            currentCycleStart = std::chrono::system_clock::now();
        }


        // sleep for 1ms as defined in task
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

