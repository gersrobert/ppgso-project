#ifndef PPGSO_EXECUTABLE_H
#define PPGSO_EXECUTABLE_H

class Executable {
public:
    virtual bool execute(float delta) = 0;
};

#endif //PPGSO_EXECUTABLE_H
