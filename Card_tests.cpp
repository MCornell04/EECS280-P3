#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here
TEST(card_less_lessTrump_test){
   Card ace(ACE, CLUBS);
   Card aceSpades(ACE, SPADES);
   Card aceSpadesDupe(ACE,SPADES);
   Card leftBower(JACK, CLUBS);
   Card rightBower(JACK, SPADES);
   Card heartNine(NINE, HEARTS);
   Card heartTen(TEN,HEARTS);
   Card diamondTen(TEN, DIAMONDS);
   Card trumpNine(NINE, SPADES);
   Card kingTrump(KING, SPADES);
   Card kingNonTrump(KING,HEARTS);
   Card queen(QUEEN,SPADES);
   Card jackNonTrump(JACK,HEARTS);
   Suit trump = string_to_suit("Spades");
   ASSERT_TRUE(Card_less(ace,aceSpades,trump));
   ASSERT_TRUE(Card_less(leftBower,rightBower,trump));
   ASSERT_TRUE(Card_less(heartNine,trumpNine,trump));
   ASSERT_TRUE(Card_less(kingNonTrump,kingTrump,trump));
   ASSERT_TRUE(Card_less(queen,kingTrump,trump));
   ASSERT_TRUE(Card_less(aceSpades,leftBower, trump));
   ASSERT_TRUE(Card_less(aceSpades, rightBower, trump));
   ASSERT_TRUE(Card_less(ace,trumpNine,trump));
   ASSERT_TRUE(Card_less(kingNonTrump,queen,trump));
   ASSERT_TRUE(Card_less(heartTen,diamondTen,trump));
   //Above is wrong I believe, should be false
   ASSERT_FALSE(Card_less(queen, kingNonTrump,trump));
   ASSERT_FALSE(Card_less(trumpNine,kingNonTrump,trump));
   ASSERT_FALSE(Card_less(aceSpades,aceSpadesDupe,trump));
   ASSERT_FALSE(Card_less(trumpNine,heartNine,trump));
   ASSERT_TRUE(Card_less(jackNonTrump,aceSpades,trump));
     //NOW TESTING LEAD
   ASSERT_TRUE(Card_less(leftBower,rightBower,aceSpades,trump));
   ASSERT_FALSE(Card_less(rightBower,leftBower,aceSpades,trump));
   ASSERT_FALSE(Card_less(rightBower,jackNonTrump,kingNonTrump,trump));
   ASSERT_TRUE(Card_less(heartNine,aceSpades,trumpNine,trump));

}
TEST(mass_test_simple){
   Card ace(ACE, CLUBS);
   Card aceSpades(ACE, SPADES);
   Card leftBower(JACK, CLUBS);
   Card rightBower(JACK, SPADES);
   Card heartNine(NINE, HEARTS);
   Card kingNonTrump(KING,HEARTS);
   Card queen(QUEEN,SPADES);
   Suit trump = rightBower.get_suit();
   Card one(TEN,HEARTS);
   Card defaultCon; 






   ASSERT_EQUAL(ace.get_rank(),ACE);
   ASSERT_EQUAL(ace.get_suit(),CLUBS);
   ASSERT_EQUAL(leftBower.get_suit(trump), trump);
   ASSERT_TRUE(kingNonTrump.is_face_or_ace());
   ASSERT_FALSE(heartNine.is_face_or_ace());
   ASSERT_FALSE(one.is_face_or_ace()) ;
   ASSERT_FALSE(defaultCon.is_face_or_ace());
   ASSERT_TRUE(leftBower.is_face_or_ace());
   ASSERT_FALSE(heartNine.is_face_or_ace());
   ASSERT_TRUE(kingNonTrump.is_face_or_ace());
   ASSERT_TRUE(rightBower.is_right_bower(trump));
   ASSERT_FALSE(ace.is_left_bower(trump));
   ASSERT_TRUE(leftBower.is_left_bower(trump));
   ASSERT_TRUE(aceSpades.is_trump(trump));
   ASSERT_FALSE(heartNine.is_trump(trump));
   ASSERT_EQUAL(Suit_next(DIAMONDS),HEARTS);
   ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
   ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
   ASSERT_EQUAL(Suit_next(CLUBS), SPADES);

}
TEST(symbol_tests_simple){
   Card ace(ACE, CLUBS);
   Card aceClubsDupe(ACE,CLUBS);
   Card aceSpades(ACE, SPADES);
   Card aceSpadesTwo(ACE,SPADES);
   Card leftBower(JACK, CLUBS);
   Card rightBower(JACK, SPADES);
   Card heartNine(NINE, HEARTS);
   Card trumpNine(NINE, SPADES);
   Card kingTrump(KING, SPADES);
   Card queen(QUEEN,SPADES);
   Card jackNonTrump(JACK,HEARTS);
   


   ASSERT_TRUE(heartNine != ace);
   ASSERT_TRUE(aceSpades != ace);
   ASSERT_FALSE(aceSpades != aceSpadesTwo);
   ASSERT_TRUE(aceClubsDupe == ace);
   ASSERT_FALSE(leftBower == rightBower);
   ASSERT_FALSE(heartNine == trumpNine);
   ASSERT_TRUE(ace <= aceClubsDupe);
   ASSERT_TRUE(heartNine <= kingTrump);
   ASSERT_FALSE(rightBower <= heartNine);
   ASSERT_TRUE(ace >= aceClubsDupe);
   ASSERT_TRUE(aceClubsDupe >= ace);
   ASSERT_TRUE(queen >= jackNonTrump);
   ASSERT_TRUE(aceSpades < ace);
   ASSERT_TRUE(heartNine < kingTrump);
   ASSERT_FALSE(kingTrump < heartNine);
   ASSERT_TRUE(ace > aceSpades);
   ASSERT_TRUE(kingTrump > heartNine);
   ASSERT_FALSE(heartNine > kingTrump)


}
TEST(input_output_tests){
     Card ace(ACE, CLUBS);
     Rank sampleRank;
     Suit sampleSuit;
    ostringstream oss;
    oss << CLUBS;
    ASSERT_EQUAL(oss.str(), "Clubs");
    ostringstream ossTwo;
    ossTwo << ACE;
    ASSERT_EQUAL(ossTwo.str(),"Ace");
    istringstream iss("Ace\n");
    iss >> sampleRank;
    ASSERT_EQUAL(sampleRank, ACE);
    istringstream issTwo("Clubs\n");
    issTwo >> sampleSuit;
    ASSERT_EQUAL(sampleSuit, CLUBS);

    ostringstream ossThree;
    ossThree << ace;
    ASSERT_EQUAL(ossThree.str(), "Ace of Clubs");
    istringstream issThree("Nine of Clubs\n");
    Card newCard;
    Card nineDupe(NINE,CLUBS);
    issThree >> newCard;
    ASSERT_EQUAL(newCard, nineDupe);



}

TEST_MAIN()
