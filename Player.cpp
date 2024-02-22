#include "Player.hpp"
#include <string>
#include <vector>
using namespace std;

std::ostream & operator<<(std::ostream &os, const Player &p) {
  assert(false);
}

class Simple : public Player{
    public:
    
    Simple(string nameIn) : name(nameIn) {}

    const string & get_name() const {
        return name;
    }

    void add_card(const Card &c){
        hand.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const {
        int count = 0;
        if(round == 1) {
            for(int i = 0; i < MAX_HAND_SIZE; i++) {
                if(hand[i].get_suit() == upcard.get_suit() && hand[i].is_face_or_ace()){
                    count++;
                }
            }
            if(count > 1) {
                order_up_suit = upcard.get_suit();
                return true;
            } else{
                return false;
            }
        } else {
            if(is_dealer) {
                order_up_suit = Suit_next(upcard.get_suit());
            }
            for(int i = 0; i < MAX_HAND_SIZE; i++) {
                if(Suit_next(hand[i].get_suit()) == upcard.get_suit() && hand[i].is_face_or_ace()){
                    count++;
                }
            }
            if(count > 0) {
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            } else {
                return false;
            }
        }
    }

    void add_and_discard(const Card &upcard){
        hand.push_back(upcard);
        Card min = hand[0];
        int minIndex = 0;
        for(int i = 1; i < 6; i++) {
            if(Card_less(hand[i], min, upcard.get_suit())) {
                min = hand[i];
                minIndex = i;
            }
        }
        hand.erase(hand.begin() + minIndex);
    }
    
    Card lead_card(Suit trump) {
        Card lead = hand[0];
        int leadIndex = 0;
        bool alltrump = true;
        for(int i = 0; i < hand.size(); i++) {
            if(hand[i].get_suit(trump) != trump) {
                alltrump = false;
            }
        }
        if(alltrump) {
            for(int i = 1; i < hand.size(); i++) {
                if(hand[i].get_rank() > lead.get_rank()) {
                    lead = hand[i];
                    leadIndex = i;
                }
            }
        } else {
            for(int i = 0; i < hand.size(); i++) {
                if(hand[i].get_suit(trump) != trump && hand[i].get_rank() > lead.get_rank()) {
                lead = hand[i];
                leadIndex = i;
                }
            }
        }
        hand.erase(hand.begin() + leadIndex);
        return lead;
    }

    Card play_card(const Card &led_card, Suit trump) {
        bool canFollow = false;
        Card play = hand[0];
        int playIndex = 0;
        for(int i = 0; i < hand.size(); i++) {
            if(hand[i].get_suit(trump) == led_card.get_suit(trump)){
                canFollow = true;
                if(hand[i].get_rank() > play.get_rank() || play.get_suit() == trump) {
                    play = hand[i];
                    playIndex = i;
                }
            }
        }
        if(!canFollow) {
            for(int i = 0; i < hand.size(); i++) {
                if(hand[i].get_rank() < play.get_rank()) {
                    play = hand[i];
                    playIndex = i;
                }
            }
        }
        Card temp = play;
        hand.erase(hand.begin() + playIndex);
        return temp;
    }

private:
string name;
vector<Card> hand;  
};

/*class Human : public Player{
    public:
    const std::string & get_name() const override{
        return name;
    }
    void add_card(const Card &c){
        hand.push_back(c);
    }
    bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit){
        cout << "Human player " << name << ", order a trump suit or pass";
        string decision;
        cin >> decision;
        if(decision == "Spades" || decision == "Hearts"
         || decision == "Clubs" || decision == "Diamonds") {
            order_up_suit = string_to_suit(decision);
            return true;
        } else {
            return false;
        }
    }
    
    
    private:
    std::string name;
    vector<Card> hand;
};
*/

Player * Player_factory(const std::string &name, const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new Simple(name);
  }
  // Repeat for each other type of Player
    /*if (strategy == "Human"){
        return new Human(name);
    }*/
  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}