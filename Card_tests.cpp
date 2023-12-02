// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(Card::RANK_ACE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c.get_suit());
}

TEST(test_card_suit_and_rank) {
    Card two_spades = Card();
    ASSERT_EQUAL(two_spades.get_rank(), Card::RANK_TWO);
    ASSERT_EQUAL(two_spades.get_suit(), Card::SUIT_SPADES);
    
    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(jack_diamonds.get_rank(), Card::RANK_JACK);
    ASSERT_EQUAL(jack_diamonds.get_suit(), Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(jack_diamonds.get_suit(Card::SUIT_HEARTS), Card::SUIT_HEARTS);
    ASSERT_EQUAL(jack_diamonds.get_suit(Card::SUIT_CLUBS), Card::SUIT_DIAMONDS);
}

TEST(test_card_type) {
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_TRUE(jack_spades.is_face());
    ASSERT_FALSE(jack_spades.is_right_bower(Card::SUIT_CLUBS));
    ASSERT_TRUE(jack_spades.is_right_bower(Card::SUIT_SPADES));
    ASSERT_FALSE(jack_spades.is_left_bower(Card::SUIT_SPADES));
    ASSERT_TRUE(jack_spades.is_left_bower(Card::SUIT_CLUBS));
    ASSERT_FALSE(jack_spades.is_trump(Card::SUIT_HEARTS));
    ASSERT_TRUE(jack_spades.is_trump(Card::SUIT_CLUBS));
    ASSERT_TRUE(jack_spades.is_trump(Card::SUIT_SPADES));
    
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_FALSE(three_spades.is_face());
    ASSERT_FALSE(three_spades.is_right_bower(Card::SUIT_CLUBS));
    ASSERT_FALSE(three_spades.is_left_bower(Card::SUIT_CLUBS));
    ASSERT_FALSE(three_spades.is_trump(Card::SUIT_CLUBS));
}

TEST(test_card_comparison) {
    //Self comparison
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_FALSE(three_spades < three_spades);
    ASSERT_FALSE(three_spades > three_spades);
    ASSERT_TRUE(three_spades == three_spades);
    ASSERT_FALSE(three_spades != three_spades);
    
    Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
    Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card eight_spades = Card(Card::RANK_EIGHT, Card::SUIT_SPADES);
    ASSERT_TRUE(eight_spades < queen_spades);
    ASSERT_TRUE(eight_spades < queen_hearts);
    ASSERT_TRUE(queen_spades < queen_hearts);
    ASSERT_FALSE(eight_spades > queen_spades);
    ASSERT_FALSE(eight_spades > queen_hearts);
    ASSERT_FALSE(queen_spades > queen_hearts);
    ASSERT_TRUE(eight_spades != queen_hearts);
}

TEST(test_Suit_next) {
    ASSERT_EQUAL(Suit_next(Card::SUIT_CLUBS), Card::SUIT_SPADES);
    ASSERT_EQUAL(Suit_next(Card::SUIT_SPADES), Card::SUIT_CLUBS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_DIAMONDS), Card::SUIT_HEARTS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_HEARTS), Card::SUIT_DIAMONDS);
}

TEST(test_card_insertion) {
    Card seven_spades = Card(Card::RANK_SEVEN, Card::SUIT_SPADES);
    ostringstream test;
    test << seven_spades;
    ASSERT_EQUAL(test.str(), "Seven of Spades");
}

TEST(test_card_less_1) {
    Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
    Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card eight_spades = Card(Card::RANK_EIGHT, Card::SUIT_SPADES);
    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    //all trump cards are more valuable than non-trump cards
    ASSERT_FALSE(Card_less(eight_spades, queen_hearts, Card::SUIT_SPADES));
    //right bower larger than left bower
    ASSERT_FALSE(Card_less(jack_diamonds, jack_hearts, Card::SUIT_DIAMONDS));
    //left bower is larger than non-bower trump suit cards
    ASSERT_FALSE(Card_less(jack_diamonds, queen_hearts, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(queen_spades, eight_spades, Card::SUIT_DIAMONDS));
    ASSERT_FALSE(Card_less(queen_hearts, eight_spades, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(queen_hearts, queen_spades, Card::SUIT_DIAMONDS));
}

TEST(test_card_less_2) {
    Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
    Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card eight_spades = Card(Card::RANK_EIGHT, Card::SUIT_SPADES);
    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    //all trump cards are more valuable than non-trump cards
    ASSERT_FALSE(Card_less(eight_spades, queen_hearts,
                           eight_spades, Card::SUIT_SPADES));
    //right bower larger than left bower
    ASSERT_FALSE(Card_less(jack_diamonds, jack_hearts,
                           jack_diamonds, Card::SUIT_DIAMONDS));
    //left bower is larger than non-bower trump suit cards
    ASSERT_FALSE(Card_less(jack_diamonds, queen_hearts,
                           queen_hearts, Card::SUIT_HEARTS));
    ASSERT_FALSE(Card_less(queen_spades, eight_spades,
                           eight_spades, Card::SUIT_DIAMONDS));
    ASSERT_FALSE(Card_less(queen_hearts, eight_spades,
                           jack_clubs, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(queen_hearts, queen_spades,
                           jack_diamonds, Card::SUIT_DIAMONDS));
    //all cards of the led suit are considered more valuable than
    //all non-trump-suit, non-led-suit cards
    ASSERT_FALSE(Card_less(eight_spades, queen_hearts,
    queen_spades, Card::SUIT_DIAMONDS));
    //led-suit cards still follow normal comparison rules
    ASSERT_FALSE(Card_less(queen_spades, eight_spades,
    queen_spades, Card::SUIT_DIAMONDS));
    //led-suit cards are smaller than trump-suit cards
    ASSERT_FALSE(Card_less(eight_spades, jack_diamonds,
                           jack_diamonds, Card::SUIT_SPADES));
}
TEST_MAIN()
