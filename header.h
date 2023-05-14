//
//  header.h
//  anl_lab_project
//
//  Created by Farida Sherif on 09/05/2023.
//

#ifndef header_h
#define header_h
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <map>
#include <queue>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

typedef pair<string, float> PAIR;

struct compare
{
    bool operator()(const PAIR& a, const PAIR& b);
};
struct Website
{
    string websitename;
    vector<string> keywords;
    float impressions = 0;
    float clicked = 0;
    float rank = 0;
    float PR = 0;
};
struct Edge
{
    int src, dest;
};

struct Graph
{
    vector<vector<int>> adjList;

    Graph(vector<Edge> const& edges, int n);
};

void update_files(Website* websites1);
bool iequals(const string& a, const string& b);
bool has_QuotedWord(vector<string> searchwords, vector<string> keywords);// Checks if the search statement contains a word between quotations
bool has_AND(vector<string> searchwords, vector<string> keywords); // Checks if the search statement contains all specified search words using the "AND" operator
bool has_OR(vector<string> searchwords, vector<string> keywords); // Checks if the search statement contains any of the specified search words using the "OR" operator

float calculate_CTR(Website website); // Calculates the Click-Through Rate (CTR) for a given website based on its impressions and clicked counts
void calculate_PR(Graph graph, Website* websites, float* outgoinglinks); // Implements the PageRank algorithm to calculate the PageRank (PR) values for the websites
void initialize_PR(Graph graph, Website* websites); // Calculates the PageRank (PR) values for the websites in the graph
float website_score(Website website); // Calculates the score of a website based on its PageRank (PR) and Click-Through Rate (CTR) values

void printGraph(Graph const& graph, int n); // Prints the adjacency list representation of the graph
Graph Create_Graph(vector<Edge>& edges); // Creates a graph using the edges obtained from the "webgraph.csv" file and returns the graph object
void openwebsite(string search); // Displays a menu when the user wants to open a website, allowing them to go back to search results, perform a new search, or exit the program
bool pattern_search(string pat, string txt); // Searches for a pattern in a given text and returns true if found
void mainmenu(); // Function declaration for the main menu
void tokenize(string const& str, const char delim, vector<string>& out); // Splits a string into tokens using the specified delimiter
void preform_search(string search); // Retrieves and displays the search results based on the provided search statement

void load_website_data(); // Reads the keyword, impressions, and clicked data from their respective files and populates the `websites1` array with the website information
priority_queue<PAIR, vector<PAIR>, compare> make_map(vector<Website> resultswebsite);
void start_program();


#endif /* header_h */
