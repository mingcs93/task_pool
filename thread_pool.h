#pragma once

#include "message_queue.h"
#include "message.h"
#include "router.h"
#include <cassert>
#include <functional>
#include <thread>
#include <vector>

class MultiplePool {
public:
    explicit MultiplePool(size_t thread_num = std::thread::hardware_concurrency())
        : queues_(thread_num)
        , thread_num_(thread_num) 
    {
        auto worker = [this](size_t id) {
            while (true) {
                std::shared_ptr<MessageBase> msg = nullptr;
                if (!queues_[id].pop(msg)) {
                    break;
                }

                if (msg) {
                    route.dispach(msg);
                }
            }
        };

        workers_.reserve(thread_num);
        for (size_t i = 0; i < thread_num; i++) {
            workers_.emplace_back(worker, i);
        }
    }

    int schedule_by_id(const std::shared_ptr<MessageBase>& msg, size_t id = -1) 
    {
        if (msg == nullptr) {
            return -1;
        }

        if (id == -1) {
            id = rand() % thread_num_;
            queues_[id].push(msg);
        }
        else {
            assert(id < thread_num_);
            queues_[id].push(msg);
        }

        return 0;
    }

    ~MultiplePool() {
        for (auto& queue : queues_) {
            queue.stop();
        }

        for (auto& worker : workers_) {
            worker.join();
        }
    }


    template<typename Function>
    void register_handler(const uint64_t cmdID, const Function& func)
    {
        route.register_handler(cmdID, func);
    }

    template<typename Function, typename Self>
    void register_handler(const uint64_t cmdID, const Function& func, const std::shared_ptr<Self> self)
    {
        route.register_handler(cmdID, func, self);
    }

private:
    std::vector<MessageQueue<std::shared_ptr<MessageBase>>> queues_;
    size_t thread_num_;
    std::vector<std::thread> workers_;
    Router route;
};