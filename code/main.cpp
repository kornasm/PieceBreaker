#include "declarations.h"
char PiecesSymbols[NO_PIECES] = {'k', 'q', 'b', 'n', 'r', 'p', '-', 'P', 'R', 'N', 'B', 'Q', 'K'};
int mailbox[64] = {22, 23, 24, 25, 26, 27, 28, 29,
                   32, 33, 34, 35, 36, 37, 38, 39,
                   42, 43, 44, 45, 46, 47, 48, 49,
                   52, 53, 54, 55, 56, 57, 58, 59,
                   62, 63, 64, 65, 66, 67, 68, 69,
                   72, 73, 74, 75, 76, 77, 78, 79,
                   82, 83, 84, 85, 86, 87, 88, 89,
                   92, 93, 94, 95, 96, 97, 98, 99};

MoveGenerator* generators[NO_PIECES + 1];
int main(){
    init(generators);
    Node *newgame = new Node();
    std::cout << "move count   " << Move::count << '\n';
    int gamestate = 1;
    while(gamestate){
        std::cout << "Enter command:" << std::endl;
        std::string command;
        std::cin >> command;
        std::cout << h(command) << std::endl;
        switch(h(command)){
            case 289517988:{ // "move"
                std::string movestr;
                std::cin >> movestr;
                if(newgame->CheckMove(movestr)){
                    std::cout << "ok\n";
                }
                else{
                    std::cout << "not ok\n";
                }
                break;
            }
            case 49742601:{ // "exit"
                gamestate = 0;
                break;
            }
            case 133597110:{ // "board"
                newgame->ShowBoard();
                break;
            }
            case 40093753:{ // moves}
                std::string notation;
                std::cin >> notation;
                int position = Not2Ind(notation);
                if(NotationValid(notation)){
                    std::list<Move>* movestemp = generators[newgame->GetBoardPtr()->GetSquareValue(position) + SYMBOLS_OFFSET]->GenerateMoveListv(position, newgame);
                    auto itt = movestemp->begin();
                    while(itt != movestemp->end()){
                        std::cout << *itt;
                        ++itt;
                    }
                    delete movestemp;
                }
                else{
                    std::cout << "main    invalid notation\n";
                }
            }
        }//*/
    }
    delete newgame;
    cleanup(generators);
    std::cout << "move count:   " << Move::count << '\n';
}