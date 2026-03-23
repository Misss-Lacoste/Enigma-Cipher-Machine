#ifndef ROTOR_HPP
#define ROTOR_HPP

class Rotor {
private:
    int currentPosition; //current position(0–25)
    int rotorType;       //rotor type(1–5)
    char ring;           //ring setting

public:
    bool isInTurnoverPosition() const;
    void offset();
    void ringApply(char ringSetting);
    void substitute(char& ch, char direction);
    void setRotor(int type);
};

#endif
