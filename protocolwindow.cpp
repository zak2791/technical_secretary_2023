#include "protocolwindow.h"
#include <QGridLayout>
#include <QBuffer>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "fight.h"
#include "pcscreen.h"

int currentX;

ProtocolWindow::ProtocolWindow(QWidget* p) : QWidget(p)
{
    currentX = 0;
    q = nullptr;
    address = "";
    mat = 0;
    vin = 0;                //0 - нет победителя, 1 - синий, 2 - красный

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("white"));
    setPalette(pal);
    setAutoFillBackground(true);

    lastItem = nullptr;
    pairedItem = nullptr;

    btnNk = new LeftRightPushButton("Нк");
    btnNk->setObjectName("NK");
    btnNk->setMinimumSize(30, 30);
    btnNk->setFocusPolicy(Qt::NoFocus);
    connect(btnNk, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnV = new LeftRightPushButton("V");
    btnV->setObjectName("V");
    btnV->setMinimumSize(30, 30);
    btnV->setFocusPolicy(Qt::NoFocus);
    connect(btnV, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnBP = new LeftRightPushButton("БП", this);
    btnBP->setObjectName("BP");
    btnBP->setMinimumSize(30, 30);
    btnBP->setFocusPolicy(Qt::NoFocus);
    connect(btnBP, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnUd = new LeftRightPushButton("Уд", this);
    btnUd->setObjectName("Ud");
    btnUd->setMinimumSize(30, 30);
    btnUd->setFocusPolicy(Qt::NoFocus);
    connect(btnUd, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnNkT = new LeftRightPushButton("Нк-Т", this);
    btnNkT->setObjectName("NKT");
    btnNkT->setMinimumSize(30, 30);
    btnNkT->setFocusPolicy(Qt::NoFocus);
    connect(btnNkT, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnYP = new LeftRightPushButton("ЯП", this);
    btnYP->setObjectName("YP");
    btnYP->setMinimumSize(30, 30);
    btnYP->setFocusPolicy(Qt::NoFocus);
    connect(btnYP, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnPT = new LeftRightPushButton("+", this);
    btnPT->setObjectName("PLUS");
    btnPT->setMinimumSize(30, 30);
    btnPT->setFocusPolicy(Qt::NoFocus);
    connect(btnPT, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnOPB = new LeftRightPushButton("ОПБ", this);
    btnOPB->setObjectName("OPB");
    btnOPB->setMinimumSize(30, 30);
    btnOPB->setFocusPolicy(Qt::NoFocus);
    connect(btnOPB, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnNPB = new LeftRightPushButton("НПБ", this);
    btnNPB->setObjectName("NPB");
    btnNPB->setMinimumSize(30, 30);
    btnNPB->setFocusPolicy(Qt::NoFocus);
    connect(btnNPB, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnNkd = new LeftRightPushButton("Нкд", this);
    btnNkd->setObjectName("Nkd");
    btnNkd->setMinimumSize(30, 30);
    btnNkd->setFocusPolicy(Qt::NoFocus);
    connect(btnNkd, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnTNk = new LeftRightPushButton("ТНк", this);
    btnTNk->setObjectName("TNK");
    btnTNk->setMinimumSize(30, 30);
    btnTNk->setFocusPolicy(Qt::NoFocus);
    connect(btnTNk, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnAr = new LeftRightPushButton("Ар", this);
    btnAr->setObjectName("AR");
    btnAr->setMinimumSize(30, 30);
    btnAr->setFocusPolicy(Qt::NoFocus);
    connect(btnAr, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnPs = new LeftRightPushButton("Пс", this);
    btnPs->setObjectName("PS");
    btnPs->setMinimumSize(30, 30);
    btnPs->setFocusPolicy(Qt::NoFocus);
    connect(btnPs, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnBack = new QPushButton("<-", this);
    btnNk->setObjectName("NK");     //?????????????
    btnBack->setMinimumSize(40, 40);
    btnBack->setFocusPolicy(Qt::NoFocus);
    connect(btnBack, SIGNAL(clicked()), this, SLOT(Back()));

    btn2 = new LeftRightPushButton("2", this);
    btn2->setObjectName("btn2");
    btn2->setMinimumSize(30, 30);
    btn2->setFocusPolicy(Qt::NoFocus);
    //btn2->setStyleSheet("background-color: red; color: white");
    connect(btn2, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn2_red = new LeftRightPushButton("2", this);
    btn2_red->setObjectName("btn2_red");
    btn2_red->setMinimumSize(30, 30);
    btn2_red->setFocusPolicy(Qt::NoFocus);
    btn2_red->setStyleSheet("background-color: red; color: white");
    connect(btn2_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn2_blue = new LeftRightPushButton("2", this);
    btn2_blue->setObjectName("btn2_blue");
    btn2_blue->setMinimumSize(30, 30);
    btn2_blue->setFocusPolicy(Qt::NoFocus);
    btn2_blue->setStyleSheet("background-color: blue; color: white");
    connect(btn2_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn1 = new LeftRightPushButton("1", this);
    btn1->setObjectName("btn1");
    btn1->setMinimumSize(30, 30);
    btn1->setFocusPolicy(Qt::NoFocus);
    //btn1->setStyleSheet("background-color: red; color: white");
    connect(btn1, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn1_red = new LeftRightPushButton("1", this);
    btn1_red->setObjectName("btn1_red");
    btn1_red->setMinimumSize(30, 30);
    btn1_red->setFocusPolicy(Qt::NoFocus);
    btn1_red->setStyleSheet("background-color: red; color: white");
    connect(btn1_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn1_blue = new LeftRightPushButton("1", this);
    btn1_blue->setObjectName("btn1_blue");
    btn1_blue->setMinimumSize(30, 30);
    btn1_blue->setFocusPolicy(Qt::NoFocus);
    btn1_blue->setStyleSheet("background-color: blue; color: white");
    connect(btn1_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    //btnA = new LeftRightPushButton("A", this);
    //btnA->setObjectName("btnA");
    //btnA->setMinimumSize(30, 30);
    //btnA->setFocusPolicy(Qt::NoFocus);
    //btnA->setStyleSheet("background-color: red; color: white");
    //connect(btnA, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));
    /*
    btnA_red = new LeftRightPushButton("A", this);
    btnA_red->setObjectName("btnA_red");
    btnA_red->setMinimumSize(30, 30);
    btnA_red->setFocusPolicy(Qt::NoFocus);
    btnA_red->setStyleSheet("background-color: red; color: white");
    connect(btnA_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnA_blue = new LeftRightPushButton("A", this);
    btnA_blue->setObjectName("btnA_blue");
    btnA_blue->setMinimumSize(30, 30);
    btnA_blue->setFocusPolicy(Qt::NoFocus);
    btnA_blue->setStyleSheet("background-color: blue; color: white");
    connect(btnA_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));
    */
    btn2P = new LeftRightPushButton("", this);
    btn2P->setObjectName("btn2P");
    btn2P->setIcon(QIcon("A22.png"));
    //btn2P->setIconSize(QSize(96, 96));
    btn2P->setMinimumSize(30, 30);
    //btn2P->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    btn2P->setFocusPolicy(Qt::NoFocus);
    //btn2P->setStyleSheet("background-color: red; color: white");
    connect(btn2P, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn2P_red = new LeftRightPushButton("", this);
    btn2P_red->setObjectName("btn2P_red");
    btn2P_red->setIcon(QIcon("A2.png"));
    btn2P_red->setMinimumSize(30, 30);
    btn2P_red->setFocusPolicy(Qt::NoFocus);
    btn2P_red->setStyleSheet("background-color: red; color: white");
    connect(btn2P_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn2P_blue = new LeftRightPushButton("", this);
    btn2P_blue->setObjectName("btn2P_blue");
    btn2P_blue->setIcon(QIcon("A2.png"));
    btn2P_blue->setMinimumSize(30, 30);
    btn2P_blue->setFocusPolicy(Qt::NoFocus);
    btn2P_blue->setStyleSheet("background-color: blue; color: white");
    connect(btn2P_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn1P = new LeftRightPushButton("", this);
    btn1P->setObjectName("btn1P");
    btn1P->setIcon(QIcon("A12.png"));
    btn1P->setMinimumSize(30, 30);
    btn1P->setFocusPolicy(Qt::NoFocus);
    //btn1P->setStyleSheet("background-color: red; color: white");
    connect(btn1P, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn1P_red = new LeftRightPushButton("", this);
    btn1P_red->setObjectName("btn1P_red");
    btn1P_red->setIcon(QIcon("A1.png"));
    btn1P_red->setMinimumSize(30, 30);
    btn1P_red->setFocusPolicy(Qt::NoFocus);
    btn1P_red->setStyleSheet("background-color: red; color: white");
    connect(btn1P_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btn1P_blue = new LeftRightPushButton("", this);
    btn1P_blue->setObjectName("btn1P_blue");
    btn1P_blue->setIcon(QIcon("A1.png"));
    btn1P_blue->setMinimumSize(30, 30);
    btn1P_blue->setFocusPolicy(Qt::NoFocus);
    btn1P_blue->setStyleSheet("background-color: blue; color: white");
    connect(btn1P_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    /*
    btnAP = new LeftRightPushButton("", this);
    btnAP->setObjectName("btnAP");
    btnAP->setIcon(QIcon("AA2.png"));
    btnAP->setMinimumSize(30, 30);
    btnAP->setFocusPolicy(Qt::NoFocus);
    //btnAP->setStyleSheet("background-color: blue; color: white");
    connect(btnAP, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnAP_red = new LeftRightPushButton("", this);
    btnAP_red->setObjectName("btnAP_red");
    btnAP_red->setIcon(QIcon("AA.png"));
    btnAP_red->setMinimumSize(30, 30);
    btnAP_red->setFocusPolicy(Qt::NoFocus);
    btnAP_red->setStyleSheet("background-color: red; color: white");
    connect(btnAP_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnAP_blue = new LeftRightPushButton("", this);
    btnAP_blue->setObjectName("btnAP_blue");
    btnAP_blue->setIcon(QIcon("AA.png"));
    btnAP_blue->setMinimumSize(30, 30);
    btnAP_blue->setFocusPolicy(Qt::NoFocus);
    btnAP_blue->setStyleSheet("background-color: blue; color: white");
    connect(btnAP_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));
    */

    btnZv = new LeftRightPushButton("Зв", this);
    btnZv->setObjectName("btnZv");
    btnZv->setMinimumSize(30, 30);
    btnZv->setFocusPolicy(Qt::NoFocus);
    //btnZv->setStyleSheet("background-color: red; color: white");
    connect(btnZv, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnZv_red = new LeftRightPushButton("Зв", this);
    btnZv_red->setObjectName("btnZv_red");
    btnZv_red->setMinimumSize(30, 30);
    btnZv_red->setFocusPolicy(Qt::NoFocus);
    btnZv_red->setStyleSheet("background-color: red; color: white");
    connect(btnZv_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnZv_blue = new LeftRightPushButton("Зв", this);
    btnZv_blue->setObjectName("btnZv_blue");
    btnZv_blue->setMinimumSize(30, 30);
    btnZv_blue->setFocusPolicy(Qt::NoFocus);
    btnZv_blue->setStyleSheet("background-color: blue; color: white");
    connect(btnZv_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnVyh = new LeftRightPushButton("В", this);
    btnVyh->setObjectName("btnVyh");
    btnVyh->setMinimumSize(30, 30);
    btnVyh->setFocusPolicy(Qt::NoFocus);
    //btnVyh->setStyleSheet("background-color: red; color: white");
    connect(btnVyh, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnVyh_red = new LeftRightPushButton("В", this);
    btnVyh_red->setObjectName("btnVyh_red");
    btnVyh_red->setMinimumSize(30, 30);
    btnVyh_red->setFocusPolicy(Qt::NoFocus);
    btnVyh_red->setStyleSheet("background-color: red; color: white");
    connect(btnVyh_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnVyh_blue = new LeftRightPushButton("В", this);
    btnVyh_blue->setObjectName("btnVyh_blue");
    btnVyh_blue->setMinimumSize(30, 30);
    btnVyh_blue->setFocusPolicy(Qt::NoFocus);
    btnVyh_blue->setStyleSheet("background-color: blue; color: white");
    connect(btnVyh_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnZp = new LeftRightPushButton("Зп", this);
    btnZp->setObjectName("btnZp");
    btnZp->setMinimumSize(30, 30);
    btnZp->setFocusPolicy(Qt::NoFocus);
    //btnZp->setStyleSheet("background-color: red; color: white");
    connect(btnZp, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnZp_red = new LeftRightPushButton("Зп", this);
    btnZp_red->setObjectName("btnZp_red");
    btnZp_red->setMinimumSize(30, 30);
    btnZp_red->setFocusPolicy(Qt::NoFocus);
    btnZp_red->setStyleSheet("background-color: red; color: white");
    connect(btnZp_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnZp_blue = new LeftRightPushButton("Зп", this);
    btnZp_blue->setObjectName("btnZp_blue");
    btnZp_blue->setMinimumSize(30, 30);
    btnZp_blue->setFocusPolicy(Qt::NoFocus);
    btnZp_blue->setStyleSheet("background-color: blue; color: white");
    connect(btnZp_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP1 = new LeftRightPushButton("П1", this);
    btnP1->setObjectName("btnP1");
    btnP1->setMinimumSize(30, 30);
    btnP1->setFocusPolicy(Qt::NoFocus);
    //btnP1->setStyleSheet("background-color: red; color: white");
    connect(btnP1, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP1_red = new LeftRightPushButton("П1", this);
    btnP1_red->setObjectName("btnP1_red");
    btnP1_red->setMinimumSize(30, 30);
    btnP1_red->setFocusPolicy(Qt::NoFocus);
    btnP1_red->setStyleSheet("background-color: red; color: white");
    connect(btnP1_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP1_blue = new LeftRightPushButton("П1", this);
    btnP1_blue->setObjectName("btnP1_blue");
    btnP1_blue->setMinimumSize(30, 30);
    btnP1_blue->setFocusPolicy(Qt::NoFocus);
    btnP1_blue->setStyleSheet("background-color: blue; color: white");
    connect(btnP1_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP2 = new LeftRightPushButton("П2", this);
    btnP2->setObjectName("btnP2");
    btnP2->setMinimumSize(30, 30);
    btnP2->setFocusPolicy(Qt::NoFocus);
    //btnP2->setStyleSheet("background-color: red; color: white");
    connect(btnP2, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP3 = new LeftRightPushButton("П3", this);
    btnP3->setObjectName("btnP3");
    btnP3->setMinimumSize(30, 30);
    btnP3->setFocusPolicy(Qt::NoFocus);
    //btnP2->setStyleSheet("background-color: red; color: white");
    connect(btnP3, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP2_red = new LeftRightPushButton("П2", this);
    btnP2_red->setObjectName("btnP2_red");
    btnP2_red->setMinimumSize(30, 30);
    btnP2_red->setFocusPolicy(Qt::NoFocus);
    btnP2_red->setStyleSheet("background-color: red; color: white");
    connect(btnP2_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP2_blue = new LeftRightPushButton("П2", this);
    btnP2_blue->setObjectName("btnP2_blue");
    btnP2_blue->setMinimumSize(30, 30);
    btnP2_blue->setFocusPolicy(Qt::NoFocus);
    btnP2_blue->setStyleSheet("background-color: blue; color: white");
    connect(btnP2_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP3_red = new LeftRightPushButton("П3", this);
    btnP3_red->setObjectName("btnP3_red");
    btnP3_red->setMinimumSize(30, 30);
    btnP3_red->setFocusPolicy(Qt::NoFocus);
    btnP3_red->setStyleSheet("background-color: red; color: white");
    connect(btnP3_red, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnP3_blue = new LeftRightPushButton("П3", this);
    btnP3_blue->setObjectName("btnP3_blue");
    btnP3_blue->setMinimumSize(30, 30);
    btnP3_blue->setFocusPolicy(Qt::NoFocus);
    btnP3_blue->setStyleSheet("background-color: blue; color: white");
    connect(btnP3_blue, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnNy = new LeftRightPushButton("н/я", this);
    btnNy->setObjectName("NY");
    btnNy->setMinimumSize(30, 30);
    btnNy->setFocusPolicy(Qt::NoFocus);
    connect(btnNy, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnDoc = new LeftRightPushButton("сн.вр", this);
    btnDoc->setObjectName("DOCT");
    btnDoc->setMinimumSize(30, 30);
    btnDoc->setFocusPolicy(Qt::NoFocus);
    connect(btnDoc, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    btnVin = new LeftRightPushButton("VIN", this);
    btnVin->setObjectName("VIN");
    btnVin->setMinimumSize(30, 30);
    btnVin->setFocusPolicy(Qt::NoFocus);
    connect(btnVin, SIGNAL(left_right_clicked(bool, QString)), this, SLOT(Rate(bool, QString)));

    BallsRed = new QLabel("0");
    BallsRed->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    BallsRed->setStyleSheet("color:black; font-size: 20px; border-style:solid; border-left:none;"
                                        "border-width:1px; border-color:black");
    BallsBlue = new QLabel("0");
    BallsBlue->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    BallsBlue->setStyleSheet("color:black; font-size: 20px;border-style:solid; border-top:none;"
                                         "border-left:none; border-width:1px; border-color:black");

    AktRed = new QLabel("0");
    AktRed->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    AktRed->setStyleSheet("color:black; font-size: 16px; border-style:solid; border-top:none;"
                                      "border-left:none; border-width:1px; border-color:black");
    AktBlue = new QLabel("0");
    AktBlue->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    AktBlue->setStyleSheet("color:black; font-size: 16px; border-style:solid; border-top:none;"
                                       "border-left:none; border-width:1px; border-color:black");

    ResultRed = new QLabel("");
    ResultRed->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ResultRed->setStyleSheet("color:black; font-size: 20px; border-style:solid;"
                                       "border-left:none; border-width:1px; border-color:black");
    ResultBlue = new QLabel("");
    ResultBlue->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ResultBlue->setStyleSheet("color:black; font-size: 20px; border-style:solid; border-top:none;"
                                       "border-left:none; border-width:1px; border-color:black");

    QLabel lblRef("Судьи");
    lblRef.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lblRef.setStyleSheet("border-style:solid; border-left:none;"
                                       "border-bottom:none; border-width:1px; border-color:black");

    nameMain = new QLabel("рук. ковра");
    nameMain->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    nameMain->setStyleSheet("border-style:solid; border-left:none; border-top: none;"
                                       "border-bottom:none; border-width:1px; border-color:black");

    nameRef = new QLabel("рефери");
    nameRef->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    nameRef->setStyleSheet("border-style:solid; border-left:none; border-top: none;"
                                       "border-bottom:none; border-width:1px; border-color:black");

    nameSide = new QLabel("боковой");
    nameSide->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    nameSide->setStyleSheet("border-style:solid; border-left:none; border-top: none;"
                                       "border-bottom:none; border-width:1px; border-color:black");

    QLabel lblTime("Время");
    lblTime.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lblTime.setStyleSheet("border-style:solid; border-left:none;"
                                       "border-bottom:none; border-width:1px; border-color:black");
    time = new QLabel("0:00");
    time->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    time->setStyleSheet("border-style:solid; border-left:none;"
                        "border-top:none; border-width:1px; border-color:black; font-size: 20px;");

    itemNameRed = new ProtocolNames();
    itemNameRed->setName("");
    sceneNameRed = new QGraphicsScene();
    sportsmenRed = new QGraphicsView(sceneNameRed);
    sportsmenRed->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sportsmenRed->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sceneNameRed->addItem(itemNameRed);

    itemNameBlue = new ProtocolNames();
    itemNameBlue->setName("");
    sceneNameBlue = new QGraphicsScene();
    sportsmenBlue = new QGraphicsView(sceneNameBlue);
    sportsmenBlue->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sportsmenBlue->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sceneNameBlue->addItem(itemNameBlue);

    sceneBlue = new QGraphicsScene();
    sceneRed = new QGraphicsScene();

    rateRed = new MyGraphicsView(&currentX);
    rateRed->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rateRed->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rateRed->setMinimumHeight(20);
    rateRed->setStyleSheet("border-style:solid; border-width:1px; border-color:black");
    rateRed->setScene(sceneRed);

    rateBlue = new MyGraphicsView(&currentX);
    rateBlue->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rateBlue->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rateBlue->setMinimumHeight(20);
    rateBlue->setStyleSheet("border-style:solid; border-top:none; border-width:1px; border-color:black;");
    rateBlue->setScene(sceneBlue);


    //self.margin = 6

    //QGridLayout grid(this);
    spacing = 6;
    margin = 6;

    grid.setSpacing(6);
    grid.setMargin(2);

    grid.addWidget(btnNk,         0,  0, 3, 3);
    grid.addWidget(btnV,          0,  3, 3, 3);
    grid.addWidget(btnBP,         0,  6, 3, 3);
    grid.addWidget(btnUd,         0,  9, 3, 3);
    grid.addWidget(btnNkT,        0, 12, 3, 3);
    grid.addWidget(btnYP,         0, 15, 3, 3);
    grid.addWidget(btnPT,         0, 18, 3, 3);
    grid.addWidget(btnDoc,        0, 21, 3, 3);

    grid.addWidget(btnOPB,        3,  0, 3, 3);
    grid.addWidget(btnNPB,        3,  3, 3, 3);
    grid.addWidget(btnNkd,        3,  6, 3, 3);
    grid.addWidget(btnTNk,        3,  9, 3, 3);
    grid.addWidget(btnAr,         3, 12, 3, 3);
    grid.addWidget(btnPs,         3, 15, 3, 3);
    grid.addWidget(btnNy,         3, 18, 3, 3);
    grid.addWidget(btnVin,        3, 21, 3, 3);

    grid.addWidget(btn2,          1, 24, 4, 4);
    grid.addWidget(btn2_red,      0, 24, 3, 4);
    grid.addWidget(btn2_blue,     3, 24, 3, 4);
    grid.addWidget(btn1,          1, 28, 4, 4);
    grid.addWidget(btn1_red,      0, 28, 3, 4);
    grid.addWidget(btn1_blue,     3, 28, 3, 4);
    //grid.addWidget(btnA,          1, 32, 4, 4);
    //grid.addWidget(btnA_red,      0, 32, 3, 4);
    //grid.addWidget(btnA_blue,     3, 32, 3, 4);
    grid.addWidget(btn2P,         1, 32, 4, 4);
    grid.addWidget(btn2P_red,     0, 32, 3, 4);
    grid.addWidget(btn2P_blue,    3, 32, 3, 4);
    grid.addWidget(btn1P,         1, 36, 4, 4);
    grid.addWidget(btn1P_red,     0, 36, 3, 4);
    grid.addWidget(btn1P_blue,    3, 36, 3, 4);
    //grid.addWidget(btnAP,         1, 44, 4, 4);
    //grid.addWidget(btnAP_red,     0, 44, 3, 4);
    //grid.addWidget(btnAP_blue,    3, 44, 3, 4);
    grid.addWidget(btnZv,         1, 40, 4, 4);
    grid.addWidget(btnZv_red,     0, 40, 3, 4);
    grid.addWidget(btnZv_blue,    3, 40, 3, 4);
    grid.addWidget(btnVyh,        1, 44, 4, 4);
    grid.addWidget(btnVyh_red,    0, 44, 3, 4);
    grid.addWidget(btnVyh_blue,   3, 44, 3, 4);
    grid.addWidget(btnZp,         1, 48, 4, 4);
    grid.addWidget(btnZp_red,     0, 48, 3, 4);
    grid.addWidget(btnZp_blue,    3, 48, 3, 4);
    grid.addWidget(btnP1,         1, 52, 4, 4);
    grid.addWidget(btnP1_red,     0, 52, 3, 4);
    grid.addWidget(btnP1_blue,    3, 52, 3, 4);
    grid.addWidget(btnP2,         1, 56, 4, 4);
    grid.addWidget(btnP2_red,     0, 56, 3, 4);
    grid.addWidget(btnP2_blue,    3, 56, 3, 4);
    grid.addWidget(btnP3,         1, 60, 4, 4);
    grid.addWidget(btnP3_red,     0, 60, 3, 4);
    grid.addWidget(btnP3_blue,    3, 60, 3, 4);
    grid.addWidget(btnBack,       0, 64, 6, 8);
    grid.addWidget(sportsmenRed,  6, 0, 4, 10);
    grid.addWidget(sportsmenBlue, 10, 0, 4, 10);
    grid.addWidget(rateRed,       6, 10, 4, 53);
    grid.addWidget(rateBlue,      10, 10, 4, 53);
    grid.addWidget(BallsRed,      6,  63, 2, 2);
    grid.addWidget(AktRed,        8,  63, 2, 2);
    grid.addWidget(BallsBlue,     10, 63, 2, 2);
    grid.addWidget(AktBlue,       12, 63, 2, 2);
    grid.addWidget(ResultRed,     6,  65, 4, 2);
    grid.addWidget(ResultBlue,    10, 65, 4, 2);

    grid.addWidget(&lblRef,        6, 67, 1, 5);
    grid.addWidget(nameMain,       7, 67, 1, 5);
    grid.addWidget(nameRef,        8, 67, 1, 5);
    grid.addWidget(nameSide,       9, 67, 1, 5);
    grid.addWidget(&lblTime,      10, 67, 1, 5);
    grid.addWidget(time,          11, 67, 3, 5);

    setLayout(&grid);

    QFont font;
    connect(rateRed, SIGNAL(shift(int, int)), this, SLOT(shiftBlue(int, int)));
    connect(rateBlue, SIGNAL(shift(int, int)), this, SLOT(shiftRed(int, int)));

    connect(rateRed, SIGNAL(strike(int, int)), this, SLOT(strikeBlue(int, int)));
    connect(rateBlue, SIGNAL(strike(int, int)), this, SLOT(strikeRed(int, int)));

    connect(p, SIGNAL(sendKeyMode(int)), this, SLOT(keyMode(int)));

    num_fight = NULL;

    keyMode(0);
}

void ProtocolWindow::keyMode(int mode){
    if(mode){
        btn1->setVisible(false);
        btn1_red->setVisible(true);
        btn1_blue->setVisible(true);
        btn2->setVisible(false);
        btn2_red->setVisible(true);
        btn2_blue->setVisible(true);
        btn1P->setVisible(false);
        btn1P_red->setVisible(true);
        btn1P_blue->setVisible(true);
        btn2P->setVisible(false);
        btn2P_red->setVisible(true);
        btn2P_blue->setVisible(true);
        //btnA->setVisible(false);
        //btnA_red->setVisible(true);
        //btnA_blue->setVisible(true);
        //btnAP->setVisible(false);
        //btnAP_red->setVisible(true);
        //btnAP_blue->setVisible(true);
        btnP1->setVisible(false);
        btnP1_red->setVisible(true);
        btnP1_blue->setVisible(true);
        btnP2->setVisible(false);
        btnP2_red->setVisible(true);
        btnP2_blue->setVisible(true);
        btnP3->setVisible(false);
        btnP3_red->setVisible(true);
        btnP3_blue->setVisible(true);
        btnZp->setVisible(false);
        btnZp_red->setVisible(true);
        btnZp_blue->setVisible(true);
        btnZv->setVisible(false);
        btnZv_red->setVisible(true);
        btnZv_blue->setVisible(true);
        btnVyh->setVisible(false);
        btnVyh_red->setVisible(true);
        btnVyh_blue->setVisible(true);
    }else{
        btn1->setVisible(true);
        btn1_red->setVisible(false);
        btn1_blue->setVisible(false);
        btn2->setVisible(true);
        btn2_red->setVisible(false);
        btn2_blue->setVisible(false);
        btn1P->setVisible(true);
        btn1P_red->setVisible(false);
        btn1P_blue->setVisible(false);
        btn2P->setVisible(true);
        btn2P_red->setVisible(false);
        btn2P_blue->setVisible(false);
        //btnA->setVisible(true);
        //btnA_red->setVisible(false);
        //btnA_blue->setVisible(false);
        //btnAP->setVisible(true);
        //btnAP_red->setVisible(false);
        //btnAP_blue->setVisible(false);
        btnP1->setVisible(true);
        btnP1_red->setVisible(false);
        btnP1_blue->setVisible(false);
        btnP2->setVisible(true);
        btnP2_red->setVisible(false);
        btnP2_blue->setVisible(false);
        btnP3->setVisible(true);
        btnP3_red->setVisible(false);
        btnP3_blue->setVisible(false);
        btnZp->setVisible(true);
        btnZp_red->setVisible(false);
        btnZp_blue->setVisible(false);
        btnZv->setVisible(true);
        btnZv_red->setVisible(false);
        btnZv_blue->setVisible(false);
        btnVyh->setVisible(true);
        btnVyh_red->setVisible(false);
        btnVyh_blue->setVisible(false);
    }
}

void ProtocolWindow::setMat(QString _mat){
    mat = _mat.toInt() * 2000 + 3;
}

void ProtocolWindow::setAddr(int, QString addr){
    if(addr != "")
        address = addr;
    //qDebug()<<"setAddress"<<address;
}

void ProtocolWindow::Rate(bool _button, QString name){
    //if(num_fight == NULL)
    //    return;
    bool button = _button;
    pairedItem = nullptr;
    ProtocolRates* item = nullptr;
    if(name == "btn1_red"){
        item = new ProtocolRates("1", 1, false, false);
        item->setSize(rateRed->height()/5.8, rateRed->height()/3);
        button = false;
    }else if(name == "btn1_blue"){
        item = new ProtocolRates("1", 1, false, false);
        item->setSize(rateRed->height()/5.8, rateRed->height()/3);
        button = true;
    }else if(name == "btn1"){
        item = new ProtocolRates("1", 1, false, false);
        item->setSize(rateRed->height()/5.8, rateRed->height()/3);
    //}else if(name == "btnA_red"){
    //    item = new ProtocolRates("A", "A", false, false);
    //    item->setSize(rateRed->height()/3.4, rateRed->height()/3);
    //    button = false;
    //}else if(name == "btnA_blue"){
    //    item = new ProtocolRates("A", "A", false, false);
    //    item->setSize(rateRed->height()/3.4, rateRed->height()/3);
    //    button = true;
    //}else if(name == "btnA"){
    //    item = new ProtocolRates("A", "A", false, false);
    //    item->setSize(rateRed->height()/3.4, rateRed->height()/3);
    }else if(name == "NK"){
        item = new ProtocolRates("V", "V", false, false);
        item->setSize(rateRed->height()/2.4, rateRed->height()/3);
        pairedItem = new ProtocolRates("Нк", 0, false, true);
        pairedItem->setSize(rateRed->height()/2.4, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "NY"){
        item = new ProtocolRates("н/я", 0);
        item->setSize(rateRed->height()/2.1, rateRed->height()/3);
    }else if(name == "V"){
        item = new ProtocolRates("V", "V", false, false);
        item->setSize(rateRed->height()/3.4, rateRed->height()/3);
    }else if(name == "BP"){
        item = new ProtocolRates("V", "V", false, false);
        item->setSize(rateRed->height()/2.1, rateRed->height()/3);
        pairedItem = new ProtocolRates("БП", 0, false, true);
        pairedItem->setSize(rateRed->height()/2.1, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "Ud"){
        item = new ProtocolRates("V", "V", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
        pairedItem = new ProtocolRates("Уд", 0, false, true);
        pairedItem->setSize(rateRed->height()/2.3, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "NKT"){
        item = new ProtocolRates("V", "V", false, false);
        item->setSize(rateRed->height()/1.35, rateRed->height()/3);
        pairedItem = new ProtocolRates("Нк-Т", 0, false, true);
        pairedItem->setSize(rateRed->height()/1.35, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "DOCT"){
        item = new ProtocolRates("сн.вр", 0);
        item->setSize(rateRed->height()/1.28, rateRed->height()/3);
    }else if(name == "YP"){
        item = new ProtocolRates("V", "V", false, false);
        item->setSize(rateRed->height()/2.1, rateRed->height()/3);
        pairedItem = new ProtocolRates("ЯП", 0, false, true);
        pairedItem->setSize(rateRed->height()/2.1, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "PLUS"){
        item = new ProtocolRates("+", "+", false, false);
        item->setSize(rateRed->height()/4.0, rateRed->height()/3);
    }else if(name == "OPB"){
        item = new ProtocolRates("ОПБ", 0);
        item->setSize(rateRed->height()/1.4, rateRed->height()/3);
    }else if(name == "NPB"){
        item = new ProtocolRates("НПБ", 0);
        item->setSize(rateRed->height()/1.4, rateRed->height()/3);
    }else if(name == "Nkd"){
        item = new ProtocolRates("Нкд", 0);
        item->setSize(rateRed->height()/1.7, rateRed->height()/3);
        pairedItem = new ProtocolRates("2", 2, false, true);
        pairedItem->setSize(rateRed->height()/4.9, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "TNK"){
        item = new ProtocolRates("V", "V", false, false);
        item->setSize(rateRed->height()/1.6, rateRed->height()/3);
        pairedItem = new ProtocolRates("ТНк", 0, false, true);
        pairedItem->setSize(rateRed->height()/1.6, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "AR"){
        item = new ProtocolRates("Ар", "A", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
    }else if(name == "PS"){
        item = new ProtocolRates("Пс", 0);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
    }else if(name == "btn2"){
        item = new ProtocolRates("2", 2, false, false);
        item->setSize(rateRed->height()/4.9, rateRed->height()/3);
    }else if(name == "btn2_red"){
        item = new ProtocolRates("2", 2, false, false);
        item->setSize(rateRed->height()/4.9, rateRed->height()/3);
        button = false;
    }else if(name == "btn2_blue"){
        item = new ProtocolRates("2", 2, false, false);
        item->setSize(rateRed->height()/4.9, rateRed->height()/3);
        button = true;
    }else if(name == "btn2P"){
        item = new ProtocolRates("2", 2, true, false);
        item->setSize(rateRed->height()/3.0, rateRed->height()/3);
    }else if(name == "btn2P_red"){
        item = new ProtocolRates("2", 2, true, false);
        item->setSize(rateRed->height()/3.0, rateRed->height()/3);
        button = false;
    }else if(name == "btn2P_blue"){
        item = new ProtocolRates("2", 2, true, false);
        item->setSize(rateRed->height()/3.0, rateRed->height()/3);
        button = true;
    }else if(name == "btn1P"){
        item = new ProtocolRates("1", 1, true, false);
        item->setSize(rateRed->height()/3, rateRed->height()/3);
    }else if(name == "btn1P_red"){
        item = new ProtocolRates("1", 1, true, false);
        item->setSize(rateRed->height()/3, rateRed->height()/3);
        button = false;
    }else if(name == "btn1P_blue"){
        item = new ProtocolRates("1", 1, true, false);
        item->setSize(rateRed->height()/3, rateRed->height()/3);
        button = true;
    //}else if(name == "btnAP"){
    //    item = new ProtocolRates("A", "A", true, false);
    //    item->setSize(rateRed->height()/3, rateRed->height()/3);
    //}else if(name == "btnAP_red"){
    //    item = new ProtocolRates("A", "A", true, false);
    //    item->setSize(rateRed->height()/3, rateRed->height()/3);
    //    button = false;
    //}else if(name == "btnAP_blue"){
    //    item = new ProtocolRates("A", "A", true, false);
    //    item->setSize(rateRed->height()/3, rateRed->height()/3);
    //    button = true;
    }else if(name == "btnZv"){
        item = new ProtocolRates("Зв", "ЗВ", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
    }else if(name == "btnZv_red"){
        item = new ProtocolRates("Зв", "ЗВ", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
        button = false;
    }else if(name == "btnZv_blue"){
        item = new ProtocolRates("Зв", "ЗВ", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
        button = true;
    }else if(name == "btnVyh"){
        pairedItem = new ProtocolRates("1", 1, false, true);
        item = new ProtocolRates("В", "В", false, false);
        item->setSize(rateRed->height()/3.0, rateRed->height()/3);
        pairedItem->setSize(rateRed->height()/5.8, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "btnVyh_red"){
        pairedItem = new ProtocolRates("1", 1, false, true);
        item = new ProtocolRates("В", "В", false, false);
        item->setSize(rateRed->height()/3.0, rateRed->height()/3);
        pairedItem->setSize(rateRed->height()/5.8, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
        button = false;
    }else if(name == "btnVyh_blue"){
        pairedItem = new ProtocolRates("1", 1, false, true);
        item = new ProtocolRates("В", "В", false, false);
        item->setSize(rateRed->height()/3.0, rateRed->height()/3);
        pairedItem->setSize(rateRed->height()/5.8, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
        button = true;
    }else if(name == "btnZp"){
        item = new ProtocolRates("Зп", "ЗП", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
    }else if(name == "btnZp_red"){
        item = new ProtocolRates("Зп", "ЗП", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
        button = false;
    }else if(name == "btnZp_blue"){
        item = new ProtocolRates("Зп", "ЗП", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
        button = true;
    }else if(name == "btnP1_red"){
        pairedItem = new ProtocolRates("1", 1, false, true);
        item = new ProtocolRates("П1", "П1", false, false);
        item->setSize(rateRed->height()/2.4, rateRed->height()/3);
        button = false;
        pairedItem->setSize(rateRed->height()/5.8, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "btnP1"){
        pairedItem = new ProtocolRates("1", 1, false, true);
        item = new ProtocolRates("П1", "П1", false, false);
        item->setSize(rateRed->height()/2.4, rateRed->height()/3);
        pairedItem->setSize(rateRed->height()/5.8, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "btnP1_blue"){
        pairedItem = new ProtocolRates("1", 1, false, true);
        item = new ProtocolRates("П1", "П1", false, false);
        item->setSize(rateRed->height()/2.4, rateRed->height()/3);
        button = true;
        pairedItem->setSize(rateRed->height()/5.8, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
    }else if(name == "btnP2"){
       pairedItem = new ProtocolRates("1", 1, false, true);
       item = new ProtocolRates("П2", "П2", false, false);
       item->setSize(rateRed->height()/2.3, rateRed->height()/3);
       pairedItem->setSize(rateRed->height()/4.9, rateRed->height()/3);
       pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
     }else if(name == "btnP2_red"){
        pairedItem = new ProtocolRates("1", 1, false, true);
        item = new ProtocolRates("П2", "П2", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
        pairedItem->setSize(rateRed->height()/4.9, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
        button = false;
    }else if(name == "btnP2_blue"){
       pairedItem = new ProtocolRates("1", 1, false, true);
       item = new ProtocolRates("П2", "П2", false, false);
       item->setSize(rateRed->height()/2.3, rateRed->height()/3);
       pairedItem->setSize(rateRed->height()/4.9, rateRed->height()/3);
       pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
        button = true;
    }else if(name == "btnP3"){
       pairedItem = new ProtocolRates("1", 1, false, true);
       item = new ProtocolRates("П3", "П3", false, false);
       item->setSize(rateRed->height()/2.3, rateRed->height()/3);
       pairedItem->setSize(rateRed->height()/4.9, rateRed->height()/3);
       pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
     }else if(name == "btnP3_red"){
        pairedItem = new ProtocolRates("1", 1, false, true);
        item = new ProtocolRates("П3", "П3", false, false);
        item->setSize(rateRed->height()/2.3, rateRed->height()/3);
        pairedItem->setSize(rateRed->height()/4.9, rateRed->height()/3);
        pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
        button = false;
    }else if(name == "btnP3_blue"){
       pairedItem = new ProtocolRates("1", 1, false, true);
       item = new ProtocolRates("П3", "П3", false, false);
       item->setSize(rateRed->height()/2.3, rateRed->height()/3);
       pairedItem->setSize(rateRed->height()/4.9, rateRed->height()/3);
       pairedItem->setPos(currentX + item->w / 2, rateRed->height()/2);
        button = true;
     }else if(name == "VIN"){
        if(button){
            if(vin == 1)
                vin = 0;
            else
                vin = 1;
        }else{
            if(vin == 2)
                vin = 0;
            else
                vin = 2;
        }
        calculation();
        rate_to_png();
        return;
    }

    item->setPos(currentX + item->w / 2, rateRed->height()/2);
    currentX += item->w;
    lastItem = item;

    if(button){
        sceneBlue->addItem(item);
        if(pairedItem != nullptr)
            sceneRed->addItem(pairedItem);
    }else{
        sceneRed->addItem(item);
        if(pairedItem != nullptr)
            sceneBlue->addItem(pairedItem);
    }

    if(calculation())
        rate_to_png();
}

void ProtocolWindow::Back(){
    if(lastItem != nullptr){
        QGraphicsScene* scene = lastItem->scene();
        currentX -= lastItem->w / 1;
        scene->removeItem(lastItem);
        lastItem = nullptr;
        if(pairedItem != nullptr){
            QGraphicsScene* paired_scene = pairedItem->scene();
            paired_scene->removeItem(pairedItem);
            pairedItem = nullptr;
        }
        calculation();
    }
}

void ProtocolWindow::shiftBlue(int shift, int x){
    QList<QGraphicsItem*> l = rateBlue->items(x, 0, rateBlue->scene()->width() - x, rateBlue->scene()->height(), Qt::IntersectsItemShape);
    for(int i = 0; i < l.length(); i++)
        l.at(i)->setPos(l.at(i)->pos().x() + shift, l.at(i)->pos().y());
    calculation();
}

void ProtocolWindow::shiftRed(int shift, int x){
    QList<QGraphicsItem*> l = rateRed->items(x, 0, rateRed->scene()->width() - x, rateRed->scene()->height(), Qt::IntersectsItemShape);
    for(int i = 0; i < l.length(); i++)
        l.at(i)->setPos(l.at(i)->pos().x() + shift, l.at(i)->pos().y());
    calculation();
}

void ProtocolWindow::strikeBlue(int x, int y){
    ProtocolRates* item = static_cast<ProtocolRates*>(rateBlue->scene()->itemAt(x, y, QTransform()));
    if(item == nullptr){
        calculation();
        return;
    }
    item->strikethrough = true;
    int penWidth = 1;
    item->update(QRectF(-item->w / 2 - penWidth / 2, -item->h / 2 - penWidth / 2, item->w + penWidth, item->h + penWidth));
    calculation();
}

void ProtocolWindow::strikeRed(int x, int y){
    ProtocolRates* item = static_cast<ProtocolRates*>(rateRed->scene()->itemAt(x, y, QTransform()));
    if(item == nullptr){
        calculation();
        return;
    }
    item->strikethrough = true;
    int penWidth = 1;
    item->update(QRectF(-item->w / 2 - penWidth / 2, -item->h / 2 - penWidth / 2, item->w + penWidth, item->h + penWidth));
    calculation();
}

void ProtocolWindow::paintEvent(QPaintEvent*){
    QPainter pn(this);
    //pn.begin(this);
    pn.setPen(Qt::black);
    pn.end();
}

void ProtocolWindow::resizeEvent(QResizeEvent*){
    sceneRed->setSceneRect(0, 0, rateRed->width(), rateRed->height());
    sceneBlue->setSceneRect(0, 0, rateRed->width(), rateRed->height());

    itemNameRed->setSize(sportsmenRed->width(), sportsmenRed->height());
    sceneNameRed->setSceneRect(0, 0, sportsmenRed->width(), sportsmenRed->height());
    itemNameRed->setPos(itemNameRed->w / 2, itemNameRed->h / 2);

    itemNameBlue->setSize(sportsmenBlue->width(), sportsmenBlue->height());
    sceneNameBlue->setSceneRect(0, 0, sportsmenBlue->width(), sportsmenBlue->height());
    itemNameBlue->setPos(itemNameBlue->w / 2, itemNameBlue->h / 2);
}

bool ProtocolWindow::calculation(){
    bool clearVin = false;          //флаг чистой победы
    bool red_p3 = false;            //флаг третьего нарушения правил
    bool blue_p3 = false;           //флаг третьего нарушения правил
    bool red_p2 = false;            //флаг второго нарушения правил
    bool blue_p2 = false;
    bool red_p1 = false;
    bool blue_p1 = false;
    bool red_zp = false;
    bool blue_zp = false;
    bool red_zv = false;
    bool blue_zv = false;
    bool red_v = false;
    bool blue_v = false;
    bool red_plus = false;
    bool blue_plus = false;
    bool finish = false;            //флаг окончания боя

    itemNameRed->setStatus(0);
    itemNameBlue->setStatus(0);
    QList<QGraphicsItem*> l = sceneRed->items();
    int redBall = 0;
    int redAct = 0;

    for(int i = 0; i < l.length(); i++){
        ProtocolRates* each = static_cast<ProtocolRates*>(l.at(i));
        QVariant val;
        if(each->getTypeValue())
            val = each->getSValue();
        else
            val = each->getIValue();

        if(val.toInt() == 1 || val.toInt() ==2){
            if(each->strikethrough == false)
                redBall += val.toInt();
        }else if(val.toString() == "A"){
            if(each->strikethrough == false)
                redAct += 1;
        }else if(val.toString() == "V" && each->strikethrough == false){
            itemNameRed->setStatus(1);
            itemNameBlue->setStatus(2);
            ResultRed->setText("5");
            ResultBlue->setText("0");
            clearVin = true;
        }else if(val.toString() == "П3" && each->strikethrough == false)
            red_p3 = true;
        else if(val.toString() == "П2" && each->strikethrough == false)
            red_p2 = true;
        else if(val.toString() == "П1" && each->strikethrough == false)
            red_p1 = true;
        else if(val.toString() == "ЗП" && each->strikethrough == false)
            red_zp = true;
        else if(val.toString() == "В" && each->strikethrough == false)
            red_v = true;
        else if(val.toString() == "ЗВ" && each->strikethrough == false)
            red_zv = true;
        else if(val.toString() == "+" && each->strikethrough == false)
            red_plus = true;
    }

    BallsRed->setText(QString::number(redBall));
    //AktRed->setText(QString::number(redAct));

    l = sceneBlue->items();
    int blueBall = 0;
    int blueAct = 0;

    for(int i = 0; i < l.length(); i++){
        ProtocolRates* each = static_cast<ProtocolRates*>(l.at(i));
        QVariant val;
        if(each->getTypeValue())
            val = each->getSValue();
        else
            val = each->getIValue();

        if(val.toInt() == 1 || val.toInt() ==2){
            if(each->strikethrough == false)
                blueBall += val.toInt();
        }else if(val.toString() == "A"){
            if(each->strikethrough == false)
                blueAct += 1;
        }else if(val.toString() == "V" && each->strikethrough == false){
            itemNameRed->setStatus(2);
            itemNameBlue->setStatus(1);
            ResultRed->setText("0");
            ResultBlue->setText("5");
            clearVin = true;
        }else if(val.toString() == "П3" && each->strikethrough == false)
            blue_p3 = true;
        else if(val.toString() == "П2" && each->strikethrough == false)
            blue_p2 = true;
        else if(val.toString() == "П1" && each->strikethrough == false)
            blue_p1 = true;
        else if(val.toString() == "ЗП" && each->strikethrough == false)
            blue_zp = true;
        else if(val.toString() == "В" && each->strikethrough == false)
            blue_v = true;
        else if(val.toString() == "ЗВ" && each->strikethrough == false)
            blue_zv = true;
        else if(val.toString() == "+" && each->strikethrough == false)
            blue_plus = true;
    }

    BallsBlue->setText(QString::number(blueBall));
    //AktBlue->setText(QString::number(blueAct));
    if(clearVin == false){
        finish = true;
        if(vin == 1){
            itemNameBlue->setStatus(1);
            itemNameRed->setStatus(2);
            if(redBall == blueBall){
                ResultRed->setText("2");
                ResultBlue->setText("3");
            }else{
                ResultRed->setText("1");
                ResultBlue->setText("4");
            }
        }else if(vin == 2){
            itemNameBlue->setStatus(2);
            itemNameRed->setStatus(1);
            if(redBall == blueBall){
                ResultRed->setText("3");
                ResultBlue->setText("2");
            }else{
                ResultRed->setText("4");
                ResultBlue->setText("1");
            }
        }else{
            ResultRed->setText("");
            ResultBlue->setText("");
            finish = false;
       }
    }else

    finish = true;

    if(red_p3)
        change_prav(0, "П3");
    else if(red_p2)
        change_prav(0, "П2");    //первый аргумент: 0 - красный, 1 - синий
    else if(red_p1)
        change_prav(0, "П1");
    else if(red_zp)
        change_prav(0, "ЗП");
    else
        change_prav(0, "");

    if(blue_p3)
        change_prav(1, "П3");
    else if(blue_p2)
        change_prav(1, "П2");     //первый аргумент: 0 - красный, 1 - синий
    else if(blue_p1)
        change_prav(1, "П1");
    else if(blue_zp)
        change_prav(1, "ЗП");
    else
        change_prav(1, "");
    /*
    if(red_v)
        change_vyh(0, "В");
    else if(red_zv)
        change_vyh(0, "ЗВ");
    else
        change_vyh(0, "");

    if(blue_v)
        change_vyh(1, "В");
    else if(blue_zv)
        change_vyh(1, "ЗВ");
    else
        change_vyh(1, "");
    */
    if(red_plus)
        set_plus(0, "+");
    else
        set_plus(0, "");

    if(blue_plus)
        set_plus(1, "+");
    else
        set_plus(1, "");

    change_rate(redBall, blueBall, redAct, blueAct);
    //qDebug()<<redBall<<blueBall<<redAct<<blueAct;

    //qDebug()<<"calc";

    return finish;
}

void ProtocolWindow::rate_to_png(){
    if(num_fight == NULL)
        return;
    //QPixmap pix(sportsmenRed->width() + rateRed->width() + BallsRed->width() + ResultRed->width() + time->width(), rateRed->height() * 2);
    QPixmap pix(sportsmenRed->width() + rateRed->width() + BallsRed->width() + ResultRed->width() + time->width() + margin + spacing * 4,
                rateRed->height() * 2 + margin + spacing);
    render(&pix, QPoint(-sportsmenRed->pos().x() + margin / 2, -sportsmenRed->pos().y() + margin / 2));

    QByteArray ba;
    QBuffer buff(&ba);
    buff.open(QIODevice::WriteOnly);
    pix.save(&buff, "PNG");
    //char* pix_bytes = ba.data();
    //ba.prepend(QByteArray::number(num_fight));
    //char* pixmap_bytes = ba.data();
    QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE");
    db.setDatabaseName("baza_out.db");
    if(!db.open()){
        qDebug()<<"error open data base";
        return;
    }
    QSqlQuery query;
    query.prepare("UPDATE rounds SET result = ? WHERE fight = " + QString::number(num_fight));
    query.bindValue(0, ba);
    if(!query.exec()){
        qDebug()<<"error database";
    }else{
        //QThread* thread = new QThread;
        //ba.prepend(QByteArray::number(num_fight));
        ba.prepend(num_fight);
        /*
        TcpClient* client = new TcpClient(ba, address, mat, nullptr);
        client->moveToThread(thread);
        connect(thread, SIGNAL(started()), client, SLOT(run()));
        thread->start();
        thread->quit();
        thread ->wait();
        */
        QTcpSocket* tcp = new QTcpSocket(this);
        tcp->connectToHost(QHostAddress(address), mat);
        if (tcp->waitForConnected(100)){
            qDebug()<<"Connected!";
            int i = tcp->write(ba);
            qDebug()<<"Writed = "<<i<<num_fight;
            if(tcp->waitForReadyRead(2000)){
                qDebug()<<tcp->readAll();
            }else{
                qDebug()<<"not data";
            }
        }
        tcp-> close();
        delete tcp;

    }
    db.close();
}

void ProtocolWindow::resetRate(){
    currentX = 0;
    QList<QGraphicsItem*> items = sceneBlue->items();
    for(int i = 0; i < items.length(); i++)
        sceneBlue->removeItem(items.at(i));
    items = sceneRed->items();
    for(int i = 0; i < items.length(); i++)
        sceneRed->removeItem(items.at(i));
    vin = 0;
    calculation();
    rate_to_png();
    PCScreen* screen = static_cast<PCScreen*>(this->parent());
    screen->resetTimers();
}

void ProtocolWindow::showQueue(){
    q = new FightQueue(address, mat);
    connect(q, SIGNAL(select_fight(QString)), this, SLOT(selectFight(QString)));
    connect(q, SIGNAL(show_fight(QString)), this, SLOT(showFight(QString)));
    q->exec();
    delete q;
}

void ProtocolWindow::showFight(QString s){
    QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE");
    db.setDatabaseName("baza_out.db");
    QSqlQuery query;
    QString sql = "SELECT result FROM rounds WHERE fight = " + s;
    if(!db.open()){
        qDebug()<<"error open database";
        return;
    }
    if(!query.exec(sql))
        qDebug()<<"error database";
    else{
        QPixmap pix;
        if(query.next()){
            QByteArray ba = query.value(0).toByteArray();
            pix.loadFromData(ba, "PNG");
            ViewFight*  view = new ViewFight(pix, q);
            view->show();
        }
    }
    db.close();
}

void ProtocolWindow::pastTime(QString t){
    time->setText(t);
}

void ProtocolWindow::selectFight(QString s){
    Fight* obj = q->findChild<Fight*>(s);
    QMessageBox dialog(QMessageBox::Question, "Выбор спортсменов", obj->red + "\n" + obj->blue,
                                     QMessageBox::Ok | QMessageBox::Cancel, this);
    int result = dialog.exec();
    if(result == QMessageBox::Ok){

        itemNameRed->setName(obj->red);
        QList<QString> r = obj->red.split("\n");
        if(r.length() > 1){
            nameRed(r.at(0).split(" ").at(0));
            regRed(r.at(1).split("(").at(0));
        }
        itemNameBlue->setName(obj->blue);
        QList<QString> b = obj->blue.split("\n");
        if(b.length() > 1){
            nameBlue(b.at(0).split(" ").at(0));
            regBlue(b.at(1).split("(").at(0));
        }
        QPixmap pix(sportsmenRed->width() + rateRed->width() + BallsRed->width() + ResultRed->width(), sportsmenRed->height() * 2);
        render(&pix, QPoint(-sportsmenRed->pos().x(), -sportsmenRed->pos().y()));
        obj->lblFight->setPixmap(pix.scaled(obj->lblFight->width(), obj->lblFight->height()));
        obj->lblFight->setVisible(true);

        QByteArray ba;
        QBuffer buff(&ba);
        buff.open(QIODevice::WriteOnly);
        pix.save(&buff, "PNG");
        //char* pixmap_bytes = ba.data();

        num_fight = obj->objectName().toInt();
        QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE");
        db.setDatabaseName("baza_out.db");
        qDebug()<<"db.isOpen()"<<db.isOpen();
        if(!db.open()){
            qDebug()<<"eror open1";
            return;
        }
        qDebug()<<"open db";
        QSqlQuery query(db);

        qDebug()<<"db.isOpen()"<<db.isOpen();

        QString sql = "INSERT INTO rounds (fight, result) VALUES (:fight, :result);";

        query.prepare(sql);

        query.bindValue(":fight", obj->objectName().toInt());
        query.bindValue(":result", ba);
        qDebug()<<"db.isOpen() 3"<<db.isOpen();
        if(!query.exec()){
            qDebug()<<"error database1";
            db.close();
        }else{
            db.close();
            QSqlDatabase db_in = QSqlDatabase::addDatabase ("QSQLITE");
            db_in.setDatabaseName("baza_in.db");
            if(!db_in.open()){
                qDebug()<<"eror open2";
                return;
            }
            QSqlQuery query_in(db_in);
            if(!query_in.exec("SELECT * FROM referees WHERE id_fight = " + QString::number(num_fight)))
                qDebug()<<"error database2";
            else{
                if(query_in.next()){
                    nameMain->setText(query_in.value(2).toString());
                    nameRef->setText(query_in.value(3).toString());
                    nameSide->setText(query_in.value(4).toString());
                }
            }
            db_in.close();
        }
        resetRate();
        setWeight(obj->title.split(",").at(2));
    }
}
