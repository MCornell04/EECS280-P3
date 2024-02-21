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
TEST(card_less_trumponly){
    Card a(ACE,HEARTS);
    Card atwo(ACE,HEARTS);
    Card rb(JACK,HEARTS);
    Card lb(JACK, DIAMONDS);
    Card k(KING, SPADES);
    Card q(QUEEN, CLUBS);
    Card ten(TEN, DIAMONDS);
    Card nine(NINE, CLUBS);
    Card ninetwo(NINE, HEARTS);
    Suit trump = rb.get_suit();

    //compare cards 
    ASSERT_TRUE(Card_less(a,rb,trump));
    ASSERT_TRUE(Card_less(lb,rb,trump));
    ASSERT_TRUE(Card_less(nine,ten,trump));
    ASSERT_TRUE(Card_less(nine,lb,trump));
    ASSERT_TRUE(Card_less(nine,rb,trump));
    ASSERT_TRUE(Card_less(a,rb,trump));
    ASSERT_TRUE(Card_less(a,lb,trump));
    
    //hopefully edge cases?
    ASSERT_FALSE(Card_less(a,atwo,trump));
    ASSERT_TRUE(Card_less(nine,ninetwo,trump));
    ASSERT_TRUE(Card_less(k,ninetwo,trump));
    ASSERT_FALSE(Card_less(ninetwo,k,trump));

}
TEST_MAIN()
