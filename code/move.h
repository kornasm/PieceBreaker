#ifndef MOVE_H_
#define MOVE_H_

#include<iostream>
#include<string>

class Move{
    private:
        int from;
        int to;
        int type;
        int promo;
    public:
        static int count;
        Move(){}
        Move(int f, int t, int tp, int pr = 0);
        ~Move(){count++;}

        static Move* String2Move(std::string);

        int From() { return from; }
        int To() { return to; }
        int Type() { return type; }
        int& Promo() { return promo; }
        void IncreaseType(int i);
        bool operator ==(Move o) const;
        friend std::ostream& operator <<(std::ostream& out, Move& move);
};

std::ostream& operator <<(std::ostream& out, Move& move);

#endif