#include "io_context_pool.h"

IoContextPool::IoContextPool(std::size_t size) : io_contexts_(size), works_(size), next_context_idx_(0)
{
    for (size_t i = 0; i < size; i++)
    {
        // 用work跟iocontext绑定，这样context就不会在没有任务的时候直接退出了
        works_[i] = std::unique_ptr<asio::io_context::work>(new asio::io_context::work(io_contexts_[i]));

        // emplace_back将线程初始化并添加进threads_
        threads_.emplace_back([this, i]()
                              { io_contexts_[i].run(); });
    }
}

IoContextPool::~IoContextPool()
{
    stop();
    std::cout << "IoContextPool destruct" << std::endl;
}

asio::io_context &IoContextPool::get_io_context()
{
    asio::io_context &context = io_contexts_[next_context_idx_];
    next_context_idx_ = (next_context_idx_ + 1) % io_contexts_.size();
    return context;
}

void IoContextPool::stop()
{
    for (auto &work : works_)
    {
        // 停止
        work->get_io_context().stop();
        // 释放
        work.reset();
    }

    for (auto &t : threads_)
    {
        t.join();
    }

    threads_.clear();
    works_.clear();

}
