#include "position.h"
#include "functions.h"
#include "move.h"
#include "movecheck.h"
#include "movegenerators.h"
#include <cmath>

const int mailbox[64] = {22, 23, 24, 25, 26, 27, 28, 29,
                         32, 33, 34, 35, 36, 37, 38, 39,
                         42, 43, 44, 45, 46, 47, 48, 49,
                         52, 53, 54, 55, 56, 57, 58, 59,
                         62, 63, 64, 65, 66, 67, 68, 69,
                         72, 73, 74, 75, 76, 77, 78, 79,
                         82, 83, 84, 85, 86, 87, 88, 89,
                         92, 93, 94, 95, 96, 97, 98, 99};

const int drawMaterial[NO_PIECES] = {0, 10, 1, 1, 10, 10, 0, 10, 10, 1, 1, 10, 0};

Position::Position(){
    toMove = WHITE;
    whiteKingPos = Not2Ind("e1");
    blackKingPos = Not2Ind("e8");
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

Position::Position(Position& pr, Move *m, int promo){
    prev = &pr;

    std::copy(prev->squares, prev->squares + 122, squares);
    whiteKingPos = pr.whiteKingPos;
    blackKingPos = pr.blackKingPos;
    toMove = prev->toMove * (-1);
    whcstl = prev->whcstl;
    blcstl = prev->blcstl;

    squares[m->To()] = squares[m->From()];
    squares[m->From()] = EMPTY_SQUARE;
    if(m->Type() & EN_PASSANT_MOVE){
        if(column(m->From()) < column(m->To())){
            squares[m->From() + 1] = 0;
        }
        else{
            squares[m->From() - 1] = 0;
        }
    }
    if(m->Type() & SHORT_CASTLE_MOVE){
        squares[m->To() - 1] = squares[m->To() + 1];
        squares[m->To() + 1] = EMPTY_SQUARE;
    }
    if(m->Type() & LONG_CASTLE_MOVE){
        squares[m->To() + 1] = squares[m->To() - 2];
        squares[m->To() - 2] = EMPTY_SQUARE;
    }
    if(m->Type() & PROMOTION_MOVE){
        if(squares[m->To()] < 0){
            squares[m->To()] = -promo;
        }
        else{
            squares[m->To()] = promo;
        }
    }

    if(whcstl & SHORT_CASTLE_MOVE){
        if(m->From() == Not2Ind("e1") || GetSquareValue(Not2Ind("h1")) != WHITE_ROOK){
            whcstl -= SHORT_CASTLE_MOVE;
        }
    }
    if(whcstl & LONG_CASTLE_MOVE){
        if(m->From() == Not2Ind("e1") || GetSquareValue(Not2Ind("a1")) != WHITE_ROOK){
            whcstl -= LONG_CASTLE_MOVE;
        }
    }
    if(blcstl & SHORT_CASTLE_MOVE){
        if(m->From() == Not2Ind("e8") || GetSquareValue(Not2Ind("h8")) != BLACK_ROOK){
            blcstl -= SHORT_CASTLE_MOVE;
        }
    }
    if(blcstl & LONG_CASTLE_MOVE){
        if(m->From() == Not2Ind("e8") || GetSquareValue(Not2Ind("a8")) != BLACK_ROOK){
            blcstl -= LONG_CASTLE_MOVE;
        }
    }

    if(m->Type() & PAWN_DOUBLE_MOVE)
        enPassant = (m->From() + m->To()) / 2;
    else
        enPassant = -1;
    if(m->From() == whiteKingPos){
        whiteKingPos = m->To();
    }
    if(m->From() == blackKingPos){
        blackKingPos = m->To();
    }
    fullMoveCounter = prev->fullMoveCounter;
    if(toMove == WHITE){
        fullMoveCounter++;
    }
    if(m->Type() < PAWN_MOVE){ // only regular moves and castles (no captures)
        halfMoveClock = prev->halfMoveClock + 1;
        searchBackRepetitions = true;
    }
    else{
        halfMoveClock = 0;
        searchBackRepetitions = false;
    }
    CheckCheck();
    CalculatePositionHash();
    CheckEndings();
}

Position::Position(std::string fen){
    while(fen[0] == ' '){
        fen.erase(0, 1);
    }
    long unsigned int ind = fen.find(' ');
    std::string fentemp = fen.substr(0, ind);
    fen.erase(0, ind + 1);

    unsigned int index = 0;
    for(int i = 7; i >= 0; i--){
        int j = 0;
        while(fentemp[index] != '/' && index < fentemp.length()){
            switch(fentemp[index]){
                case 'K':
                    squares[mailbox[8 * i + j]] = WHITE_KING;
                    whiteKingPos = mailbox[8 * i + j];
                    break;
                case 'k':
                    squares[mailbox[8 * i + j]] = BLACK_KING;
                    blackKingPos = mailbox[8 * i + j];
                    break;
                case 'Q':
                    squares[mailbox[8 * i + j]] = WHITE_QUEEN;
                    break;
                case 'q':
                    squares[mailbox[8 * i + j]] = BLACK_QUEEN;
                    break;
                case 'R':
                    squares[mailbox[8 * i + j]] = WHITE_ROOK;
                    break;
                case 'r':
                    squares[mailbox[8 * i + j]] = BLACK_ROOK;
                    break;
                case 'B':
                    squares[mailbox[8 * i + j]] = WHITE_BISHOP;
                    break;
                case 'b':
                    squares[mailbox[8 * i + j]] = BLACK_BISHOP;
                    break;
                case 'N':
                    squares[mailbox[8 * i + j]] = WHITE_KNIGHT;
                    break;
                case 'n':
                    squares[mailbox[8 * i + j]] = BLACK_KNIGHT;
                    break;
                case 'P':
                    squares[mailbox[8 * i + j]] = WHITE_PAWN;
                    break;
                case 'p':
                    squares[mailbox[8 * i + j]] = BLACK_PAWN;
                    break;
                default:;
            }
            if(isdigit(fentemp[index])){
                int x = (int)(fentemp[index]) - '0';
                for(; x> 0; x--){
                    squares[mailbox[8 * i + j]] = EMPTY_SQUARE;
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
    fen[0] == 'w'? toMove = WHITE : toMove = BLACK;
    fen.erase(0, 2);
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
    fen[0] == '-' ? enPassant = -1 : enPassant = Not2Ind(fen.substr(0, 2));
    fen.erase(0,fen.find(' ') + 1);
    if(fen[1] != ' '){
        halfMoveClock = 10 * ((int)(fen[0]) - '0') + (int)(fen[1]) - '0';
    }
    else{
        halfMoveClock = (int)(fen[0]) - '0';
    }
    fen.erase(0, fen.find(' ') + 1);
    fullMoveCounter = 1;
    for(uint i = 0; i < fen.length(); i++){
        fullMoveCounter *= 10;
        fullMoveCounter += (int)(fen[i]) - '0';
    }
    searchBackRepetitions = false;
    CalculatePositionHash();
    CheckCheck();
    CheckEndings();
    prev = NULL;
    result = GameResult::ONGOING;
}

Position::~Position(){}

void Position::ShowBoard() const{
    std::cout << std::endl;
    std::cout << "  +---+---+---+---+---+---+---+---+\n";
    for(int row = 8; row > 0; row--){
        std::cout << row << " ";
        for(int col = 1; col <= 8; col++){
            std::cout << "| " << GetPiece(col, row) << " ";
        }
        std::cout << "|";
        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "    a   b   c   d   e   f   g   h\n\n";
    return;
}
std::ostream& Position::ShowTinyBoard(std::ostream& out) const{
    for(int row = 8; row > 0; row--){
        out << " ";
        for(int col = 1; col <= 8; col++){
            out << GetPiece(col, row);
        }
        out << '\n';
    }
    return out;
}

Position* Position::MakeMove(Move *toExecute){
    Move *expectedmove = CheckIfMoveFullLegal(toExecute);
    if(NULL != expectedmove)
    {
        Position *newposition = new Position(*this, expectedmove, toExecute->Promo());
        delete expectedmove;
        return newposition;
    }
    return NULL;
}

int Position::MakeSoftMove(Move *toExecute){
    int takenPiece = squares[toExecute->To()];
    squares[toExecute->To()] = squares[toExecute->From()];
    squares[toExecute->From()] = EMPTY_SQUARE;
    if(toExecute->Type() & EN_PASSANT_MOVE){
        if(toMove == WHITE){
            squares[enPassant - 10] = EMPTY_SQUARE;
        }
        else{
            squares[enPassant + 10] = EMPTY_SQUARE;
        }
    }
    if(toExecute->From() == whiteKingPos){
        whiteKingPos = toExecute->To();
    }
    if(toExecute->From() == blackKingPos){
        blackKingPos = toExecute->To();
    }
    return takenPiece;
}

void Position::MakeSoftBack(Move *toExecute, int takenPiece){
    squares[toExecute->From()] = squares[toExecute->To()];
    squares[toExecute->To()] = takenPiece;
    if(toExecute->Type() & EN_PASSANT_MOVE){
        if(toMove == WHITE){
            squares[enPassant - 10] = BLACK_PAWN;
        }
        else{
            squares[enPassant + 10] = WHITE_PAWN;
        }
    }
    if(toExecute->To() == whiteKingPos){
        whiteKingPos = toExecute->From();
    }
    if(toExecute->To() == blackKingPos){
        blackKingPos = toExecute->From();
    }
}

Move* Position::CheckIfMoveFullLegal(Move* checkedmove, bool pseudoLegalWarranty){
    int PieceColor = GetSquareColor(checkedmove->From());
    if(toMove != PieceColor){
        std::cout << "wrong piece chosen (wrong color)\n";
        return NULL;
    }
    Move *expectedmove = NULL;
    if(pseudoLegalWarranty){
        expectedmove = new Move(*checkedmove);
    }
    else{
        expectedmove = CheckIfMovePseudoLegal(checkedmove->From(), checkedmove->To());
    }
    if((expectedmove->Type()) & PROMOTION_MOVE){
        if(checkedmove->Promo() == 0){
            delete expectedmove;
            std::cout << "no promo char entered\n";
            return NULL;
        }
    }
    if(NULL != expectedmove)
    {
        int takenPiece = MakeSoftMove(expectedmove);
        int kingPos = 0;
        toMove == WHITE ? kingPos = whiteKingPos : kingPos = blackKingPos;
        bool ownCheckAfter = false;
        if(InBetweenEmpty(*this, kingPos, expectedmove->From(), true, true) || row(kingPos) == row(expectedmove->From())){
            ownCheckAfter = IsPlaceAttacked(kingPos, -toMove);
        }
        MakeSoftBack(expectedmove, takenPiece);
        if(ownCheckAfter){
            delete expectedmove;
            return NULL;
        }
        return expectedmove;
    }
    return NULL;
}

Move* Position::CheckIfMovePseudoLegal(int from, int to) const{
    Move *expected = MoveCheckHandler::CheckMove(*this, from, to);
    return expected;
}

void Position::CheckCheck(){
    if(toMove == WHITE){
        if(IsPlaceAttacked(whiteKingPos, -toMove)){
            underCheck = true;
        }
        else{
            underCheck = false;
        }
    }
    else{
        if(IsPlaceAttacked(blackKingPos, -toMove)){
            underCheck = true;
        }
        else{
            underCheck = false;
        }
    }
}

std::list<Move>* Position::GenerateAllLegalMoves(bool searchAtLeastOne){
    std::list<Move>* moves = AllMovesGenerator::GenerateMoves(*this);
    std::list<Move>* results = new std::list<Move>();
    Move *temp;
    auto it = moves->begin();
    bool found = false;
    while(it != moves->end()){
        if((temp = CheckIfMoveFullLegal(&(*it), false))){
            found = true;
            delete temp;
            results->push_back(*it);
        }
        if(found && searchAtLeastOne){
            delete moves;
            return results;
        }
        ++it;
    }
    delete moves;
    return results;
}

void Position::CheckEndings(){
    // 50-move rule
    if(halfMoveClock >= 100){
        result = GameResult::DRAW;
        return;
    }
    result = GameResult::ONGOING;

    // Repetition draw
    int count = 0;
    Position *current = this;
    while(true){
        if(current->positionHash == this->positionHash){
            count++;
            if(count>= 3){
                result = GameResult::DRAW;
                //std::cout << "draw (repetition)\n";
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
    std::list<Move>* possiblemoves = GenerateAllLegalMoves(true);
    if(possiblemoves->size() == 0){
        if(underCheck){ // Checkmate
            if(toMove == WHITE){
                result = GameResult::BLACK_WIN;
                //std::cout << "black win (checkmate)\n";
            }
            else{
                result = GameResult::WHITE_WIN;
                //std::cout << "white win (checkmate)\n";
            }
        }
        else{ // Stalemate
            result = GameResult::DRAW;
            //std::cout << "draw (stalemate)\n";            
        }
    }
    delete possiblemoves;
    if(result != GameResult::ONGOING){
        return;
    }

    //
    int sqmaterial[2] = {0, 0}; // on white and black squares
    int colmaterial[3] = {0, 0, 0};
    int col = 0;
    for(int i = 0; i < 64; i++){
        if(i % 8 != 0){
            col = 1 - col;
        }
        int ind = mailbox[i];
        sqmaterial[col & 1] += drawMaterial[squares[ind] + SYMBOLS_OFFSET];
        colmaterial[GetSquareColor(ind) + 1] += drawMaterial[squares[ind] + SYMBOLS_OFFSET];
    }
    if(colmaterial[BLACK + 1] + colmaterial[WHITE + 1] < 2){
        result = DRAW;
        //std::cout << "draw (material)\n";
        return;
    }
    if(colmaterial[BLACK + 1] + colmaterial[WHITE + 1] == 2 && colmaterial[BLACK + 1] == colmaterial[WHITE + 1]){
        if(sqmaterial[0] != sqmaterial[1]){
            bool isknight = false;
            for(int i = 0; i < 64; i++){
                int ind = mailbox[i];
                if(squares[ind] == WHITE_KNIGHT || squares[ind] == BLACK_KNIGHT){
                    isknight = true;
                }
            }
            if(isknight == false){
                result = DRAW;
                //std::cout << "draw (material)\n";
            }
        }
    }
} 

void Position::CalculatePositionHash(){
    long long value = toMove + 313;
    for(int i = 0; i < 64; i++){
        value *= 313;
        value += GetSquareValue(mailbox[i]) + NO_PIECES;
        value %= 1000000009 + 7;
    }
    positionHash = value;
}

char Position::GetPiece(int column, int row) const{
    //std::cerr << "Get piece     " << GetSquareValue(column, row) << "   " << column << "  " << row << std::endl;
    return GetPieceSymbol(GetSquareValue(column, row));
}

int Position::GetSquareValue(int column, int row) const{
    if(column < 1 || column > 8 || row < 1 || row > 8){
        std::cout << "function    Position::GetSquareValue error    " << column << " " << row << std::endl;
    }
    //std::cerr << "Get Square value    from   "  << 11 + 10 * row + column << "     " << squares[11 + 10 * row + column] << std::endl;
    return squares[11 + 10 * row + column];
}

int Position::GetSquareColor(int column, int row) const{
    int val = GetSquareValue(column, row);
    if(val == OUTSIDE_BOARD){
        return OUTSIDE_BOARD;
    }
    else{
        return sgn(val);
    }
}

char Position::GetPiece(int index) const{
    return GetPieceSymbol(squares[index]);
}

int Position::GetSquareValue(int index) const{
    return squares[index];
}

int Position::GetSquareColor(int index) const{
    int val = GetSquareValue(index);
    if(val == OUTSIDE_BOARD){
        return OUTSIDE_BOARD;
    }
    else{
        return sgn(squares[index]);
    }
}

bool Position::IsPlaceAttacked(int attackedplace, int attackingcolor) const{
    bool answer = false;
    for(int i = 0; i < 64 && answer == false; i++){
        int ind = mailbox[i];
        if(GetSquareColor(ind) == attackingcolor){
            Move *m = MoveCheckHandler::CheckMove(*this, ind, attackedplace);
            if(m != NULL){
                delete m;
                return true;
            }
        }
    }
    return answer;
}