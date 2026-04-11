// PokerCalculatorOptimized.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Initial version based on Michael Shackleford's code

#include <iostream>
#include <time.h>

struct card
{
    int r;
    int s;
};

void setup(void);
void deal(void);
void predraw(int rank1, int rank2, int rank3, int rank4, int rank5, int suit1, int suit2, int suit3, int suit4, int suit5, int weight);
void draw(int c1, int c2, int c3, int c4, int c5, int weight);
int score(card deal[]);

int score_array[2598960];
card deck[52];
int draw_combinations_array[] = { 1533939, 178365, 178365, 16215, 178365, 16215, 16215, 1081, 178365, 16215, 16215, 1081, 16215, 1081, 1081, 47, 178365, 16215, 16215, 1081, 16215, 1081, 1081, 47, 16215, 1081, 1081, 47, 1081, 47, 47, 1 };
int weighting_array[] = { 5, 43, 43, 473, 43, 473, 473, 7095, 43, 473, 473, 7095, 473, 7095, 7095, 163185, 43, 473, 473, 7095, 473, 7095, 7095, 163185, 473, 7095, 7095, 163185, 7095, 163185, 163185, 7669695 };
__int64 tot_combinations[52];
int tot_weight = 0;

const char* hand_name_array[] = {
    "Loss","Jacks or better","Two pair","Three of a kind","Straight","Flush","Full house","Four of a kind","Straight flush","Royal flush","Total" };
int win_array[] = { 0,1,2,3,4,6,9,25,50,800 };


int main()
{
    time_t begtime, endtime;
    begtime = time(NULL);
    setup();
    deal();
    endtime = time(NULL);
    printf("Total seconds=\t%i\n", (int)endtime - (int)begtime);
}

void setup(void)
{
    int i, c1, c2, c3, c4, c5, count, score_deal_array[52], sc;
    card deal[5];
    for (i = 0; i <= 51; i++)
    {
        deck[i].r = (int)(i / 4);
        deck[i].s = i % 4;
        score_deal_array[i] = 0;
        tot_combinations[i] = 0;
    }
    count = 0;
    for (c1 = 0; c1 <= 47;c1++)
    {
        deal[0] = deck[c1];
        for (c2 = c1 + 1; c2 <= 48; c2++)
        {
            deal[1] = deck[c2];
            for (c3 = c2 + 1; c3 <= 49; c3++)
            {
                deal[2] = deck[c3];
                for (c4 = c3 + 1; c4 <= 50; c4++)
                {
                    deal[3] = deck[c4];
                    for (c5 = c4 + 1; c5 <= 51; c5++)
                    {
                        deal[4] = deck[c5];
                        sc = score(deal);
                        score_array[count] = sc;
                        score_deal_array[sc]++;
                        count++;
                    }
                }
            }
        }
    }
    //   std::cerr << "count =\t" << count << "\n";
    //   for (i = 0; i <= 9; i++)
    //       std::cerr << hand_name_array[i] << "\t" << score_deal_array[i] << "\n";
}


void deal(void)
{
    /*    int i, c1, c2, c3, c4, c5;
        int count = 0;
        for (c1 = 0; c1 <= 47;c1++)
        {
            for (c2 = c1 + 1; c2 <= 48; c2++)
            {
                for (c3 = c2 + 1; c3 <= 49; c3++)
                {
                    for (c4 = c3 + 1; c4 <= 50; c4++)
                    {
                        for (c5 = c4 + 1; c5 <= 51; c5++)
                        {
                            draw(c1, c2, c3, c4, c5);
                            count++;
                            if ((count % 1000) == 0)
                                std::cerr << count << "\n";
                        }
                    }
                }
            }
        }  */
    int i, r1, r2, r3, r4, r5;
    // Four of a kind
    for (r1 = 0; r1 <= 12; r1++)
    {
        for (r2 = 0; r2 <= 12; r2++)
        {
            if (r1 != r2)
            {
                predraw(r1, r1, r1, r1, r2, 0, 1, 2, 3, 0, 4);
            }
        }
    }
    // full house
    for (r1 = 0; r1 <= 12; r1++)
    {
        for (r2 = 0; r2 <= 12; r2++)
        {
            if (r1 != r2)
            {
                predraw(r1, r1, r1, r2, r2, 0, 1, 2, 0, 1, 12);
                predraw(r1, r1, r1, r2, r2, 0, 1, 2, 0, 3, 12);
            }
        }
    }
    // three of a kind
    for (r1 = 0; r1 <= 12; r1++) // three of a kind
    {
        for (r2 = 0; r2 <= 11; r2++) // first singleton
        {
            for (r3 = r2 + 1; r3 <= 12; r3++) // second singleton
            {
                if ((r1 != r2) && (r1 != r3))
                {
                    predraw(r1, r1, r1, r2, r3, 0, 1, 2, 0, 0, 12);
                    predraw(r1, r1, r1, r2, r3, 0, 1, 2, 3, 3, 4);
                    predraw(r1, r1, r1, r2, r3, 0, 1, 2, 0, 1, 24);
                    predraw(r1, r1, r1, r2, r3, 0, 1, 2, 0, 3, 12);
                    predraw(r1, r1, r1, r2, r3, 0, 1, 2, 3, 0, 12);
                }
            }
        }
    }
    // two pair
    for (r1 = 0; r1 <= 12; r1++)  // singleton
    {
        for (r2 = 0; r2 <= 11; r2++) // pair 1
        {
            for (r3 = r2 + 1; r3 <= 12; r3++) // pair 2
            {
                if ((r1 != r2) && (r1 != r3))
                {
                    predraw(r1, r2, r2, r3, r3, 0, 0, 1, 0, 1, 12);
                    predraw(r1, r2, r2, r3, r3, 2, 0, 1, 0, 1, 12);
                    predraw(r1, r2, r2, r3, r3, 0, 0, 1, 2, 3, 12);
                    predraw(r1, r2, r2, r3, r3, 2, 0, 1, 2, 3, 12);
                    predraw(r1, r2, r2, r3, r3, 0, 0, 1, 0, 2, 24);
                    predraw(r1, r2, r2, r3, r3, 1, 0, 1, 0, 2, 24);
                    predraw(r1, r2, r2, r3, r3, 2, 0, 1, 0, 2, 24);
                    predraw(r1, r2, r2, r3, r3, 3, 0, 1, 0, 2, 24);
                }
            }
        }
    }
    // one pair
    for (r1 = 0; r1 <= 12; r1++) // pair
    {
        std::cerr << "One pair\t" << r1 << "\n";
        for (r2 = 0; r2 <= 10; r2++) // singleton 1 
        {
            for (r3 = r2 + 1; r3 <= 11; r3++)// singleton 2 
            {
                for (r4 = r3 + 1; r4 <= 12; r4++) // singleton 3
                {
                    if ((r1 != r2) && (r1 != r3) && (r1 != r4))
                    {
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 0, 0, 12);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 2, 2, 12);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 0, 1, 12);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 1, 0, 12);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 1, 0, 0, 12);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 2, 3, 12);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 3, 2, 12);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 3, 2, 2, 12);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 0, 2, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 2, 0, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 0, 0, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 2, 2, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 0, 2, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 2, 0, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 1, 2, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 2, 1, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 0, 1, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 0, 2, 3, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 0, 3, 24);
                        predraw(r1, r1, r2, r3, r4, 0, 1, 2, 3, 0, 24);
                    }
                }
            }
        }
    }
    // five singletons
    for (r1 = 0; r1 <= 8; r1++)
    {
        std::cerr << "Five singletons\t" << r1 << "\n";
        for (r2 = r1 + 1; r2 <= 9; r2++)
        {
            for (r3 = r2 + 1; r3 <= 10; r3++)
            {
                for (r4 = r3 + 1; r4 <= 11; r4++)
                {
                    for (r5 = r4 + 1; r5 <= 12; r5++)
                    {
                        predraw(r1, r2, r3, r4, r5, 0, 0, 0, 0, 0, 4);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 0, 0, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 0, 0, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 1, 0, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 0, 1, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 0, 0, 1, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 0, 1, 1, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 1, 0, 1, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 1, 1, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 0, 0, 1, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 0, 1, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 1, 0, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 0, 0, 1, 12);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 0, 1, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 1, 0, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 1, 1, 0, 0, 0, 12);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 0, 1, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 1, 0, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 1, 2, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 0, 0, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 0, 2, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 2, 0, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 0, 0, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 0, 2, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 2, 0, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 0, 0, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 1, 2, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 2, 1, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 2, 2, 1, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 1, 2, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 2, 1, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 2, 2, 1, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 1, 0, 2, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 0, 1, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 0, 2, 1, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 1, 2, 0, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 1, 0, 2, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 2, 0, 1, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 1, 2, 2, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 1, 2, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 2, 1, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 0, 1, 2, 3, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 0, 2, 3, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 2, 0, 3, 24);
                        predraw(r1, r2, r3, r4, r5, 0, 1, 2, 3, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 0, 2, 3, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 2, 0, 3, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 0, 2, 3, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 0, 0, 3, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 0, 3, 0, 24);
                        predraw(r1, r2, r3, r4, r5, 1, 2, 3, 0, 0, 24);
                    }
                }
            }
        }
    }
    printf("Total weight=\t%i\n", tot_weight);
    for (i = 9; i >= 0; i--)
        printf("%s\t%i\t%I64i\n", hand_name_array[i], win_array[i], tot_combinations[i]);
}

void predraw(int rank1, int rank2, int rank3, int rank4, int rank5, int suit1, int suit2, int suit3, int suit4, int suit5, int weight)
{
    int c1, c2, c3, c4, c5;
    c1 = rank1 * 4 + suit1;
    c2 = rank2 * 4 + suit2;
    c3 = rank3 * 4 + suit3;
    c4 = rank4 * 4 + suit4;
    c5 = rank5 * 4 + suit5;
    tot_weight += weight;
    draw(c1, c2, c3, c4, c5, weight);
}

void draw(int c1, int c2, int c3, int c4, int c5, int weight)
{
    int i, j, d1, d2, d3, d4, d5, index, sc, draw_score_array[32][10];
    int count = 0;
    for (i = 0; i <= 31; i++)
    {
        for (j = 0; j <= 9; j++)
            draw_score_array[i][j] = 0;
    }
    for (d1 = 0; d1 <= 47; d1++)
    {
        for (d2 = d1 + 1; d2 <= 48; d2++)
        {
            for (d3 = d2 + 1; d3 <= 49; d3++)
            {
                for (d4 = d3 + 1; d4 <= 50; d4++)
                {
                    for (d5 = d4 + 1; d5 <= 51; d5++)
                    {
                        sc = score_array[count];
                        index = 0;
                        if ((d1 == c1) || (d2 == c1) || (d3 == c1) || (d4 == c1) || (d5 == c1))
                            index += 16;
                        if ((d1 == c2) || (d2 == c2) || (d3 == c2) || (d4 == c2) || (d5 == c2))
                            index += 8;
                        if ((d1 == c3) || (d2 == c3) || (d3 == c3) || (d4 == c3) || (d5 == c3))
                            index += 4;
                        if ((d1 == c4) || (d2 == c4) || (d3 == c4) || (d4 == c4) || (d5 == c4))
                            index += 2;
                        if ((d1 == c5) || (d2 == c5) || (d3 == c5) || (d4 == c5) || (d5 == c5))
                            index += 1;
                        draw_score_array[index][sc]++;
                        count++;
                    }
                }
            }
        }
    }
    double ev;
    double max_ev = 0;
    int best_play = 0;
    for (i = 0; i <= 31; i++)
    {
        ev = 0.0;
        for (j = 0; j <= 9; j++)
            ev += draw_score_array[i][j] * win_array[j];
        ev /= draw_combinations_array[i];
        if (ev > max_ev)
        {
            max_ev = ev;
            best_play = i;
        }
    }
    for (j = 0; j <= 9; j++)
    {
        tot_combinations[j] += weight * draw_score_array[best_play][j] * weighting_array[best_play];
    }
}

int score(card deal[])
{
    int straight, flush;
    if ((deal[0].s == deal[1].s) && (deal[0].s == deal[2].s) && (deal[0].s == deal[3].s) && (deal[0].s == deal[4].s))
        flush = 1;
    else
        flush = 0;
    if ((deal[0].r != deal[1].r) && (deal[1].r != deal[2].r) && (deal[2].r != deal[3].r) && (deal[3].r != deal[4].r))
    {
        if ((deal[0].r + 4) == deal[4].r)
            straight = 1;
        else if ((deal[4].r == 12) && (deal[3].r == 3)) // wheel
            straight = 1;
        else
            straight = 0;
    }
    else
        straight = 0;
    if ((straight == 1) && (flush == 1))
    {
        if (deal[0].r == 8)
            return 9;
        else
            return 8;
    }
    else if (flush == 1)
        return 5;
    else if (straight == 1)
        return 4;
    else if ((deal[0].r == deal[3].r) || (deal[1].r == deal[4].r))
        return 7;
    else if ((deal[0].r == deal[2].r) && (deal[3].r == deal[4].r))
        return 6;
    else if ((deal[0].r == deal[1].r) && (deal[2].r == deal[4].r))
        return 6;
    else if ((deal[0].r == deal[2].r) || (deal[1].r == deal[3].r) || (deal[2].r == deal[4].r))
        return 3;
    else if ((deal[0].r == deal[1].r) && (deal[2].r == deal[3].r))
        return 2;
    else if ((deal[0].r == deal[1].r) && (deal[3].r == deal[4].r))
        return 2;
    else if ((deal[1].r == deal[2].r) && (deal[3].r == deal[4].r))
        return 2;
    else if ((deal[0].r == deal[1].r) && (deal[0].r >= 9))
        return 1;
    else if ((deal[1].r == deal[2].r) && (deal[1].r >= 9))
        return 1;
    else if ((deal[2].r == deal[3].r) && (deal[2].r >= 9))
        return 1;
    else if ((deal[3].r == deal[4].r) && (deal[3].r >= 9))
        return 1;
    else
        return 0;
}