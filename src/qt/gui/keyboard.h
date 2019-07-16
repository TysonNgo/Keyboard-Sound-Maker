#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QtMultimedia/QSound>
#include <QString>
#include <vector>
#include <map>

class Keyboard{
    private:
        std::map<int, bool> heldScanCodes;

        std::vector<QSound*> smallPress;
        std::vector<QSound*> mediumPress;
        std::vector<QSound*> spacePress;

        std::vector<QSound*> smallRelease;
        std::vector<QSound*> mediumRelease;
        std::vector<QSound*> spaceRelease;

        // press indexes;
        int sPi = 0;
        int mPi = 0;
        int spPi = 0;

        // release indexes;
        int sRi = 0;
        int mRi = 0;
        int spRi = 0;

        QSound* getSound(std::vector<QSound*> &qsound, int &prIndex);
    public:
        Keyboard(QString kbDir);
        ~Keyboard();
        void play(int scanCode, bool isPressed);

        static const bool RELEASED = false;
        static const bool PRESSED = true;
};

#endif // KEYBOARD_H
