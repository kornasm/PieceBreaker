#include<iostream>


class Move{
    private:
        int from;
        int to;
        int type;
    public:
        Move(){}
        Move(const Move& m);
        Move(int f, int t, int tp);
        ~Move(){}
        int From() { return from; }
        int To() { return to; }
        int Type() {return type; }
        void IncreaseType(int i);
        bool operator ==(Move o) const;
        friend std::ostream& operator <<(std::ostream& out, Move& move);
};

std::ostream& operator <<(std::ostream& out, Move& move);