#include<iostream>


class Move{
    private:
        int from;
        int to;
        int type;
    public:
        Move(int f, int t, int tp);
        ~Move(){ std::cout << "Move destructor\n";}
        bool operator ==(Move o) const;
        friend std::ostream& operator <<(std::ostream& out, Move& move);
        static Move CreateMove(int f, int t, int tp){
            Move* m = new Move(f, t, tp);
            return *m;
        }
};

std::ostream& operator <<(std::ostream& out, Move& move);
