#include "declarations.h"

char PiecesSymbols[13] = {'k', 'q', 'b', 'n', 'r', 'p', '-', 'P', 'R', 'N', 'B', 'Q', 'K'};
MoveGenerator* generators[13];
int main(){
    init(generators);
    Node *newgame = new Node();

    int gamestate = 1;
    while(gamestate){
        sleep(1);
        std::cout << '\n';
        std::cout << "Enter command" << std::endl;
        std::string command;
        std::cin >> command;
        std::cout << h(command) << std::endl;
        switch(h(command)){
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
                //std::cout << "notation   " << notation << '\n';
                int position = Not2Ind(notation);
                if(NotationValid(notation)){
                    //std::cout << "notation valid,  position   " << position << '\n';
                    std::list<int>* movestemp = generators[newgame->GetBoardPtr()->GetSquareValue(position) + SYMBOLS_OFFSET]->GenerateMoveListv(position, newgame);
                    auto itt = movestemp->begin();
                    while(itt != movestemp->end()){
                        std::cout << Ind2Not(*itt) << " ";
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