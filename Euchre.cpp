#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <cstring>
#include "Player.hpp"
#include "Pack.hpp"

using namespace std;



class Game {
   private:
  std::vector<Player*> players;
  Pack pack;
  int pointsNeeded;
  Suit trump;
  bool maybeShuffle;
  std::vector<Player*> original;
  int points1 = 0;
  int points2 = 0;

 public:
  Game(Player* p0, Player* p1, Player* p2, Player* p3, int points, ifstream& ifs, bool isShuffle){
    players.push_back(p0);
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(p3);
    original = players;
    pointsNeeded = points;
    points1 = 0;
    points2 = 0;
    pack = Pack(ifs);
    maybeShuffle = isShuffle;
    if(isShuffle){
      pack.shuffle();
    }
  }
  
  
  bool play(int points){
  int orderIndex;
  int totalHands = 0;
  bool evenRound = true;
  std::vector<Player*> copy;
  for(int i = 0; i < 4 ; i++){
    copy.push_back(players[i]);
  }
  while(points1 < points && points2 < points) {
    cout << "Hand " << totalHands << endl;
    if(totalHands > 0){
        players[0] = copy[1];
        players[1] = copy[2];
        players[2] = copy[3];
        players[3] = copy[0];
      copy = players;
      pack.reset();
      if(maybeShuffle){
        pack.shuffle();
      }
    }
    cout << players[0]->get_name() << " deals" << endl;
    threeDeal(players[1]);
    twoDeal(players[2]);
    threeDeal(players[3]);
    twoDeal(players[0]);
    twoDeal(players[1]);
    threeDeal(players[2]);
    twoDeal(players[3]);
    threeDeal(players[0]);
    orderIndex = set_trump();
    play_hand(trump, orderIndex, evenRound);
    if(evenRound) {
      evenRound = false;
    } else {
      evenRound = true;
    }
    totalHands++;
    cout << original[0]->get_name() << " and " << original[2]->get_name() << " have " << points1 << " points" << endl;
    cout << original[1]->get_name() << " and " << original[3]->get_name() << " have " << points2 << " points" << endl << endl; 
  }
  if(points1 > points2) {
    return true;
  } else {
    return false;
  }
  
}

 void twoDeal(Player* player) {
  player->add_card(pack.deal_one());
  player->add_card(pack.deal_one());
 }
 void threeDeal(Player* player){
  player->add_card(pack.deal_one());
  player->add_card(pack.deal_one());
  player->add_card(pack.deal_one());
 }
int set_trump(){
  Card upCard = pack.deal_one();
  cout << upCard.get_rank() << " of " << upCard.get_suit() << " turned up" << endl;
  Suit trumpSuit;
  bool dealer = false;
  int placement = 1;
  int currentRound = 1;
  bool trumpMade = false;
  bool nextRound = false;

  while(!trumpMade){
    trumpMade = players[placement]->make_trump(upCard, dealer, currentRound, trumpSuit);
    if(nextRound) {
      currentRound = 2;
    }
    if (!trumpMade){
      cout << players[placement]->get_name() << " passes" << endl;
      placement++;
      if(placement != 4){
        dealer = false;
      } else {
        dealer = true;
      }
      if(placement == 4) {
        placement = 0;
        nextRound = true;
      }
    }
  }

  if(currentRound == 1) {
    players[0]->add_and_discard(upCard);
  }
  trump = trumpSuit;
  cout << players[placement]->get_name() << " orders up " << trump << endl << endl;
  return placement;
}

void play_trick(Suit trump, int& p1, int& lead, int orderIndex){
  Card leadCard = players[lead]->lead_card(trump);
  cout << leadCard << " led by " << players[lead]->get_name() << endl;
  vector<Card> cards;
  cards.push_back(leadCard);
  for(int i = 1; i < 4; i++) {
    cards.push_back(players[(lead + i) % 4]->play_card(leadCard, trump));
    cout << cards[i] << " played by " << players[(lead + i) % 4]->get_name() << endl;
  }
  Card high = leadCard;
  int leadIndex = 0;
  for(int i = 1; i < 4; i++) {
    if(Card_less(high, cards[i], leadCard, trump)) {
      high = cards[i];
      leadIndex = i;
    }
  }
  lead = (lead + leadIndex) % 4;
  cout << players[lead]->get_name() << " takes the trick" << endl << endl;
  //Who ordered!!!!!
  if(players[lead] == players[orderIndex] || players[(lead + 2) % 4] == players[orderIndex]) {
    p1++;
  }
}

void play_hand(Suit trump, int orderIndex, bool evenRound) {
  int handsWon = 0;
  int lead = 1;
  for(int i = 0; i < 5; i++) {
      play_trick(trump, handsWon, lead, orderIndex);
  }
  awardPoint(orderIndex, handsWon);
}
void awardPoint(int orderIndex, int handsWon) {
  int indexTracker = -1;
  for(int i = 0; i < 4; i++){
    if(players[orderIndex]->get_name() == original[i]->get_name()){
      indexTracker = i;
    }
  }
  if(indexTracker > 2) {
    indexTracker -= 2;
  }
  if(indexTracker == 0) {
    if(handsWon < 3) {
      cout << original[1]->get_name() << " and " << original[3]->get_name() << " win the hand" << endl;
      cout << "euchred!" << endl;
      points2 += 2;
    } else if(handsWon == 5) {
      cout << original[0]->get_name() << " and " << original[2]->get_name() << " win the hand" << endl;
      cout << "march!" << endl;
      points1 += 2;
    } else {
      cout << original[0]->get_name() << " and " << original[2]->get_name() << " win the hand" << endl;
      points1 += 1;
    }
  } else {
    if(handsWon < 3) {
      cout << original[0]->get_name() << " and " << original[2]->get_name() << " win the hand" << endl;
      cout << "euchred!" << endl;
      points1 += 2;
    } else if(handsWon == 5) {
      cout << original[1]->get_name() << " and " << original[3]->get_name() << " win the hand" << endl;
      cout << "march!" << endl;
      points2 += 2;
    } else {
      cout << original[1]->get_name() << " and " << original[3]->get_name() << " win the hand" << endl;
      points2 += 1;
    }
  }
}


};

int main(int argc, char **argv) {
   Player* p0 = Player_factory(argv[4], argv[5]);
   Player* p1 = Player_factory(argv[6], argv[7]);
   Player* p2 = Player_factory(argv[8], argv[9]);
   Player* p3 = Player_factory(argv[10], argv[11]);
  bool shuffle;
  int points = stoi(argv[3]);
  
  if(argc != 12) {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
    return 1;
   }
   
   if(points < 1 || points > 100) {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
    return 1;
  }
  if(strcmp(argv[2],"shuffle") == 0) {
    shuffle = true;
  } else if(  strcmp(argv[2],"noshuffle") == 0) {
    shuffle = false;
  } else {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
    return 1;
  }

  for(int i = 5; i < argc; i += 2) {
    if(strcmp(argv[1],"Human") != 0 && strcmp(argv[i],"Simple") != 0) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
        return 1;
    }
  }
  string importedPack = argv[1];
   std::ifstream fin(importedPack);
   
  if(!fin.is_open()){
    cout << "Error opening " << importedPack << endl;
    return 1;
  }

  for(int i = 0; i < 12; i++) {
    cout << argv[i] << " ";
  }
  cout << endl;
  Game game(p0,p1,p2,p3,points,fin, shuffle);
  bool outcome = game.play(points);
  if (outcome){
    cout << endl << p0->get_name() << " and " << p2->get_name() << " win!" << endl;
  }else{
    cout << endl << p1->get_name() << " and " << p3->get_name() << " win!" << endl;
  }
}


