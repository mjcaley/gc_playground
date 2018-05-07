#pragma once

struct Collectable
{
    virtual ~Collectable() = default;

    Collectable* add_to_frame(Frame& frame)
    {
        frame.add_to_stack(this);
    }
    
    virtual std::vector<PointerBase*> get_pointers() = 0;
};
