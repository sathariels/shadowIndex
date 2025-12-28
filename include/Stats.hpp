#ifndef STATS_HPP
#define STATS_HPP

struct Stats {
    int resolve = 0;
    int dread = 0;
    int clarity = 0;
    int pride = 0;
    int compassion = 0;

    Stats operator+(const Stats& other) const {
        return {
            resolve + other.resolve,
            dread + other.dread,
            clarity + other.clarity,
            pride + other.pride,
            compassion + other.compassion
        };
    }
    
    Stats& operator+=(const Stats& other) {
        resolve += other.resolve;
        dread += other.dread;
        clarity += other.clarity;
        pride += other.pride;
        compassion += other.compassion;
        return *this;
    }
};

#endif // STATS_HPP
