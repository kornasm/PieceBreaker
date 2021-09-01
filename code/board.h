class MoveGenerator;
class Node;
class Move;

extern int PossibleMoves[13][28];
extern MoveGenerator* generators[];

class Board{
    public:
        int squares[121] = {7,
                             7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                             7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                             7, 2, 3, 4, 5, 6, 4, 3, 2, 7,
                             7, 1, 1, 1, 1, 1, 1, 1, 1, 7,
                             7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                             7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                             7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                             7, 0, 0, 0, 0, 0, 0, 0, 0, 7,
                             7, -1, -1, -1, -1, -1, -1, -1, -1, 7,
                             7, -2, -3, -4, -5, -6, -4, -3, -2, 7,
                             7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                             7, 7, 7, 7, 7, 7, 7, 7, 7, 7
        };
        Node* parentnode;
        int whiteKingPos;
        int blackKingPos;

        Board(Node* nd);
        Board(const Board&) = delete;
        Board& operator=(Board&) = delete;
        Board(Board* b, Move* m, int promo=0);
        ~Board();
        void ShowBoard() const;
        char GetPiece(int column, int row) const;
        int GetSquareValue(int column, int row) const;
        int GetSquareColor(int column, int row) const;
        char GetPiece(int index) const;
        int GetSquareValue(int index) const;
        int GetSquareColor(int index) const;

        bool IsPlaceAttacked(int attackedplace, int atackingcolor);

        Move* CheckMove(int from, int to); // only checks pseudo-legality
};