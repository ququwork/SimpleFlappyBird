#include "mainwindow.h"
#include <ctime>
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    win_width = 380;
    win_height = 490;
    this->setFixedSize(win_width,win_height);	//視窗大小
    this->setWindowIcon(QIcon(":/Image/bird1.png"));
    this->setWindowTitle("Qt FlappyBird");

    die = new easyMusic("Soundeffect/DAMN SON!.mp3",100 ,0);
    flap = new easyMusic("Soundeffect/Jump.mp3",80,0);
    hit = new easyMusic("Soundeffect/Punch.mp3",80,0);
    nathan = new easyMusic("Soundeffect/He.mp3",100,0);
    bgm = new easyMusic("Soundeffect/SpongeBob.mp3",85,1);

    //創建水管
    for(int i=0;i<pipeCount;i++)
        pipe[i]=new OBSTACLE(this);

    //地板
    for(int i=0;i<2;i++)
        floor[i]=new FLOOR(this);

    //創建鳥
    birds=new ROLE(this);
    //翻身鳥
    flip =new flipping(this);
    flip->setVisible(0);

    //遊戲初始模式
    gameRedy();

    //遊戲開始
    //gameStart();
}
void MainWindow::paintEvent(QPaintEvent *)		//繪圖事件, 用来產生背景
{
    QPainter painter(this);
    QPixmap bg;
    bg.load(":/Image/bg.png");
    painter.drawPixmap(0, 0, 380, 450, bg);

    // 可以在這裡新增背景圖片
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(gamemod==redy){
        if(event->button()==Qt::LeftButton){
            gameStart();
        }

    } else if(gamemod==start){
        if(event->button()==Qt::LeftButton){
            flap->stop();
            flap->play();
            birdup();
        }
    } else if(gamemod==lose){
        if(event->button()==Qt::LeftButton){
            gameRedy();

        }
    }
    // 在這裡設定按下滑鼠要觸發的功能
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(gamemod==redy){
        if(event->key()==Qt::Key_Space){
            gameStart();
        }

    } else if(gamemod==start){
        if(event->key()==Qt::Key_Space){
            flap->stop();
            flap->play();
            birdup();
        }
    } else if(gamemod==lose){
        if(event->key()==Qt::Key_Space){
            gameRedy();

        }
    }
    // 在這裡設定按下鍵盤要觸發的功能
}
void MainWindow::createflip()
{
    flip->move(0,0);

    flipTimer = new QTimer(this);
    connect(flipTimer,SIGNAL(timeout()),this,SLOT(fliperino()));
    flipTValue = 8;
}

void MainWindow::createFloor()
{
    floor[0]->move(0,450);
    floor[1]->move(405,450);

    floorTimer = new QTimer(this);
    connect(floorTimer,SIGNAL(timeout()),this,SLOT(floorAction()));
    floorTValue=8;
}
void MainWindow::createPipe()		//初始化水管，依序排在視窗外
{

    int startx=win_width+10;		//第一個水管的位置
    pipeXgap=200;		//水管水平間距
    int pipR;			//水管垂直位置隨機數
    qsrand(time(NULL));		//qrand是qt的隨機函數 用法和c的rand一樣

    for(int i=0;i<pipeCount;i++)
    {
        pipR=qrand()%200;

        pipe[i]->move(startx+i*pipeXgap,-200+pipR-40);
        lastPipe=i;		//很重要 記錄最後一個水管號碼 為後面的循環水管建立基礎
    }

    pipeTimer =new QTimer(this);	//pipeTimer 處理兩個動作
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(pipeAction()));
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(collisDete()));
    pipeTValue=8;

}
void MainWindow::createBird(){

    birds->move(60,250);
    //鳥的上下速度
    this->birdV_array[0]=-3;
    this->birdV_array[1]=-4;
    this->birdV_array[2]=-3;
    this->birdV_array[3]=-2;
    this->birdV_array[4]=-1;
    this->birdV_array[5]=-1;
    this->birdV_array[6]=-1;
    this->birdV_array[7]=0;
    this->birdV_array[8]=1;
    this->birdV_array[9]=1;
    this->birdV_array[10]=2;
    this->birdV_array[11]=2;
    this->birdV_array[12]=2;
    this->birdV_array[13]=3;
    this->birdV_array[14]=3;

    this->index_birdV=0;

    birdV=0;

    birdTimer=new QTimer(this);
    connect(birdTimer,SIGNAL(timeout()),this,SLOT(birdAction()));
    timedata=8;
    connect(birdTimer,SIGNAL(timeout()),this,SLOT(fliperino()));
}
void MainWindow::birdup()
{
    index_birdV=0.0;
    birdV=birdV_array[int(index_birdV)];
}

void MainWindow::fliperino()//翻
{
    flip->move(birds->pos().x(),birds->pos().y());
}

void MainWindow::birdAction()
{
    //鳥的運動
    // 每次觸發這個function都會更改鳥的位置，x軸不變, y軸加上 birdV成為新的位置
    // XX->pos().x() and XX->pos().y() 是QWidget物件的函式，可以用來取得xy座標位置
    birds->move(birds->pos().x(),birds->pos().y()+birdV);


    if(index_birdV<14.0)
        index_birdV+=0.2;
    else
        index_birdV=14.0;

    birdV=birdV_array[int(index_birdV)];

    // 這邊做一個地板碰撞偵測
    if(birds->pos().y()+birds->height()>=450+10)
    {
        if(!alreadyhit){
            bgm->stop();
            die->play();
        }

        birds->move(birds->pos().x(),450-birds->height()+10); // XX->height可用來取得物件高度
        birdTimer->stop();
        birds->setVisible(0);
        flip->setVisible(1);
        gameLose();
    }
}
void MainWindow::pipeAction()
{
    //水管動畫，水管離開地圖後，重新回到右側接替lastPipe位置
    int pipR; // 一個隨機變數，讓每次水管的位置都不太一樣
    // 每次觸發這個函式，水管都會向左移動，如果最左邊的水管離開地圖範圍則移動到最右邊
    for(int i=0;i<pipeCount;i++)
    {
        pipe[i]->move(pipe[i]->pos().x()-1,pipe[i]->pos().y());
        if(pipe[i]->pos().x()<-100)
        {
            pipR=qrand()%200;
            pipe[i]->move(pipe[lastPipe]->pos().x()+pipeXgap,-200+pipR);
            lastPipe=i;
        }
    }
}
void MainWindow::floorAction()//地板移動
{
    for(int i=0;i<2;i++){
        floor[i]->move(floor[i]->pos().x()-1,450);
        if(floor[i]->pos().x()<-405){
          floor[i]->move(404,450);
        }
    }
}
void MainWindow::collisDete()
{
    //水管碰撞偵測
    int birdRx=birds->pos().x()+30;
    int birdDy=birds->pos().y()+30;
    for(int i=0;i<pipeCount;i++)
    {
        if(birdRx>=pipe[i]->pos().x()&&birds->pos().x()<=pipe[i]->pos().x()+pipe[i]->width()-10) // XX->width可用來取得物件寬度
        {
            if(birds->pos().y() <= (pipe[i]->pos().y()+pipe[i]->getH1()) || birdDy >= (pipe[i]->pos().y()+pipe[i]->getH1()+pipe[i]->getGap()))
            {
                bgm->stop();
                hit->play();

                alreadyhit=1;//
                //碰到水管遊戲結束
                QThread::sleep(1);
                nathan->play();
                QThread::sleep(3);
                birds->setVisible(0);//音效動畫間隔
                flip->setVisible(1);
                gameLose();
            }
        }
    }
}
void MainWindow::gameRedy()
{
    gamemod=redy;
    bgm->play();
    createBird();
    createPipe();
    createFloor();
    birds->setVisible(1);
    flip->setVisible(0);
    nathan->stop();
    die->stop();
    //終止音效
}
void MainWindow::gameLose()
{
    gamemod=lose;
    pipeTimer->stop();
    floorTimer->stop();

}
void MainWindow::gameStart()
{
    gamemod=start;
    birdTimer->start(timedata);
    pipeTimer->start(pipeTValue);
    floorTimer->start(floorTValue);
    alreadyhit=0;

}
