#include "pulttime.h"
#include "ftd2xx.h"
#include <QDebug>

PultTime::PultTime(QObject* parent) : QObject(parent){
    process = true;
    state_green = false;
    state_red   = false;
    state_blue  = false;
}

void PultTime::StopProcess(){
    process = false;
}

void PultTime::stateGreen(bool s){
    if(!s)
        state_green = 2;
    else
        state_green = 0;
}

void PultTime::stateRed(bool s){
    if(!s)
        state_red = 1;
    else
        state_red = 0;
}

void PultTime::stateBlue(bool s){
    if(!s)
        state_blue = 4;
    else
        state_blue = 0;
}

void PultTime::Process(){
    FT_STATUS ftStatus;
    FT_HANDLE ftHandle;
    bool flag_red = false;
    bool flag_green = false;
    bool flag_blue = false;
    while(process){
        while(true){
            Sleep(1000);
            //qDebug()<<"connPult";
            ftStatus = FT_Open(0, &ftHandle);
            if (ftStatus == FT_OK) {
                ftStatus = FT_SetBitMode(ftHandle, 0xf0, 0x01);
                if (ftStatus == FT_OK) {
                    ftStatus = FT_SetBaudRate(ftHandle, 300); // Set baud rate to 9600
                    //if (ftStatus == FT_OK)
                    //    qDebug()<<"FT_SetBaudRate";
                    FT_SetTimeouts(ftHandle, 1000, 1000);
                    break;
                }
                FT_Close(ftHandle);
            }
            if(process == false)
                break;
        }

        while(true){
            char RxBuffer[256];
            DWORD BytesReceived;
            DWORD BytesWritten;
            ftStatus = FT_Read(ftHandle, &RxBuffer, 256, &BytesReceived);
            if (ftStatus == FT_OK) {
                int b = (int)(RxBuffer[255] & 0x07);

                int blue   = b & 0x01 ? 0 : 1;
                int green = b & 0x02 ? 0 : 1;
                int red  = b & 0x04 ? 0 : 1;
                //qDebug()<<"readPult";
                if(green){
                    if(!flag_green){
                        flag_green = true;
                        sigGreen();
                    }
                }else{
                    flag_green = false;
                }
                if(red){
                    if(!flag_red){
                        flag_red = true;
                        sigRed();
                    }
                }else{
                    flag_red = false;
                }
                if(blue){
                    if(!flag_blue){
                        flag_blue = true;
                        sigBlue();
                    }
                }else{
                    flag_blue = false;
                }
                int led;
                led =  (state_blue | state_green | state_red) <<  4;
                FT_Write(ftHandle, &led, 1, &BytesWritten);
            }else{
                FT_Close(ftHandle);
                //qDebug()<<"break";
                break;
            }
            if(process == false){
                FT_Close(ftHandle);
                break;
            }
        }

    }
}
