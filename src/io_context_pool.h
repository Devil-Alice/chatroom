#pragma once
#include "global.h"

class IoContextPool : public Singleton<IoContextPool>
{
    friend class Singleton<IoContextPool>;
    using work_ptr = std::unique_ptr<asio::io_context::work>;
private:
    std::vector<asio::io_context> io_contexts_;
    std::vector<work_ptr> works_;
    std::vector<std::thread> threads_;
    std::size_t next_context_idx_;

    //这里可以设置size为std::thread::hardware_concurrency()，这样就可以获取当前的核心数
    IoContextPool(std::size_t size = std::thread::hardware_concurrency());

public:
    ~IoContextPool();
    asio::io_context &get_io_context();
    /// @brief 
    void stop();
};