#include "Player.hpp"
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;

std::ostream & operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
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
            for(int i = 0; i < hand.size(); i++) {
                if(hand[i].get_suit(upcard.get_suit()) == upcard.get_suit() && hand[i].is_face_or_ace()){
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
                return true;
            }
            for(int i = 0; i < hand.size(); i++) {
                if((Suit_next(hand[i].get_suit(upcard.get_suit())) == upcard.get_suit()) && hand[i].is_face_or_ace()){
                    count++;
                } else if(hand[i].is_left_bower(upcard.get_suit())) {
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
                play = hand[i];
                playIndex = i;
            }
            if(canFollow) {
                if(hand[i].get_rank() > play.get_rank() && hand[i].get_suit() == led_card.get_suit()) {
                    play = hand[i];
                    playIndex = i;
                }
                if(led_card.get_suit() == trump){
                    bool rb = false;
                    for (int i = 0; i < hand.size(); i++) {
                        if(hand[i].is_right_bower(trump)) {
                            rb = true;
                            play = hand[i];
                            playIndex = i;
                        }
                        if(!rb) {
                            if(hand[i].is_left_bower(trump)) {
                            play = hand[i];
                            playIndex = i;
                            }
                        }
                    }
                }
            }
        }
        if(!canFollow) {
            for(int i = 0; i < hand.size(); i++) {
                if(Card_less(hand[i], play, trump)) {
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



class Human : public Player{
    public:

    Human(string nameIn) : name(nameIn) {}

    const std::string & get_name() const override{
        return name;
    }
    
    void add_card(const Card &c) override {
        hand.push_back(c);
    }
    
    bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override {
            print_hand();
            if(round == 2 && is_dealer) {
                cout << "Human player " << name << ", please enter a suit:\n";
                string decision;
                cin >> decision;
                order_up_suit = string_to_suit(decision);
                return true;
            }
            cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
            string decision;
            cin >> decision;
            if(decision != "pass") {
                order_up_suit = string_to_suit(decision);
                return true;
            } else {
                return false;
            }
    }

    void add_and_discard(const Card &upcard) override {
        sort(hand.begin(), hand.end());
        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name << ", please select a card to discard:\n";
        int discard;
        cin >> discard;
        if(discard == -1) {
            return;
        } else {
            hand[discard] = upcard;
        }
    }

    Card lead_card(Suit trump) override {
        print_hand();
        cout << "Human player " << name << ", please select a card:\n";
        int selected;
        cin >> selected;
        Card temp = hand[selected];
        hand.erase(hand.begin() + selected);
        return temp;
    }

    Card play_card(const Card &led_card, Suit trump) override {
        print_hand();
        cout << "Human player " << name << ", please select a card:\n";
        int selected;
        cin >> selected;
        Card temp = hand[selected];
        hand.erase(hand.begin() + selected);
        return temp;
    }
    
    
    private:
    std::string name;
    vector<Card> hand;
    void print_hand() const {
  for (size_t i=0; i < hand.size(); ++i)
    cout << "Human player " << name << "'s hand: "
         << "[" << i << "] " << hand[i] << "\n";
}
};

Player * Player_factory(const std::string &name, const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new Simple(name);
  }
  // Repeat for each other type of Player
    if (strategy == "Human"){
        return new Human(name);
    }
  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}
