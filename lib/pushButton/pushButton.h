#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

class PushButton {

public:
    PushButton() {}
    void updateState (int state);
    bool isPushed ();
};

#endif