#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QtMultimedia/QSound>
#include <QString>
#include <vector>
#include <map>

class Keyboard{
    private:
        QString kbSoundDir;

        std::map<int, bool> heldScanCodes;

        std::vector<QSound*> smallPress;
        std::vector<QSound*> mediumPress;
        std::vector<QSound*> spacePress;

        std::vector<QSound*> smallRelease;
        std::vector<QSound*> mediumRelease;
        std::vector<QSound*> spaceRelease;

        // press indexes;
        unsigned long long sPi = 0;
        unsigned long long mPi = 0;
        unsigned long long spPi = 0;

        // release indexes;
        unsigned long long sRi = 0;
        unsigned long long mRi = 0;
        unsigned long long spRi = 0;

        QSound* getSound(std::vector<QSound*> &qsound, unsigned long long &prIndex);
    public:
        Keyboard();
        Keyboard(QString kbDir);
        ~Keyboard();
        void loadSounds();
        void loadSounds(QString kbDir);
        void play(int scanCode, bool isPressed);

        static const bool RELEASED = false;
        static const bool PRESSED = true;
};

#endif // KEYBOARD_H
