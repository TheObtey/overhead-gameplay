#include "Multithreading/TaskGraph.h"

void TaskGraph::AddTask(uptr<Task>& task)
{
    m_taskQueue.push(std::move(task));
}

void TaskGraph::Execute()
{
    while(m_taskQueue.empty() == false)
    {
        //TODO Multithread this my g
        m_taskQueue.top()->TaskFunction();
        m_taskQueue.pop();
    }
}
