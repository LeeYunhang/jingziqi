#ifndef ROBOT_H
#define ROBOT_H
#include<chessregion.h>
#include<QPoint>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
#include<QVector>
#include<QCoreApplication>
class Robot
{
public:
    Robot(ChessRegion *cr);
    void updateChessRegion (ChessRegion *cr)
    {
        this->cr = cr;
    }
    QPoint nextStep ()
    {
        if ((cr->getTotal () + 1) / 2 == 1)
            return handleFirstStep ();
        else if ((cr->getTotal () + 1) / 2 == 2)
             return  handleSecond ();
        else if( handleNext() != QPoint(-1,-1)){ //如果不是第一和第二步(对于机器人)，那么
               return handleNext();
        }
        else return {-1,-1};

    }
    Robot () = default;
    ~Robot();
private:
    using MyVector = QVector<QPoint>;
    ChessRegion *cr;
    unsigned first_step_case;
    const int MAN_WIN = 1;
    const int ROBOT_WIN = 2;
    QPoint first_piece;   //机器人下的第一步棋的位置
    const QPoint line[8][3]  {
        {{0,0},{1,0},{2,0}},
        {{0,1},{1,1},{2,1}},
        {{0,2},{1,2},{2,2}},
        {{0,0},{0,1},{0,2}},
        {{1,0},{1,1},{1,2}},
        {{2,0},{2,1},{2,2}},
        {{0,0},{1,1},{2,2}},
        {{0,2},{1,1},{2,0}},
    };
    QPoint handleFirstStep ()
    {
        srand ((unsigned)time (NULL));
        handleFirstStep_case ();
        switch (first_step_case){
        case 0:  //表示人下的第一颗棋在中间
            return first_piece =  getElsePiece ((unsigned)rand() % 4);
        case 1:
        case 2:
            first_piece = QPoint(1,1);
            return first_piece;
        }
    }
    void handleFirstStep_case ()
    {
        if (cr->getStatus (1,1) == HAS_ROUND_PIECE)
            first_step_case = 0; //表示人下的第一颗棋在中间
        else if (cr->getStatus (0,0) == HAS_ROUND_PIECE || cr->getStatus (2,0) == HAS_ROUND_PIECE
                 || cr->getStatus (0,2) == HAS_ROUND_PIECE || cr->getStatus (2,2) == HAS_ROUND_PIECE)
            first_step_case = 1; //表示人下的第一颗棋在对角线上
        else first_step_case = 2; //表示hi人下的第一颗棋不属于前两种的情况
    }
    /*返回垂直水平方向上任意一颗棋，
     * 给上右下左的棋分别标记为0，1，2，3*/
    QPoint getVerAndHorPiece (unsigned i)
    {
        switch (i){
        case 0:
            return QPoint (1,0);
        case 1:
            return QPoint (2,1);
        case 2:
            return QPoint (1,2);
        case 3:
            return QPoint (0,1);
        }
    }
    /*返回对角线方向上任意一颗棋，
     * 给上右下左的棋分别标记为0，1，2，3*/
    QPoint getElsePiece (unsigned i)
    {
        switch (i){
        case 0:
            return QPoint (0,0);
        case 1:
            return QPoint (2,0);
        case 2:
            return QPoint (2,2);
        case 3:
            return QPoint (0,2);
        }
    }
    QPoint handleSecond ()
    {
        QPoint tmp;
        switch (first_step_case){
        case 0:
        {
            //
            switch (handleSecond_case ()) {
                case 0:
                return handleOnSameLine();
            default:
                handleNext();
            }
        }
        case 1:
        case 2:
            return handleNext();

        }
    }

    unsigned handleSecond_case ()
    {
        if (onSameLine ())
            return 0;
        else return 1;
    }
    QPoint handleOnSameLine ()
    {
        QPoint tmp[2];
        if ((cr->getStatus (0,0) == HAS_NOROUND_PIECE) || (cr->getStatus (2,2) == HAS_NOROUND_PIECE))
        {
            tmp[0] = QPoint(2,0);
            tmp[1] = QPoint(0,2);
        }
        else
        {
            tmp[0] = QPoint(0,0);
            tmp[1] = QPoint(2,2);
        }
        return tmp[(unsigned)rand() % 2]; //随机选择一个空位下
    }

    bool onSameLine ()
    {
        if ((first_piece == QPoint (0,0) && cr->getStatus (2,2) == HAS_ROUND_PIECE) ||
               (first_piece == QPoint (2,0) && cr->getStatus (0,2) == HAS_ROUND_PIECE)||
               (first_piece == QPoint (0,2) && cr->getStatus (2,0)  == HAS_ROUND_PIECE)||
                (first_piece == QPoint (2,2) && cr->getStatus (0,0)  == HAS_ROUND_PIECE))
            return true;
        else return false;

    }
    QPoint handleNext ()
    {
        QPoint tmp = satisfyIsWin (ROBOT_WIN);
        MyVector vector_tmp;
        if (tmp != QPoint(-1,-1)){ //如果机器人可以取得胜利的话
            return tmp;
        }
        tmp = satisfyIsWin (MAN_WIN);
         if (tmp != QPoint(-1,-1)) {//如果人可以取得胜利的话
             return tmp;
         }
         vector_tmp = canPlayChess ();
         if (vector_tmp.size () != 0 && vector_tmp[0] != QPoint(-1,-1)) {
             tmp = vector_tmp[(unsigned)rand() % vector_tmp.size ()];
             return tmp;
         }
         for (int i = 0; i < 3; ++i) {
             for (int j = 0; j < 3; ++j) {
                 if (cr->getStatus (i,j) == NON_PIECE)
                   return {i,j};
             }
         }
         return {-1,-1};

    }
    /*可以落下棋的位置的集合*/
    MyVector canPlayChess ()
    {
        MyVector vector_tmp;
        for (int i = 0; i < 8; ++i) {
            int noRound = 0; //某一条线上的机器人的棋的个数
            int noPiece = 0; //空白位置个数
            for (auto j = 0;j < 3; ++j) {
                QPoint tmp = line[i][j];
                if (cr->getStatus (tmp.rx (),tmp.ry ()) == HAS_NOROUND_PIECE)
                    ++noRound;
                else if (cr->getStatus (tmp.rx (),tmp.ry ()) == NON_PIECE) {
                    ++noPiece;
                }
            }
            //如果满足胜利的条件，则返回接下来需要下的位置
            if (noRound == 1 && noPiece == 2)
                for (auto var : line[i]) {
                    if (cr->getStatus (var.rx (),var.ry ()) == NON_PIECE)
                        vector_tmp.push_back(var);
                }
            noRound = noPiece = 0;
        }
        if (vector_tmp.size () != 0)
            return vector_tmp;
        else {
            vector_tmp.push_back ({-1,-1});
        }
    }

    QPoint satisfyIsWin (int who)
    {
        QPoint result;
        if (who == ROBOT_WIN) {
            for (int i = 0; i < 8; ++i) {
                int noRound = 0; //某一条线上的机器人的棋的个数
                int noPiece = 0; //空白位置个数
                for (auto j = 0;j < 3; ++j) {
                    QPoint tmp = line[i][j];
                    if (cr->getStatus (tmp.rx (),tmp.ry ()) == HAS_NOROUND_PIECE)
                        ++noRound;
                    else if (cr->getStatus (tmp.rx (),tmp.ry ()) == NON_PIECE) {
                        ++noPiece;
                        result = tmp;
                    }
                }
                //如果满足胜利的条件，则返回接下来需要下的位置
                if (noRound == 2 && noPiece == 1)
                    return result;
                noRound = noPiece = 0;
            }
            return {-1,-1}; //因为所有方向都没有找到可以赢的位置，返回-1表示错误
        }
       else if (who == MAN_WIN) {
            for (int i = 0; i < 8; ++i) {
                int Round = 0; //某一条线上的人的棋的个数
                int noPiece = 0; //空白位置个数
                for (auto j = 0;j < 3; ++j) {
                    QPoint tmp = line[i][j];
                    if (cr->getStatus (tmp.rx (),tmp.ry ()) == HAS_ROUND_PIECE)
                        ++Round;
                    else if (cr->getStatus (tmp.rx (),tmp.ry ()) == NON_PIECE) {
                        ++noPiece;
                        result = tmp;
                    }
                }
                //如果满足胜利的条件，则返回接下来需要下的位置
                if (Round == 2 && noPiece == 1)
                    return result;
                Round = noPiece = 0;
            }
            return {-1,-1}; //因为所有方向都没有找到可以赢的位置，返回-1表示错误
        }
        else return {-1,-1}; //非法输入返回的错误结果
    }
};

#endif // ROBOT_H
