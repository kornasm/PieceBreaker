#include "declarations.h"

int mailbox[64] = {22, 23, 24, 25, 26, 27, 28, 29,
                   32, 33, 34, 35, 36, 37, 38, 39,
                   42, 43, 44, 45, 46, 47, 48, 49,
                   52, 53, 54, 55, 56, 57, 58, 59,
                   62, 63, 64, 65, 66, 67, 68, 69,
                   72, 73, 74, 75, 76, 77, 78, 79,
                   82, 83, 84, 85, 86, 87, 88, 89,
                   92, 93, 94, 95, 96, 97, 98, 99};
extern MoveGenerator* generators[NO_PIECES + 1];
int drawMaterial[NO_PIECES] = {0, -10, -1, -1, -10, -10, 0, 10, 10, 1, 1, 10, 0};

Node::Node(){
    board = new Board(this);
    toMove = WHITE;
    whcstl = SHORT_CASTLE_MOVE | LONG_CASTLE_MOVE;
    blcstl = SHORT_CASTLE_MOVE | LONG_CASTLE_MOVE;
    enPassant = -1;
    prev = NULL;
    result = ONGOING;
    underCheck = false;
    halfMoveClock = 0;
    fullMoveCounter = 1;
    searchBackRepetitions = false;
    CalculatePositionHash();
}

Node::Node(Node *pr, Board* b, Move *m, bool realnode){
    prev = pr;
    board = b;
    b->parentnode = this;
    toMove = prev->toMove * (-1);
    whcstl = prev->whcstl;
    blcstl = prev->blcstl;
    if(pr->toMove == WHITE){
        if(whcstl % SHORT_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e1") || m->From() == Not2Ind("h1")){
                whcstl -= SHORT_CASTLE_MOVE / 2;
            }
        }
        if(whcstl % LONG_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e1") || m->From() == Not2Ind("a1")){
                whcstl -= LONG_CASTLE_MOVE / 2;
            }
        }
    }
    else{
        if(blcstl % SHORT_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e8") || m->From() == Not2Ind("h8")){
                blcstl -= SHORT_CASTLE_MOVE / 2;
            }
        }
        if(blcstl % LONG_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e8") || m->From() == Not2Ind("a8")){
                blcstl -= LONG_CASTLE_MOVE / 2;
            }
        }
    }
    if(m->Type() == PAWN_DOUBLE_MOVE){
        enPassant = (m->From() + m->To()) / 2;
    }
    else{
        enPassant = -1;
    }//*/
    if(m->From() == b->whiteKingPos){
        b->whiteKingPos = m->To();
    }
    if(m->From() == b->blackKingPos){
        b->blackKingPos = m->To();
    }
    if(toMove == WHITE){
        fullMoveCounter++;
    }
    if(m->Type() < PAWN_MOVE){ // only regular moves and castles (no captures)
        halfMoveClock++;
        searchBackRepetitions = true;
    }
    else{
        halfMoveClock = 0;
        searchBackRepetitions = false;
    }
    if(realnode){
        CalculatePositionHash();
        CheckEndings();
    }
}

Node::Node(std::string fen){
    board = new Board(this);
    while(fen[0] == ' '){
        fen.erase(0, 1);
    }
    //std::cout << "full fen    #" << fen << '#' << '\n';
    int ind = fen.find(' ');
    //std::cout << ind << '\n';
    std::string fentemp = fen.substr(0, ind);
    fen.erase(0, ind + 1);
    //std::cout << "first cut   #" << fentemp << '#' << '\n';
    
    unsigned int index = 0;
    for(int i = 7; i>= 0; i--){
        int j = 0;
        while(fentemp[index] != '/' && index < fentemp.length()){
            switch(fentemp[index]){
                case 'K':
                    board->squares[mailbox[8 * i + j]] = WHITE_KING;
                    board->whiteKingPos = mailbox[8 * i + j];
                    break;
                case 'k':
                    board->squares[mailbox[8 * i + j]] = BLACK_KING;
                    board->blackKingPos = 8 * i + j;
                    break;
                case 'Q':
                    board->squares[mailbox[8 * i + j]] = WHITE_QUEEN;
                    break;
                case 'q':
                    board->squares[mailbox[8 * i + j]] = BLACK_QUEEN;
                    break;
                case 'R':
                    board->squares[mailbox[8 * i + j]] = WHITE_ROOK;
                    break;
                case 'r':
                    board->squares[mailbox[8 * i + j]] = BLACK_ROOK;
                    break;
                case 'B':
                    board->squares[mailbox[8 * i + j]] = WHITE_BISHOP;
                    break;
                case 'b':
                    board->squares[mailbox[8 * i + j]] = BLACK_BISHOP;
                    break;
                case 'N':
                    board->squares[mailbox[8 * i + j]] = WHITE_KNIGHT;
                    break;
                case 'n':
                    board->squares[mailbox[8 * i + j]] = BLACK_KNIGHT;
                    break;
                case 'P':
                    board->squares[mailbox[8 * i + j]] = WHITE_PAWN;
                    break;
                case 'p':
                    board->squares[mailbox[8 * i + j]] = BLACK_PAWN;
                    break;
                default:;
            }
            if(isdigit(fentemp[index])){
                int x = (int)(fentemp[index]) - '0';
                for(; x> 0; x--){
                    board->squares[mailbox[8 * i + j]] = EMPTY_SQUARE;
                    j++;
                }
            }
            else{
                j++;
            }
            index++;
        }
        index++;
    }
    //std::cout << fen << '\n';
    fen[0] == 'w'? toMove = WHITE : toMove = BLACK;
    fen.erase(0, 2);
    //std::cout << fen << "!\n";
    fentemp = fen.substr(0, fen.find(' '));
    whcstl = 0;
    blcstl = 0;
    if(fentemp.find('K') != std::string::npos) 
        whcstl |= SHORT_CASTLE_MOVE;
    if(fentemp.find('Q') != std::string::npos) 
        whcstl |= LONG_CASTLE_MOVE;
    if(fentemp.find('k') != std::string::npos) 
        blcstl |= SHORT_CASTLE_MOVE;
    if(fentemp.find('q') != std::string::npos) 
        blcstl |= LONG_CASTLE_MOVE;
    fen.erase(0, fentemp.length() + 1);
    //std::cout << fen << "!\n";
    fen[0] == '-' ? enPassant = -1 : enPassant = Not2Ind(fen.substr(0, 2));
    fen.erase(0,fen.find(' ') + 1);
    if(fen[1] != ' '){
        halfMoveClock = 10 * ((int)(fen[0]) - '0') + (int)(fen[1]) - '0';
    }
    else{
        halfMoveClock = (int)(fen[0]) - '0';
    }
    fen.erase(0, fen.find(' ') + 1);
    //std::cout << fen << "!\n";
    fullMoveCounter = 1;
    for(uint i = 0; i < fen.length(); i++){
        fullMoveCounter *= 10;
        fullMoveCounter += (int)(fen[i]) - '0';
    }
    searchBackRepetitions = false;
    CalculatePositionHash();
    CheckCheck();
    prev = NULL;
    result = GameResult::ONGOING;
}

Node::~Node(){
    delete board;
    for(auto nd : children){
        delete nd;   
    }
}
void Node::ShowBoard() const{
    board->ShowBoard();
}

Board* Node::GetBoardPtr(){
    return board;
}


bool Node::CheckMove(Move *checkedmove, bool execute){
    int PieceColor = board->GetSquareColor(checkedmove->From());
    if(toMove != PieceColor){
        std::cout << "wrong piece chosen (wrong color)\n";
        return false;
    }
    Move *expectedmove = board->CheckMove(checkedmove->From(), checkedmove->To());
    if(NULL != expectedmove)
    {
        //std::cout << "expected move found\n\n";
        if(((expectedmove->Type() & PROMOTION_MOVE) == PROMOTION_MOVE) && checkedmove->Promo() == EMPTY_SQUARE){
            std::cout << "No promo char entered\n";
            delete expectedmove;
            return false;
        } 
        //std::cout << "creating new board\n";
        Board *newboard = new Board(board, expectedmove, checkedmove->Promo());
        Node *newnode = new Node(this, newboard, expectedmove, execute);
        if(toMove == WHITE){
            if(newboard->IsPlaceAttacked(newboard->whiteKingPos, newnode->toMove)){
                std::cout << "king attacked after the move\n";
                delete newnode;
                delete expectedmove;
                return false;
            }
        }
        else{
            if(newboard->IsPlaceAttacked(newboard->blackKingPos, newnode->toMove)){
                std::cout << "king attacked after the move\n";
                delete newnode;
                delete expectedmove;
                return false;
            }
        }
        if(execute == true){
            children.push_back(newnode);
        }
        else{
            delete newnode;
        }
        delete expectedmove;
        return true;
    }
    return false;
}

void Node::CheckCheck(){
    if(toMove == WHITE){
        if(board->IsPlaceAttacked(board->whiteKingPos, -toMove)){
            underCheck = true;
        }
        else{
            underCheck = false;
        }
    }
    else{
        if(board->IsPlaceAttacked(board->blackKingPos, -toMove)){
            underCheck = true;
        }
        else{
            underCheck = false;
        }
    }
}

std::list<Move>* Node::GenerateAllLegalMoves(){
    std::list<Move>* moves = AllMovesGenerator::GenerateMoves(this);
    std::list<Move>* results = new std::list<Move>();
    auto it = moves->begin();
    while(it != moves->end()){
        if(CheckMove(&(*it), false)){
            results->push_back(*it);
        }
        ++it;
    }
    delete moves;
    return results;
}

void Node::CheckEndings(){
    if(halfMoveClock>= 50){
        result = GameResult::DRAW;
        return;
    }
    result = GameResult::ONGOING;
    int count = 0; // repetition draw
    //std::cout << "checking repetition draw  " << positionHash << '\n';
    Node *current = this;
    while(true){
        if(current->positionHash == this->positionHash){
            count++;
            if(count>= 3){
                result = GameResult::DRAW;
                std::cout << "repetition draw\n";
                return;
            }
        }
        if(current->searchBackRepetitions == false){
            break;
        }
        if(NULL == current->prev){
            break;
        }
        else{
            current = current->prev;
        }
    }
    std::list<Move>* possiblemoves = GenerateAllLegalMoves();
    if(possiblemoves->size() == 0){
        if(underCheck){
            if(toMove == WHITE){
                result = GameResult::BLACK_WIN;
            }
            else{
                result = GameResult::WHITE_WIN;
            }
        }
        else{
            result = GameResult::DRAW;
        }
    }
    delete possiblemoves;
    if(result != GameResult::ONGOING){
        return;
    }
    int sqmaterial[2] = {0, 0}; // on white and black squares
    int colmaterial[3] = {0, 0, 0};
    for(int i = 0; i < 64; i++){
        int ind = mailbox[i];
        sqmaterial[ind & 1] += std::abs(drawMaterial[board->squares[ind] + SYMBOLS_OFFSET]);
        colmaterial[board->GetSquareColor(ind) + 1] += drawMaterial[board->squares[ind] + SYMBOLS_OFFSET];
        //std::cout << "on   " << (ind & 1) << "   added  " << drawMaterial[_board->squares[ind] + SYMBOLS_OFFSET] << "   by   " << board->squares[ind] <<  "   which is    "  <<  PiecesSymbols[_board->squares[ind] + SYMBOLS_OFFSET] << "   on place   " << Ind2Not(ind) << '\n';
    }
    if(colmaterial[BLACK + 1] + colmaterial[WHITE + 1] < 2){
        result = DRAW;
        return;
    }
    if(colmaterial[BLACK + 1] + colmaterial[WHITE + 1] == 2 && colmaterial[BLACK + 1] == colmaterial[WHITE + 1]){
        if(sqmaterial[0] == sqmaterial[1]){
            bool isknight = false;
            for(int i = 0; i < 64; i++){
                int ind = mailbox[i];
                if(board->squares[ind] == WHITE_KNIGHT || board->squares[ind] == BLACK_KNIGHT){
                    isknight = true;
                }
            }
            if(isknight == false){
                result = DRAW;
            }
        }
    }
}

void Node::CalculatePositionHash(){
    long long value = toMove + 313;
    for(int i = 0; i < 64; i++){
        value *= 313;
        value += board->GetSquareValue(mailbox[i]) + NO_PIECES;
        value %= 1000000009 + 7;
    }
    positionHash = value;
}