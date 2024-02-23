#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_insertion) {
  // Create a Human player
  Player * human = Player_factory("NotRobot", "Human");

  // Print the player using the stream insertion operator
  ostringstream oss1;
  oss1 << * human;

  // Verify that the output is the player's name
  ASSERT_EQUAL(oss1.str(), "NotRobot");

  // Create a Simple player
  Player * alice = Player_factory("Alice", "Simple");

  // Print the player using the stream insertion operator
  ostringstream oss2;
  oss2 << *alice;
  ASSERT_EQUAL(oss2.str(), "Alice");

  // Clean up players that were created using Player_factory()
  delete human;
  delete alice;
}

TEST(test_player_get_name) {
  // Create a player and verify that get_name() returns the player's name
  Player * alice = Player_factory("Alice", "Simple");
  ASSERT_EQUAL(alice->get_name(), "Alice");
  delete alice;

  Player * blank = Player_factory("", "Simple");
  ASSERT_EQUAL(alice->get_name(), "");
  delete blank;
  
}

TEST(test_simple_player_make_trump) {
  // Basic
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob makes tump
  Card nine_spades(NINE, SPADES);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_spades,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, SPADES);

  delete bob;
  
  
  //first round with bower
  Player * jim = Player_factory("Jim", "Simple");
  jim->add_card(Card(JACK, HEARTS));
  jim->add_card(Card(QUEEN, DIAMONDS));
  jim->add_card(Card(QUEEN, SPADES));
  jim->add_card(Card(KING, SPADES));
  jim->add_card(Card(ACE, SPADES));

  // Jim makes tump
  Card nine_diamonds(NINE, DIAMONDS);
  orderup = jim->make_trump(
    nine_diamonds,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, DIAMONDS);

  delete jim;

  
  //2nd round no order up
  Player * tim = Player_factory("tim", "Simple");
  tim->add_card(Card(TEN, HEARTS));
  tim->add_card(Card(QUEEN, DIAMONDS));
  tim->add_card(Card(QUEEN, CLUBS));
  tim->add_card(Card(KING, SPADES));
  tim->add_card(Card(NINE, SPADES));

  // Tim makes trump
  orderup = tim->make_trump(
    nine_diamonds,    // Upcard
    false,           // Tim is also the dealer
    2,              // Second round
    trump           // Suit ordered up (if any)
  );

  // Verify Tim's order up and trump suit
  ASSERT_FALSE(orderup);

  orderup = tim->make_trump(
    nine_diamonds,    // Upcard
    true,           // Tim is also the dealer
    2,              // Second round
    trump           // Suit ordered up (if any)
  );

  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, HEARTS);

  delete tim;
  
  
  //second round order with right bower
  Player * fin = Player_factory("fin", "Simple");
  fin->add_card(Card(JACK, HEARTS));
  fin->add_card(Card(QUEEN, DIAMONDS));
  fin->add_card(Card(QUEEN, SPADES));
  fin->add_card(Card(KING, SPADES));
  fin->add_card(Card(ACE, SPADES));

  // fin makes tump
  orderup = fin->make_trump(
    nine_diamonds,    // Upcard
    false,           // fin is also the dealer
    2,              // Second round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, HEARTS);

  delete fin;
}

TEST(test_simple_player_lead_card) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob adds a card to his hand and discards one card
  bob->add_and_discard(
    Card(NINE, HEARTS) // upcard
  );

  // Bob leads
  Card card_led = bob->lead_card(HEARTS);

  // Verify the card Bob selected to lead
  Card ace_spades(ACE, SPADES);
  ASSERT_EQUAL(card_led, ace_spades); //check led card

  delete bob;
}

TEST(test_simple_player_play_card) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob plays a card
  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,  // Nine of Diamonds is led
    HEARTS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));
  delete bob;
}
TEST(test_play_card) {
  
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  
  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(nine_diamonds,HEARTS); 
  Card ace_hearts(ACE, HEARTS);
  Card next_played = bob->play_card(ace_hearts,SPADES);
  Card card_three = bob->play_card(ace_hearts,SPADES);
  Card card_four = bob->play_card(Card(NINE,HEARTS),HEARTS);
  Card card_five = bob->play_card(Card(JACK,SPADES),SPADES);
  
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));
  ASSERT_EQUAL(next_played, Card(TEN,SPADES));
  ASSERT_EQUAL(card_three, Card(QUEEN,SPADES));
  ASSERT_EQUAL(card_four, Card(KING,SPADES));
  ASSERT_EQUAL(card_five, Card(ACE,SPADES));
  delete bob;
}
TEST(test_lead_card){
 Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, HEARTS));

  Card one = bob->lead_card(SPADES);
  Card two = bob->lead_card(HEARTS);
  Card three = bob->lead_card(SPADES);
  Card four = bob->lead_card(SPADES);
  Card five = bob->lead_card(SPADES);
 

  ASSERT_EQUAL(one , Card(ACE,HEARTS));
  ASSERT_EQUAL(two, Card(KING,SPADES));
  ASSERT_EQUAL(three, Card(QUEEN,HEARTS));
  ASSERT_EQUAL(four, Card(TEN,SPADES));
  ASSERT_EQUAL(five, Card(NINE,SPADES));
  delete bob;
}
TEST(add_and_discard){
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, HEARTS));


  bob->add_and_discard(Card(JACK,HEARTS));
  Card played = bob->play_card(Card(KING,HEARTS), HEARTS);
  Card playedTwo = bob->play_card(Card(KING,HEARTS), HEARTS);
  Card playedThree = bob->play_card(Card(KING,HEARTS), HEARTS);
  Card playedFour = bob->play_card(Card(KING,HEARTS), HEARTS);
  Card playedFive = bob->play_card(Card(KING,HEARTS), HEARTS);

  ASSERT_EQUAL(played, Card(JACK,HEARTS));
  ASSERT_EQUAL(playedTwo, Card(ACE,HEARTS));
  ASSERT_EQUAL(playedThree, Card(QUEEN,HEARTS));
  ASSERT_EQUAL(playedFour, Card(KING,SPADES));
  ASSERT_EQUAL(playedFive, Card(TEN,SPADES));

  
  Player * jim = Player_factory("jim", "Simple");
  jim->add_card(Card(NINE, SPADES));
  jim->add_card(Card(TEN, SPADES));
  jim->add_card(Card(QUEEN, SPADES));
  jim->add_card(Card(KING, SPADES));
  jim->add_card(Card(ACE, SPADES));
  jim->add_and_discard(Card(JACK,SPADES));
  
  played = bob->play_card(Card(KING,HEARTS), HEARTS);
  playedTwo = bob->play_card(Card(KING,HEARTS), HEARTS);
  playedThree = bob->play_card(Card(KING,HEARTS), HEARTS);
  playedFour = bob->play_card(Card(KING,HEARTS), HEARTS);
  playedFive = bob->play_card(Card(KING,HEARTS), HEARTS);

}

// Add more tests here

TEST_MAIN()
