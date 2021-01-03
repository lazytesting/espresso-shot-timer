#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

class PushButton {

public:
    PushButton(int pushState);
    void updateState (int state);
    bool isPushed ();

private: 
    int lastState;
    int currentState;
    int pushState;

};

#endif