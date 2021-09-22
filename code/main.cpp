#include "position.h"
#include "movegenerators.h"
#include "functions.h"
#include "node.h"
#include "move.h"
#include "search.h"

int main(){
    init();
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
                current->position->ShowBoard();
                break;
            }
            case 40093753:{ // moves}
                std::list<Move> *moves = current->position->GenerateAllLegalMoves();
                PrintMoveList(moves);
                delete moves;
                break;
            }
            case 9897063:{ //fen
                std::string fen;
                std::getline(std::cin, fen);
                delete newgame;
                newgame = new Node(fen);
                current = newgame;
                break;
            }
            case 169095377:{ // info
                /*std::cout << "white king           " << Ind2Not(current->position->whiteKingPos) << '\n';
                std::cout << "black king           " << Ind2Not(current->position->blackKingPos) << '\n';
                std::cout << "white castl          " << current->position->whcstl << '\n';
                std::cout << "black castl          " << current->position->blcstl << '\n';
                std::cout << "en passant           " << Ind2Not(current->position->EnPassantPos() << '\n';
                std::cout << "is check             " << current->position->underCheck << '\n';
                std::cout << "half move clock      " << current->position->halfMoveClock << '\n';
                std::cout << "pos hash             " << current->position->PositionHash() << '\n';
                std::cout << "to move              " << current->position->ToMove() << '\n';//*/
                break;
            }
            case 32144: { // go
                int depth;
                std::cin >> depth;
                SearchTree tree;
                tree.entryNode = current;
                tree.Search(depth);
                break;
            }
        }
    }
    delete newgame;
    cleanup();
}