//
// Created by uyplayer on 2025/8/25.
//

#include "once_call.h"

namespace components
{
    OnceCall::OnceCall() : state_(State::Uninitialized)
    {
    }

    OnceCall::~OnceCall() = default;

    bool OnceCall::is_initialized() const
    {
        return state_.load(std::memory_order_acquire) == State::Initialized;
    }
}
