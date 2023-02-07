#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <algorithm>

using namespace std;

unordered_map<int, unordered_map<int, float>> dataSet;
unordered_map<int, unordered_map<int, float>> userSim;
vector<int>targetUser_id;
vector<int>targetUser_movieid;

struct sort_pred {
    bool operator()(const std::pair<int, float>& left, const std::pair<int, float>& right) {
        return left.second > right.second;
    }
};

void readTrain(string train_path) {

    std::fstream readFile;

    readFile.open(train_path, std::ios::in);

    if (!readFile.is_open()) {
        std::cerr << "can't reach the train file" << std::endl;
    }

    std::string user_id;
    std::string movie_id;
    std::string rating;

    int int_user;
    int int_movie;
    float movieRatings;

    while (getline(readFile, user_id, ','), getline(readFile, movie_id, ','), getline(readFile, rating)) {
        int_user = stoi(user_id);
        int_movie = stoi(movie_id);
        movieRatings = stof(rating);

        dataSet[int_user][int_movie] = movieRatings;
    }

    readFile.close();
}

void readTest(string test_path) {

    std::fstream testFile;

    testFile.open(test_path, std::ios::in);

    if (!testFile.is_open()) {
        std::cerr << "can't reach the test file" << std::endl;
    }

    std::string id;
    std::string testuser_id;
    std::string testmovie_id;

    int int_id;
    int testint_user;
    int testint_movie;

    while (getline(testFile, id, ','), getline(testFile, testuser_id, ','), getline(testFile, testmovie_id)) {
        testint_user = stoi(testuser_id);
        testint_movie = stoi(testmovie_id);
        targetUser_id.push_back(testint_user);
        targetUser_movieid.push_back(testint_movie);
    }
}

float predict(int target_user, int target_movie,int N) {

    unordered_map<int, float> temp = userSim[target_user];

    std::vector<std::pair<int, float>> similiarUsers_and_similarities_to_target;
    std::copy(temp.begin(), temp.end(), std::back_inserter(similiarUsers_and_similarities_to_target));
    std::sort(similiarUsers_and_similarities_to_target.begin(), similiarUsers_and_similarities_to_target.end(), sort_pred());

    if(N > similiarUsers_and_similarities_to_target.size()){
        N = similiarUsers_and_similarities_to_target.size();
    }

    std::vector<std::pair<int, float>>::iterator end = similiarUsers_and_similarities_to_target.begin();

    std::advance(end,N);

    float A = 0, B = 0;
    for (auto itr = similiarUsers_and_similarities_to_target.begin(); itr != end; itr++) {
        if(dataSet[itr->first][target_movie]>=0.5){
         A += itr->second * dataSet[itr->first][target_movie];
         B += itr->second;
        } 
    }
    if (A == 0||B == 0) {
        return 0;
    }
    return A / B;
}

float jaccardSimilarity(unordered_map<int, float>& ratings1, unordered_map<int, float>& ratings2) {
    int user1 = ratings1.begin()->first;
    int user2 = ratings2.begin()->first;
    if (userSim[user1].find(user2) != userSim[user1].end() || userSim[user2].find(user1) != userSim[user2].end()) {
        return userSim[user1][user2];
    }

    int intersection = 0;
    int union_ = 0;
    for (auto r_ratings1 = ratings1.begin(); r_ratings1 != ratings1.end(); r_ratings1++) {
        auto it = ratings2.find(r_ratings1->first);
        if (it == ratings2.end()) {
            union_++;
        } else {
            intersection++;
            union_++;
        }
    }
    for (auto r_ratings2 = ratings2.begin(); r_ratings2 != ratings2.end(); r_ratings2++) {
        auto it = ratings1.find(r_ratings2->first);
        if (it == ratings1.end()) {
            union_++;
        }
    }
    if (union_ == 0) {
        return 0;
    }
    return (float) intersection / union_;
}

int main() {

    std::fstream outFile;

    outFile.open("C:\\Users\\90507\\Desktop\\outputt.csv", std::ios::out);

    readTrain("C:\\Users\\90507\\Desktop\\train.csv");
    readTest("C:\\Users\\90507\\Desktop\\testt.csv");

    int N = 100;
    vector<int>::iterator target_itr = targetUser_movieid.begin();

    for (auto targetUser_itr = targetUser_id.begin(); targetUser_itr != targetUser_id.end(); targetUser_itr++) {
        // Iterate over all users in the dataSet unordered map except the target user
        int value = *targetUser_itr;
        int value2 = *target_itr;
        for (auto current_user_it = dataSet.begin(); current_user_it != dataSet.end(); current_user_it++) {
            if (current_user_it->first == value) {
                continue;
            }
            // Calculate the similarity between the current user that watched the target movie and the target user
            if(current_user_it->second.find(value2)!=current_user_it->second.end()){
                 float sim = jaccardSimilarity(current_user_it->second,dataSet[value]);
                 userSim[current_user_it->first][value] = sim;
                 userSim[value][current_user_it->first] = sim;
            }
            continue;
        }
        target_itr++;
    }

    cout << "operation finished" << endl;

    vector<int>::iterator targetMovie_itr = targetUser_movieid.begin();
    int i = 0;

    for (auto targetUser_itr = targetUser_id.begin(); targetUser_itr != targetUser_id.end(); targetUser_itr++) {
        int valueFirst = *targetUser_itr;
        int valueSecond = *targetMovie_itr;
        float pre = predict(valueFirst, valueSecond,N);
        outFile << i++ << "," << pre << endl;
        if (targetMovie_itr != targetUser_movieid.end()) {
            targetMovie_itr++;
        }
    }

    return 0;
}
