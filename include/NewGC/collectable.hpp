#pragma once

struct Collectable
{
    virtual ~Collectable() = default;

    // Collectable* add_to_frame(Frame& frame)
    // {
    //     frame.add_to_stack(this);
    // }
    
    int current_mark { -1 };
    virtual void mark(int new_mark) = 0;
};
