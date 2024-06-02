#include "threadpool.h"
#include "http_conn.h"
template <typename T>
threadpool<T>::threadpool( int actor_model, connection_pool *connPool, int thread_number, int max_requests) : m_actor_model(actor_model),m_thread_number(thread_number), m_max_requests(max_requests), m_threads(NULL),m_connPool(connPool)
{
    static_assert(std::is_base_of<BaseClass, T>::value, "T must be derived from BaseClass");
    if (thread_number <= 0 || max_requests <= 0)
    {
        throw std::exception();
    }
        
    m_threads = new pthread_t[m_thread_number];
    if (!m_threads)
        throw std::exception();
    for (int i = 0; i < thread_number; ++i)
    {
        if (pthread_create(m_threads + i, NULL, worker, this) != 0)
        {
            delete[] m_threads;
            throw std::exception();
        }
        if (pthread_detach(m_threads[i]))
        {
            delete[] m_threads;
            throw std::exception();
        }
    }
}

template <typename T>
threadpool<T>::~threadpool()
{
    delete[] m_threads;
}
// template <typename T>
// bool threadpool<T>::append(T *request)
// {
//     m_queuelocker.lock();
//     if (m_workqueue.size() >= m_max_requests)
//     {
//         m_queuelocker.unlock();
//         return false;
//     }
//     // TODO: 这里的state不应该这里加
//     // request->m_state = state; 
//     m_workqueue.push_back(request);
//     m_queuelocker.unlock();
//     m_queuestat.post();
//     return true;
// }
template <typename T>
bool threadpool<T>::append_p(T *request)
{
    m_queuelocker.lock();
    if (m_workqueue.size() >= m_max_requests)
    {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}
template <typename T>
void *threadpool<T>::worker(void *arg)
{
    threadpool *pool = (threadpool *)arg;
    pool->run();
    return pool;
}
template <typename T>
void threadpool<T>::run()
{
    while (true)
    {
        m_queuestat.wait();
        m_queuelocker.lock();
        if (m_workqueue.empty())
        {
            m_queuelocker.unlock();
            continue;
        }
        T *request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        
        if (!request)
            continue;
        // TODO 这部分要进行封装
        request->run(m_actor_model,m_connPool);
    }
}

template class threadpool<http_conn>;
