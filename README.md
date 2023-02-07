# primitive-recommendation-system

Collaborative Filtering

This repository contains a collaborative filtering algorithm implemented in C++ as a school project. The algorithm is used to predict movie ratings for users based on past ratings of other similar users.

# Problem Statement
Given a training dataset containing past ratings of users for different movies, we want to predict the ratings of new users for new movies.

# Data
The dataset used in this project is the MovieLens 100k dataset. It contains 100,000 ratings for 1682 movies by 943 users. The data is in the form of user_id,movie_id,rating.

# Algorithm
The algorithm uses the Jaccard Similarity to find the most similar users to the target user. The Jaccard Similarity measures the similarity between two sets of items, in this case, the items are the movies that each user has rated.

Once the most similar users are found, the algorithm calculates the predicted rating of the target user for a new movie by taking a weighted average of the ratings of the similar users for that movie. The weights used in the average are the Jaccard Similarities of the similar users to the target user.

# Running the Code
To run the code, you need to have a C++ compiler installed. The code takes two arguments, the training data file path and the test data file path.

# Results
The results of the algorithm are stored in a file named submission.txt in the same directory as the code. The file contains the predicted ratings of new users for new movies in the format user_id,movie_id,prediction.

# Conclusion
The results of this project show that the algorithm performs well in predicting the ratings of new users for new movies. The algorithm's accuracy can be improved by using other similarity measures or by incorporating additional information about the movies and users.
