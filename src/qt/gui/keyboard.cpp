#include "keyboard.h"

#include <algorithm>

#include <QDebug>

Keyboard::~Keyboard(){
    for (auto p : this->smallPress){ delete p; }
    for (auto p : this->mediumPress){ delete p; }
    for (auto p : this->spacePress){ delete p; }

    for (auto p : this->smallRelease){ delete p; }
    for (auto p : this->mediumRelease){ delete p; }
    for (auto p : this->spaceRelease){ delete p; }

    smallPress.clear();
    mediumPress.clear();
    spacePress.clear();
    smallRelease.clear();
    mediumRelease.clear();
    spaceRelease.clear();
}

QSound* Keyboard::getSound(std::vector<QSound*> &qsound, int &prIndex){
    // get random index from [ prIndex, qsound.size() )
    int randIndex = prIndex + std::rand() % (qsound.size() - prIndex);

    std::swap(qsound[prIndex], qsound[randIndex]);

    int i = prIndex;

    prIndex = ++prIndex % qsound.size();

    return qsound[i];
}

Keyboard::Keyboard(QString kbDir)
{
    const QString sm = kbDir + "small\\";
    const QString md = kbDir + "medium\\";
    const QString sp = kbDir + "space\\";

    const QString p = "keypress\\";
    const QString r = "keyrelease\\";

    for (int i = 0; i < 50; i++){
        const QString press = p + "keypress_" + QString::number(i) + ".wav";
        const QString release = r + "keyrelease_" + QString::number(i) + ".wav";

        QSound* smP = new QSound(sm + press);
        QSound* mdP = new QSound(md + press);
        QSound* spP = new QSound(sp + press);
        QSound* smR = new QSound(sm + release);
        QSound* mdR = new QSound(md + release);
        QSound* spR = new QSound(sp + release);

        this->smallPress.push_back(smP);
        this->mediumPress.push_back(mdP);
        this->spacePress.push_back(spP);

        this->smallRelease.push_back(smR);
        this->mediumRelease.push_back(mdR);
        this->spaceRelease.push_back(spR);
    }
}

void Keyboard::play(int scanCode, bool isPressed){
    /* Plays press or release sound of key from scanCode
     * scanCodes:
     *   medium sized keys:
     *     shift - 42 54
     *     enter - 28
     *     backspace - 14
     *     numpad0 - 82
     *     numpad+ - 78
     *   spacebar:
     *     space - 57
     */
    if (isPressed){
        if (
                this->heldScanCodes.find(scanCode) == this->heldScanCodes.end() ||
                !this->heldScanCodes[scanCode]
           ){
            this->heldScanCodes[scanCode] = true;
            switch(scanCode){
                case 42: case 54:
                case 28:
                case 14:
                case 82:
                case 78:
                    getSound(this->mediumPress, this->mPi)->play();
                    break;
                case 57:
                    getSound(this->spacePress, this->spPi)->play();
                    break;
                default:
                    getSound(this->smallPress, this->sPi)->play();
            }
        }
    } else {
        if (this->heldScanCodes.find(scanCode) != this->heldScanCodes.end() && this->heldScanCodes[scanCode]){
            this->heldScanCodes[scanCode] = false;
        }
        switch(scanCode){
            case 42: case 54:
            case 28:
            case 14:
            case 82:
            case 78:
                getSound(this->mediumRelease, this->spRi)->play();
                break;
            case 57:
                getSound(this->spaceRelease, this->mRi)->play();
                break;
            default:
                getSound(this->smallRelease, this->sRi)->play();
        }
    }
}
