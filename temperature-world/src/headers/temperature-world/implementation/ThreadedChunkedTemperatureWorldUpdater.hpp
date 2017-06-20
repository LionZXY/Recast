//
// Created by Oleg Morozenkov on 18.06.17.
//

#ifndef RECAST_THREADEDCHUNKEDTEMPERATUREWORLDUPDATER_H
#define RECAST_THREADEDCHUNKEDTEMPERATUREWORLDUPDATER_H


#include <list>
#include <queue>
#include <condition_variable>
#include <thread>
#include <future>
#include "temperature-world/interfaces/IUpdater.hpp"
#include "../interfaces/ITemperatureWorld.hpp"
#include "temperature-world/interfaces/ITemperatureWorldChunkable.hpp"
#include "temperature-world/interfaces/ITemperatureWorldChunkableGeneratable.hpp"
#include "temperature-world/interfaces/ITemperatureWorldChunkableObservable.hpp"

/**
 * Implementation of chunked temperature world updater.
 * It updates every chunk asynchronously in a thread pool.
 * This class is thread-safe.
 */
class ThreadedChunkedTemperatureWorldUpdater : public IUpdater {
public:
    ThreadedChunkedTemperatureWorldUpdater(
            std::shared_ptr<ITemperatureWorldChunkableObservable<ITemperatureWorldChunkableGeneratable<ITemperatureWorldChunkable<ITemperatureWorld>>>> world,
            std::function<std::shared_ptr<IUpdater>(std::shared_ptr<ITemperatureWorldBoundable<ITemperatureWorld>>)> makeChunkUpdaterFn);

    ~ThreadedChunkedTemperatureWorldUpdater();

    void update() override;

protected:
    void _work();
    void _watchChunk(std::shared_ptr<ITemperatureWorldBoundable<ITemperatureWorld>> chunk);

    std::shared_ptr<ITemperatureWorldChunkableObservable<ITemperatureWorldChunkableGeneratable<ITemperatureWorldChunkable<ITemperatureWorld>>>> _world;
    std::function<std::shared_ptr<IUpdater>(std::shared_ptr<ITemperatureWorldBoundable<ITemperatureWorld>>)> _makeChunkUpdaterFn;

    std::atomic<bool> _isRunning;
    std::vector<std::thread> _workers;

    std::vector<std::shared_ptr<IUpdater>> _updaters;
    std::mutex _updatersMutex;

    std::vector<std::future<void>> _tasks;
    std::queue<std::pair<std::shared_ptr<IUpdater>, std::promise<void>>> _tasksQueue;
    std::mutex _tasksQueueMutex;
    std::condition_variable _tasksQueueWait;
    std::mutex _tasksQueueWaitMutex;
};


#endif //RECAST_THREADEDCHUNKEDTEMPERATUREWORLDUPDATER_H
