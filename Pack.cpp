#include "Pack.hpp"
#include "Card.hpp"
#include <array>
#include <string>
using namespace std;

Pack::Pack(){
    for(int i = 0; i < PACK_SIZE; i++) {
        if(i < 6) {
            cards[i] = Card(static_cast<Rank>(7 + i), string_to_suit("Spades"));
        } else if(i < 12){
            cards[i] = Card(static_cast<Rank>(1 + i), string_to_suit("Hearts"));
        } else if(i < 18){
            cards[i] = Card(static_cast<Rank>(-5 + i), string_to_suit("Clubs"));
        } else{
            cards[i] = Card(static_cast<Rank>(-12 + i), string_to_suit("Diamonds"));
        }
    }
    next = 0;
}

Pack::Pack(std::istream& pack_input) {
    Suit suit;
    string filler;
    Rank rank;
    for(int i = 0; i < PACK_SIZE; i++) {
        pack_input >> rank;
        pack_input >> filler;
        pack_input >> suit;
        cards[i] = Card(rank, suit);
    }
    next = 0;

}

Card Pack::deal_one(){
    Card temp = cards[next];
    next++;
    return temp;
}
void Pack::reset(){
    next = 0;
}

void Pack::shuffle(){
    Card temp [24];
    int place = 0;
    for(int j = 0; j < 7; j++){
        for(int i = 0; i < 24; i++) {
            temp[i] = cards[i];
        }
        for(int i = 0; i < 12; i++ ){
            cards[place] = temp[i + 12];
            place++;
            cards[place] = temp[i];
            place++;
        }
        place = 0;
    }
   
}

 bool Pack::empty() const{
 if (next == 24){
    return true;
 }else{
    return false;
 }
}
