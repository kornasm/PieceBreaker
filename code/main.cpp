#include "declarations.h"
char PiecesSymbols[NO_PIECES] = {'k', 'q', 'b', 'n', 'r', 'p', '-', 'P', 'R', 'N', 'B', 'Q', 'K'};
MoveGenerator* generators[NO_PIECES + 1];
int main(){
    init(generators);
    Node *newgame = new Node();

    int gamestate = 1;
    while(gamestate){
        sleep(1);
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
}