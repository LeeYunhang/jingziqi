#ifndef WIDGET_H
#define WIDGET_H
#include<QMouseEvent>
#include <QWidget>
#include<robot.h>
#include<QDebug>
#include<QPainter>
#include<QColorDialog>
#include<QMessageBox>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QCursor cursor;
    ChessRegion cr; //棋盘
    Robot robot;
    void DrawBound (QPainter& painter) //画边界
    {
        QRect rect = cr.getRect ();
        painter.drawLine (0,0,330,0);
        painter.drawLine (0,0,0,240);
        painter.drawLine (0,240,330,240);
        painter.drawLine (330,0,330,240);
        painter.drawLine(0,80,330,80);
        painter.drawLine(0,160,330,160);
        painter.drawLine(110,0,110,240);
        painter.drawLine(220,0,220,240);
    }

protected:
    void mousePressEvent (QMouseEvent *event) override
    {
        auto offset = event->globalPos () - pos();
        update();
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (cr.getSonRect (i,j).contains (offset,true) && cr.getStatus (i,j) == NON_PIECE)
                {
                    cr.setStatus (i,j,HAS_ROUND_PIECE);
                    robot.updateChessRegion (&cr);
                    QPoint tmp = robot.nextStep ();
                    if (cr.getTotal () < 9){
                        cr.setStatus (tmp.rx (),tmp.ry (),HAS_NOROUND_PIECE);

                    }


                }

    }

    void mouseMoveEvent (QMouseEvent *event) override
    {
        auto offset = event->globalPos () - pos();
       if (cr.getRect ().contains (offset,true))
        {
            cursor.setShape (Qt::OpenHandCursor);
            setCursor (cursor);
        }
        else {
            cursor.setShape (Qt::ArrowCursor);
            setCursor (cursor);
        }
    }
    void paintEvent (QPaintEvent *event)
    {
        QPainter painter (this);
        QRect rect_tmp;
        QPoint point; //每一个子矩形中心的点的坐标
        DrawBound(painter);
        painter.setPen (QColor(0,0,255));
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j)
            {
                rect_tmp = cr.getSonRect (i,j);
                point = QPoint((rect_tmp.left ()+ rect_tmp.right ()) / 2,
                               (rect_tmp.top () + rect_tmp.bottom ()) / 2);
                if (cr.getStatus (i,j) == HAS_NOROUND_PIECE) {
                    painter.drawLine (point - QPoint(30,30),point + QPoint(30,30));
                    painter.drawLine (point - QPoint(-30,30),point + QPoint(-30,30));
                    /*painter.drawLine (QPoint(rect_tmp.left (),rect_tmp.top ()),
                                      QPoint(rect_tmp.right (),rect_tmp.bottom ()));
                    painter.drawLine (QPoint(rect_tmp.right (),rect_tmp.top ()),
                                      QPoint(rect_tmp.left (),rect_tmp.bottom ()));*/
                }
                else if (cr.getStatus (i,j) == HAS_ROUND_PIECE) {
                    painter.drawEllipse (cr.getSonRect (i,j).center (),30,30);
                }
            }
    }
}
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // WIDGET_H
