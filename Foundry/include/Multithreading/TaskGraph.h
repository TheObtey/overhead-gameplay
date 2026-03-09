#ifndef TASK_GRAPH__H_
#define TASK_GRAPH__H_

#include "Task.h"

#include <queue>
#include <vector>

#include "Define.h"

class TaskGraph
{

public:
    TaskGraph() = default;
    TaskGraph(TaskGraph const& other) = delete;
    TaskGraph(TaskGraph&& other) noexcept = delete;

    TaskGraph& operator=(TaskGraph const& other) = delete;
    TaskGraph& operator=(TaskGraph&& other) = delete;
    ~TaskGraph() = default;

    void AddTask(uptr<Task>& task);
    void Execute();

private:
    // Using a custom function object to compare elements.
    struct Comparator
    {
        bool operator()(uptr<Task> const& l, uptr<Task> const& r) const { return l->GetDegree() > r->GetDegree(); }
    };

    std::priority_queue<uptr<Task>, std::vector<uptr<Task>>, Comparator> m_taskQueue;
};

#endif
