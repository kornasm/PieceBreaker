class MoveGenerator;
class Node;
class Move;

extern int PossibleMoves[13][28];
extern MoveGenerator* generators[];

class Board{
    public:
        int _squares[121] = {7,
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
        int _whiteKingPos;
        int _blackKingPos;

        //Board();
        Board(Node* nd);
        Board(const Board&) = delete;
        Board& operator=(Board&) = delete;
        Board(Board* b, Move* m, int promo=0);
        ~Board();
        void ShowBoard();
        char GetPiece(int column, int row);
        int GetSquareValue(int column, int row);
        int GetSquareColor(int column, int row);
        char GetPiece(int index);
        int GetSquareValue(int index);
        int GetSquareColor(int index);

        bool IsPlaceAttacked(int attackedplace, int atackingcolor);

        Move* CheckMove(int from, int to); // only checks pseudo-legality
};