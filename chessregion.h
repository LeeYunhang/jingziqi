#ifndef CHESSREGION_H
#define CHESSREGION_H
#include<QRect>
#include<QPoint>
#include<QMap>
#include<QStack>
enum Status {HAS_ROUND_PIECE, HAS_NOROUND_PIECE, NON_PIECE};
class ChessRegion
{
public:
    ChessRegion();
    ~ChessRegion();
    const QRect& getRect () const
    {
        return rect;
    }
    const QRect& getSonRect  (int row, int col) const
    {
        return sonRect[row][col];
    }
    Status getStatus (int row, int col) const
    {
        return hasPiece[row][col];
    }
    Status getStatus (QPoint tmp)
    {
        return hasPiece[tmp.rx ()][tmp.ry ()];
    }

    void setStatus (int row, int col,Status status)
    {
        if (status != NON_PIECE) {
            hasPiece[row][col] = status;
            total++;
            stack_point.push (QPoint(row,col));
            stack_status.push (status);
        }
    }
    void regret () //对棋盘进行悔棋操作
    {
        QPoint tmp;
        if (total < 9) //<9的时候一定是偶数 需要一次撤销两颗棋子
        {
            if (stack_point.empty () == false){
                tmp = stack_point.top ();
                stack_point.pop ();
                stack_status.pop ();
                setStatus (tmp.rx (),tmp.ry (),NON_PIECE);
                tmp = stack_point.top ();
                stack_point.pop ();
                stack_status.pop ();
                setStatus (tmp.rx (),tmp.ry (),NON_PIECE);
                ----total;
            }
        }
        else if (total == 9){
            if (stack_point.empty () == false){
                tmp = stack_point.top ();
                stack_point.pop ();
                stack_status.pop ();
                setStatus (tmp.rx (),tmp.ry (),NON_PIECE);
                --total;
            }
        }
    }

    unsigned getTotal  () const
    {
        return total;
    }
    void clear ()
    {
        total = 0;
        stack_point.clear ();
        stack_status.clear ();
        for (int i = 0;i < MAX_BOUND; ++i)
            for (int j = 0; j < MAX_BOUND; ++j)
                hasPiece[i][j] = NON_PIECE;
    }

private:
    static const int MAX_BOUND = 3;
    QRect rect = {0,0,330,240};
    QStack<Status> stack_status; //保存状态的栈
    QStack<QPoint> stack_point; //保存坐标的栈
    QRect sonRect[3][3] {
        {{0,0,110,80},{110,0,110,80},{220,0,110,80}},
        {{0,80,110,80},{110,80,110,80},{220,80,110,80}},
        {{0,160,110,80},{110,160,110,80},{220,160,110,80}},
    };
    Status hasPiece[3][3];
    unsigned total = 0; //当前棋盘上总的棋
};

#endif // CHESSREGION_H
