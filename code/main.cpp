#include "declarations.h"

MoveGenerator* generators[NO_PIECES + 1];
int main(){
    init(generators);
    Node *newgame = new Node();
    Node *current = newgame;
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
                if(movestr.length() >= 4){
                    Move *insertedmove = Move::String2Move(movestr);
                    if(insertedmove != NULL){
                        if(current->CheckMove(insertedmove)){
                            std::cout << "ok\n";
                            current = current->children.back();
                        }
                        else{
                            std::cout << "not ok\n";
                        }
                        delete insertedmove;
                    }
                }
                else{
                    std::cout << "incorrect move syntax\n";
                }
                break;
            }
            case 49742601:{ // "exit"
                gamestate = 0;
                break;
            }
            case 133597110:{ // "board"
                current->ShowBoard();
                break;
            }
            case 40093753:{ // moves}
                std::list<Move> *moves = current->GenerateAllLegalMoves();
                PrintMoveList(moves);
                std::cout << "\n\n";
                delete moves;
                std::string notation;
                std::cin >> notation;
                int position = Not2Ind(notation);
                if(NotationValid(notation)){
                    std::list<Move>* movestemp = generators[current->GetBoardPtr()->GetSquareValue(position) + SYMBOLS_OFFSET]->GenerateMoveListVirtual(position, newgame);
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
            case 9897063:{ //fen
                std::string fen;
                std::getline(std::cin, fen);
                delete newgame;
                newgame = new Node(fen);
                current = newgame;
            }
        }//*/
    }
    delete newgame;
    cleanup(generators);
}