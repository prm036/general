/////////////////////////////////////////////
// Approach-1 Naive
#include <vector>
using namespace std;
class Solution {
public:
    int tallestBillboard(vector<int>& rods, int left, int right, int i){
        if(i == rods.size()){
            return left == right ? left: 0;
        }
        int a = tallestBillboard(rods, left+rods[i], right, i+1);
        int b = tallestBillboard(rods, left, right+rods[i], i+1);
        int c = tallestBillboard(rods, left, right, i+1);
        return max({a,b,c});
    }
    int tallestBillboard(vector<int>& rods) {
        return tallestBillboard(rods, 0, 0, 0);
    }
};



/////////////////////////////////////////////
// Approach-2 instead of capturing left, right states, we are
// instead relying on gap and smartly keeping track of one of
// the state.
// Motivation of this approach -- helpful in memoization

class Solution {
public:
    int tallestBillboard(vector<int>& rods, int gap, int i){
        if(i == rods.size()){
            return gap == 0 ? 0: INT_MIN;
        }
        int a = rods[i] + tallestBillboard(rods, gap+rods[i], i+1);
        int b = tallestBillboard(rods, gap-rods[i], i+1);
        int c = tallestBillboard(rods, gap, i+1);
        return max({a,b,c});
    }
    int tallestBillboard(vector<int>& rods) {
        return tallestBillboard(rods, 0, 0);
    }
};




/////////////////////////////////////////////
// Approach-3  memoization

class Solution {
    int dp[25][10007];
public:
    int tallestBillboard(vector<int>& rods, int gap, int i){
        if(i == rods.size()){
            return gap == 0 ? 0: INT_MIN;
        }
        if(dp[i][gap+5000] != -1){
            return dp[i][gap+5000];
        }
        int a = rods[i] + tallestBillboard(rods, gap+rods[i], i+1);
        int b = tallestBillboard(rods, gap-rods[i], i+1);
        int c = tallestBillboard(rods, gap, i+1);
        return dp[i][gap+5000] = max({a,b,c});
    }
    int tallestBillboard(vector<int>& rods) {
        memset(dp, -1, sizeof(dp));
        return tallestBillboard(rods, 0, 0);
    }
};

/////////////////////////////////////////////
// Approach-4: Meet in the middle
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    // Helper function to generate all possible (difference -> max_left_height)
    unordered_map<int, int> get_dist(const vector<int>& rods) {
        unordered_map<int, int> states;
        states[0] = 0; // Base case: 0 difference, 0 height

        for (int x : rods) {
            unordered_map<int, int> next_states = states;
            for (auto const& [diff, height] : states) {
                // Option 1: Add rod to Pile 1 (Left)
                // New diff = diff + x, New height = height + x
                next_states[diff + x] = max(next_states[diff + x], height + x);

                // Option 2: Add rod to Pile 2 (Right)
                // New diff = diff - x, New height = height stays the same
                next_states[diff - x] = max(next_states[diff - x], height);
            }
            states = next_states;
        }
        return states;
    }

    int tallestBillboard(vector<int>& rods) {
        int n = rods.size();
        
        // 1. Split rods into two halves
        vector<int> left_rods(rods.begin(), rods.begin() + n / 2);
        vector<int> right_rods(rods.begin() + n / 2, rods.end());

        // 2. Generate all (diff, height) pairs for both halves
        unordered_map<int, int> left_map = get_dist(left_rods);
        unordered_map<int, int> right_map = get_dist(right_rods);

        int max_billboard = 0;

        // 3. Match them up
        // If left half has diff 'd', we need right half to have diff '-d'
        for (auto const& [d, h_left] : left_map) {
            if (right_map.count(-d)) {
                max_billboard = max(max_billboard, h_left + right_map[-d]);
            }
        }

        return max_billboard;
    }
};


/////////////////////////////////////////////////////
// Final Approach -- Modified C++ Knapsack Code
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int tallestBillboard(vector<int>& rods) {
        int sum = 0;
        for (int r : rods) sum += r;

        // dp[gap] is the maximum height of the shorter pile.
        // We use -1e9 to represent "impossible" gaps.
        vector<int> dp(sum + 1, -1e9);
        dp[0] = 0;

        for (int x : rods) {
            vector<int> prev_dp = dp; // Copy to ensure we use each rod only once
            
            for (int gap = 0; gap <= sum; ++gap) {
                if (prev_dp[gap] < 0) continue;

                // CHOICE 1: Put rod on the TALLER side
                // The gap increases, shorter side stays the same height.
                if (gap + x <= sum) {
                    dp[gap + x] = max(dp[gap + x], prev_dp[gap]);
                }

                // CHOICE 2: Put rod on the SHORTER side
                // This is the "Knapsack Profit" part. The shorter side's 
                // height increases by either the rod or the gap.
                int new_gap = abs(gap - x);
                int height_gain = min(gap, x); 
                dp[new_gap] = max(dp[new_gap], prev_dp[gap] + height_gain);
                
                // CHOICE 3: Ignore the rod
                // Handled implicitly by initializing dp with prev_dp.
            }
        }

        return dp[0];
    }
};