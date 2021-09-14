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
        Move(){}
        Move(int f, int t, int tp, int pr = 0);
        ~Move(){}

        static Move* String2Move(std::string);

        int From() const { return from; }
        int To() const { return to; }
        int Type() const { return type; }
        int Promo() const { return promo; }
        void SetPromo(int val) { promo = val; }
        void IncreaseType(int i);
        bool operator ==(Move o) const;
        friend std::ostream& operator <<(std::ostream& out, const Move& move);
};

std::ostream& operator <<(std::ostream& out, const Move& move);

#endif