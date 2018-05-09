#pragma once

struct Collectable
{
    Collectable() : current_mark(-1) {}
    virtual ~Collectable() = default;

    int current_mark { -1 };
    virtual void mark(int new_mark) = 0;
};
