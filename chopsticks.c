#include <stdio.h>
#include <stdbool.h>

#define LEFT 0
#define RIGHT 1

#include <time.h>
#include <unistd.h>

/*
 * > Future changes: Find and fix loopholes.
 * > Add Update screen feature where the history gets dissapeared.
 *      use :: system("clear");
 * > Issue (01) :: A non terminating loop is created when you enter random text in the Enter Attack Code as an input.
*/

bool isAlive(int *arr, int player){
    if (arr[player]==0){
        return 0;
    }
    else {
        return 1;
    }
}

int hand(int num, bool side){

    if (side==LEFT){
        if (num%10 == num){
            return 0;
        }
        else {
            return num/10;
        }
    }
    else{
        return num%10;
    }
}

void printHand(int qty)
{
    for (int i = 0; i < hand(qty, LEFT); i++) printf("|");
    for (int i = 0; i < 4-hand(qty, LEFT); i++) printf("_");
    printf("  ");
    for (int i = 0; i < hand(qty, RIGHT); i++) printf("|");
    for (int i = 0; i < 4-hand(qty, RIGHT); i++) printf("_");
    printf("\n");
}

/*void rearange(int *arr, int player, bool hand_give, int qty){

    if (isAlive(arr, player)){

        if (qty > hand(arr[player], hand_give)){
            printf("Redo");
        }
        else{
            if (hand(arr[player],!hand_give)==hand(arr[player],hand_give)-qty){
                printf("Redo");
            }
            else{
                if (hand_give){
                    arr[player]=((hand(arr[player],0)+qty)%5)*10+(hand(arr[player],1)-qty);
                }
                else{
                    arr[player]=(hand(arr[player],0)-qty)*10+(hand(arr[player],1)+qty)%5;
                }
            }
        }
    }
    else {
        printf("Redo");
    }
}*/

int rearrange(int *arr, int player, int move)
{
    int currentSum = hand(arr[player], LEFT) + hand(arr[player], RIGHT);
    int finalSum = hand(move, LEFT) + hand(move, RIGHT);

    if (finalSum == 0) return 1;

    if ((currentSum%5) == (finalSum%5))
    {
        if (hand(move, RIGHT) != hand(arr[player], LEFT))
        {
            arr[player] = move;
            return 0;
        }
        else return 1;
    }
    else return 1;
}

int attack(int *arr, int attacker, int attacker_hand, int victim, int victim_hand){

    if (isAlive(arr, attacker) && isAlive(arr, victim)){

        if (hand(arr[victim], victim_hand) == 0 || hand(arr[attacker], attacker_hand) == 0){
            return 1;
        }
        else{
            if (victim_hand == LEFT){
                arr[victim] = ((hand(arr[victim],LEFT)+hand(arr[attacker], attacker_hand))%5)*10 + hand(arr[victim],RIGHT);
                return 0;
            } 
            else {
                arr[victim] = (hand(arr[victim],RIGHT)+hand(arr[attacker],attacker_hand))%5 + (10*hand(arr[victim],LEFT));    
                return 0;
            }
        }
    }
    else {
        return 1;
    }
}

// (solved) issue 01 :: if left hand dies then the number stored is eg. 1 and not as 01
// (solved) issue 02 :: if left dies attacker cannot give to left

int main(){

    int no_ppl;

    printf("How many players: ");
    scanf("%d", & no_ppl);
    char names[no_ppl][4];
    char name_temp[3];

    for (int i = 0; i < no_ppl; i++)
    {
        printf("Player %d, enter your 3 letter name: ", i+1);
        scanf("%s", name_temp);
        names[i][0] = name_temp[0];
        names[i][1] = name_temp[1];
        names[i][2] = name_temp[2];
        names[i][3] = '\0';
        printf("\n");

    }

    /* for (int i = 0; i < no_ppl; i++)
    {
        printf("%s\n", names[i]);
    } */

    /*
        Add Names Array
        Add Interface
        Check if array arr updates or not
    */

    int arr[no_ppl];

    for (int i=0; i<no_ppl; ++i){
        arr[i]=11;
    }

    int turn = 0;
    int oldTurn;
    int move;
    int returnCode;

    while (1)
    {
        printf("\nIt is %s's turn!\n\n", names[turn]);
        for (int i = 0; i < no_ppl; i++)
        {
            printf("%s's hand: ", names[i]);
            printHand(arr[i]);
        }
        do
        {
            //printf("\nTurn :: %d\n", turn+1);
            printf("\nEnter your move (0 for rearrange, 1 for attack): ");
            scanf("%d", &move);
            if (move)
            {
                int attackCode=000;
                while (1){
                    printf("Enter your attack code: "); // our hand no, victim no, victim hand no
                    scanf("%d", &attackCode);
                    if ((attackCode%100 - attackCode%10)/10 != turn){
                        break;
                    }
                    else {continue;}
                }
                    //while((attackCode%100 - attackCode%10)/10!=turn);
                returnCode = attack(arr, turn, (attackCode/100)-1, (attackCode%100 - attackCode%10)/10, (attackCode%10)-1);
            }
            else
            {
                int rearrangeCode;
                printf("Enter your rearrangement code: "); // your final move
                scanf("%d", &rearrangeCode);
                returnCode = rearrange(arr, turn, rearrangeCode);
            }
            printf("\n");
        }
        while(returnCode);

        oldTurn = turn;

        do
        {
            turn += 1;
            if (turn == no_ppl) turn = 0;
        }
        while (arr[turn] == 0);

        if (oldTurn == turn)
        {
            printf("\nPlayer %s has won!!\n", names[turn]);
            return 0;
        }
    }
}
