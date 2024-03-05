#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <fstream>
#include "Player.hpp"
#include "Pack.hpp"

using namespace std;

int main(int argc, char **argv) {
  // Read command line args and check for errors
   Player* p0 = Player_factory(argv[4], argv[5]);
   Player* p1 = Player_factory(argv[6], argv[7]);
   Player* p2 = Player_factory(argv[8], argv[9]);
   Player* p3 = Player_factory(argv[10], argv[11]);

  
  bool shuffle;
  string pointsNeeded = argv[3];
  int points = 0;
  for(int i = 0; i < pointsNeeded.size(); i++) {
    points *= 10;
    points += pointsNeeded[i];
  }
  
  if(argc != 12) {
    error();
    return 1;
   }
   
   if(points < 1 || points > 100) {
    error();
    return 1;
  }
  
  if(argv[2] == "shuffle") {
    shuffle = true;
  } else if(argv[2] == "noshuffle") {
    shuffle = false;
  } else {
    error();
    return 1;
  }

  for(int i = 5; i < argc; i + 2) {
    if(argv[i] != "Human" && argv[i] != "Simple") {
        error();
        return 1;
    }
  }
  string importedPack = argv[1];
   std::ifstream fin(importedPack);
   
  if(!fin.is_open()){
    cout << "Error opening " << importedPack << endl;
    return 1;
  }
   




   
  Game game(p0,p1,p2,p3,points,fin, shuffle);
  game.play();
}

class Game {
 public:
  Game(Player* p0, Player* p1, Player* p2, Player* p3, int points, ifstream& ifs, bool isShuffle){
    players.push_back(p0);
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(p3);
    pointsNeeded = points;
    pack = Pack(ifs);
    if(isShuffle){
      pack.shuffle();
    }else{

    }
  }
  void play(Player* p0, Player* p1, Player* p2, Player* p3, int points, ifstream& ifs, bool isShuffle){
    
  }

 private:
  std::vector<Player*> players;
  Pack pack;
  int pointsNeeded;
};



void error(){
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
}
