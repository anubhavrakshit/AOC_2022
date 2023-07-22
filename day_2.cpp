#include <iostream>
#include <string>
#include <map>
#include <sstream>

using namespace std;
// Opponents moves and Your moves 
// Opponent R rock P paper S scissor
// Your RO rock PA paper SI scissor 
enum game_move {R = 'A', P = 'B', S = 'C', RO = 'X', PA = 'Y', SI = 'Z'};
// Give score for opponent and your move
int play(game_move opp, game_move you) {
    map <game_move, int> shape_score;
    shape_score[RO] = 1;
    shape_score[PA] = 2;
    shape_score[SI] = 3;

    // Table with scores for a pair of moves
    // {opponent move, your move} -> Score
    map <pair<game_move, game_move>, int> outcome;
    // Draw 3 points
    outcome[{R, RO}] = 3;
    outcome[{P, PA}] = 3;
    outcome[{S, SI}] = 3;
    // You loose 0 point
    outcome[{R, SI}] = 0;
    outcome[{P, RO}] = 0;
    outcome[{S, PA}] = 0;
    // You win 6 points
    outcome[{R, PA}] = 6;
    outcome[{P, SI}] = 6;
    outcome[{S, RO}] = 6;
    
    return  outcome[{opp, you}] + shape_score[you];
}

int choose_move(game_move opp, game_move you) {
    map<game_move, game_move> loose, win, draw;
    
    // What should be your move?
    loose[R] = SI;
    loose[P] = RO;
    loose[S] = PA;
    draw[R] = RO;
    draw[P] = PA;
    draw[S] = SI;
    win[R] = PA;
    win[P] = SI;
    win[S] = RO;
    
    // Your intent?
    if (you == RO) {
        // Loose
        cout << "Loose" << endl;
        return play(opp, loose[opp]);
    } else if (you == PA) {
        // Draw
        cout << "Draw" << endl;
        return play(opp, draw[opp]);
    } else {
        // Win
        cout << "Win" << endl;
        return play(opp, win[opp]);
    }
}

int main() {
    int total_score{0};
    string input;
    while (getline(cin, input)) {
        stringstream ss(input);
        char opp, you;
        ss >> opp >> you;
        int score = choose_move(static_cast<game_move>(opp), static_cast<game_move>(you));
        cout << "Score " << score << endl;
        total_score += score;
    }
    cout << "Total Score " << total_score << endl;
    return 0;
}