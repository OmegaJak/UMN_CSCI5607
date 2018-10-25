#pragma once
class TimeKeeper {
   public:
    TimeKeeper();
    ~TimeKeeper();

    static void SetTime(double time);
    static double GetTime();

   private:
    static double time_;  // Should range from 0 to 1
};
