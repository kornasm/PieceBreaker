#include<iostream>


class Move{
    private:
        int from;
        int to;
        int type;
    public:
        Move(){ std::cout << "Move constructor1\n"; }
        Move(const Move& m);
        Move(int f, int t, int tp);
        ~Move(){ std::cout << "Move destructor----  " << *this; count--; }
        int To() { return to; }
        void IncreaseType(int i);
        bool operator ==(Move o) const;
        friend std::ostream& operator <<(std::ostream& out, Move& move);
        static Move* CreateMove(int f, int t, int tp){
            Move* m = new Move(f, t, tp);
            return m;
        }

        static int count;
};

std::ostream& operator <<(std::ostream& out, Move& move);