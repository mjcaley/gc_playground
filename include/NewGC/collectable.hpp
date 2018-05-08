#pragma once

struct Collectable
{
    virtual ~Collectable() = default;

    int current_mark { -1 };
    virtual void mark(int new_mark) = 0;
};
