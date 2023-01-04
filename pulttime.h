#ifndef PULTTIME_H
#define PULTTIME_H

#include <QObject>

class PultTime : public QObject
{
    Q_OBJECT
public:
    PultTime(QObject* parent = nullptr);
    void StopProcess(void);

public slots:
    void Process(void);
    void stateGreen(bool);
    void stateRed(bool);
    void stateBlue(bool);

private:
    bool process;
    int state_green;
    int state_red;
    int state_blue;

signals:
    void sigGreen(void);
    void sigRed(void);
    void sigBlue(void);

};

#endif // PULTTIME_H
