#include "pch.h"
#include "time_keeper.h"

TimeKeeper::TimeKeeper() = default;

TimeKeeper::~TimeKeeper() = default;

void TimeKeeper::SetTime(double time) {
    time_ = time;
}

double TimeKeeper::GetTime() {
    return time_;
}

double TimeKeeper::time_ = 0;
