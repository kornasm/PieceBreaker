class MoveGenerator;
class Node;

extern int PossibleMoves[13][28];
extern MoveGenerator* generators[];

class Board{
    private:
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
        Node* parent;
        int _whiteKingPos;
        int _blackKingPos;
    public:
        //Board();
        Board(Node* nd);
        void ShowBoard();
        char GetPiece(int column, int row);
        int GetSquareValue(int column, int row);
        int GetSquareColor(int column, int row);
        char GetPiece(int index);
        int GetSquareValue(int index);
        int GetSquareColor(int index);

        bool CheckMove(int from, int to);
};