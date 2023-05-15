//
//  source.cpp
//  anl_lab_project
//
//  Created by Farida Sherif on 09/05/2023.
//
#include "header.h"
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
#include <cctype>

using namespace std;

int counter = 0; // Counter variable to track the number of websites
Website* websites1;

bool compare::operator()(const PAIR& a, const PAIR& b)
{
    return a.second < b.second;
}
Graph::Graph(vector<Edge> const& edges, int n)
{
    adjList.resize(n);

    for (int i = 0; i < edges.size(); i++)
    {
        int src = edges[i].src;
        int dest = edges[i].dest;
        adjList[src].push_back(dest);
    }
}

void update_files(Website* websites1){
    
    ofstream imp;
    ofstream cli;
 
//    imp.open("/Users/faridabey/Documents/ANL and DSGN of algorithms Lab/anl_lab_project/anl_lab_project/impressions.csv");
    imp.open("impressions.csv");
    for(int i =0; i<counter; i++)
    {
        if (i < counter - 1)
        {
            imp << websites1[i].websitename << "," << websites1[i].impressions << " \n";
        }
        else
        {
            imp << websites1[i].websitename << "," << websites1[i].impressions << " ";
        }
    }
    imp.close();
    
//    cli.open("/Users/faridabey/Documents/ANL and DSGN of algorithms Lab/anl_lab_project/anl_lab_project/clicked.csv");
    cli.open("clicked.csv");
    for(int j =0; j<counter; j++)
    {
        
        if (j < counter - 1)
        {
            cli << websites1[j].websitename << "," << websites1[j].clicked << " \n";
        }
        else
        {
            cli << websites1[j].websitename << "," << websites1[j].clicked << " ";
        }
    }
    cli.close();
}

bool has_QuotedWord( vector<string> searchwords,  vector<string> keywords) {
    for (const string& searchword : searchwords) {
        bool found = false;
        bool quoted = false;
        string keyword = searchword;

        // Check if the search word is enclosed in quotation marks
        if (searchword.size() >= 2 && searchword.front() == '"' && searchword.back() == '"') {
            quoted = true;
            keyword = searchword.substr(1, searchword.size() - 2);  // Remove the quotation marks
        }

        // Search for the keyword in the same case
        for (const string& kw : keywords) {
            if (quoted) {
                if (kw == keyword) {
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

bool has_AND(vector<string> searchwords, vector<string> keywords){

    for (int i = 0; i < searchwords.size(); i++)
    {
        string searchwords_elt = searchwords[i];
        if (find(keywords.begin(), keywords.end(), searchwords_elt) == keywords.end())
        {
            return false;
        }
    }
    return true;
}
bool has_OR(vector<string> searchwords, vector<string> keywords){
    for (int i = 0; i < searchwords.size(); i++)
    {
        if (find(keywords.begin(), keywords.end(), searchwords[i]) != keywords.end())
        {
            return true;
        }
    }
    return false;
}

float calculate_CTR(Website website){
    float CTR = (website.clicked / website.impressions) * 100;
    return CTR;
}
void calculate_PR(Graph graph, Website* websites, float* outgoinglinks){
    vector< pair <float, float> > webs[4];
    vector< pair <float, float> > webs2[4];
    for (int i = 0; i < counter; i++){

        for (int j = 0; j < counter; j++){
            list<int> ::iterator it;
            for (auto it = graph.adjList[j].begin(); it != graph.adjList[j].end(); ++it){
                if (*it == i)
                    webs[i].push_back(make_pair(websites[j].PR, outgoinglinks[j]));
            }
        }
    }
    for (int i = 0; i < counter; i++){
        for (int z = 0; z < webs[i].size(); z++){
            if (z == 0)
                websites[i].PR = webs[i][z].first / webs[i][z].second;
            else if (z > 0)
                websites[i].PR += webs[i][z].first / webs[i][z].second;
        }
    }

}
void initialize_PR(Graph graph, Website* websites){
    float counter2 = counter;
    float* outgoinglinks = new float[counter];
    for (int i = 0; i < counter; i++)
        outgoinglinks[i] = 0;
    
    for (int i = 0; i < counter; i++)
        websites[i].PR = 1 / counter2;
    
    for (int j = 0; j < counter; j++)
    {
        list<int> ::iterator it;
        for (auto it = graph.adjList[j].begin(); it != graph.adjList[j].end(); ++it)
            outgoinglinks[j]++;
    }
    for (int i = 0; i < 2; i++)
        calculate_PR(graph, websites, outgoinglinks);
}
float website_score(Website website){
    /*
     pagescore = (0.4 * pagerank) + (((1 - ((0.1 * impressions) / (1 + (0.1 * impressions)))) * pagerank) +
     (((0.1 * impressions) / (1 + (0.1 * impressions))) * ctr) * 0.6)
     */

    float CTR = calculate_CTR(website);
    float score = (0.4 * website.PR) + ((1 - ((0.1 * website.impressions)/(1 + 0.1 * website.impressions))) * website.PR + ((0.1 * website.impressions)/(1 + 0.1 * website.impressions)) * CTR) * 0.6;
   
    return score;
}

Graph Create_Graph(vector<Edge>& edges){
    ifstream myfile;
    vector<string> lines;
    vector<string> linesreduced;
    vector<int> websiteIndices1;
    vector<int> websiteIndices2;
//    myfile.open("/Users/faridabey/Documents/ANL and DSGN of algorithms Lab/anl_lab_project/anl_lab_project/webgraph.csv");
    myfile.open("webgraph.csv");
    if (myfile.is_open())
    {
        while (myfile.good())
        {
            string substr;
            getline(myfile, substr, ' ');
            lines.push_back(substr);

        }
    }
    for (int i = 0; i < lines.size(); i++)
    {
        tokenize(lines[i], ',', linesreduced);
    }
    for (int i = 1; i < linesreduced.size(); i++)
    {
        if (i % 2 == 0)
        {
            linesreduced[i].erase(0, 1);
        }
    }
    for (int i = 0; i < linesreduced.size(); i++)
    {
        for (int j = 0; j < counter; j++)
        {
            if (linesreduced[i] == websites1[j].websitename)
            {
                if (i == 0 || i % 2 == 0)
                {
                    websiteIndices1.push_back(j);
                }
                else
                {
                    websiteIndices2.push_back(j);
                }
            }
        }
    }
    for (int i = 0; (i < websiteIndices1.size())&&(i < websiteIndices2.size()); i++)
    {
        edges.push_back({ websiteIndices1[i],websiteIndices2[i]});
    }
    Graph graph(edges, counter);

    return graph;
}
void openwebsite(string search){
    system("CLS");
//    system("clear");
    cout << " WELCOME! " << endl << endl;
    cout << "Would you like to" << endl;
    cout << "1. Go back to search results" << endl;
    cout << "2. New search" << endl;
    cout << "3. Exit" << endl;
    int choice;
    cout << "Type in your choice: ";
    cin >> choice;
    while (choice < 1 || choice > 3 || cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid Input!" << endl;
        cout << "Please Enter a valid input." << endl;
        cout<<"Enter your choice : ";
        cin >> choice;
    }
    cout << endl;
    if (choice == 1)
    {
        system("CLS");
//        system("clear");
        preform_search(search);
    }
    else if (choice == 2)
    {
        system("CLS");
//        system("clear");
        mainmenu();
    }
    else if (choice == 3)
    {
        update_files(websites1);
        exit(0);
    }
}
bool pattern_search(string pattern, string txt){
    bool found = false;
    int s = pattern.length();
    int d = txt.length();

    for (int i = 0; i <= d - s; i++)
    {
        int j;
        for (j = 0; j < s; j++)
        {
            if (txt[i + j] != pattern[j])
            {
                break;
            }
        }
        if (j == s)
        {
            found = true;
        }
    }
    return found;
}
void load_website_data(){
    ifstream myfile;
//    myfile.open("/Users/faridabey/Documents/ANL and DSGN of algorithms Lab/anl_lab_project/anl_lab_project/keyword.csv");
    myfile.open("keyword.csv");

    vector<string> website_keywords;
    vector<string> website_name;
    vector<string> keywords_line;
    vector<string> imp_data;
    if (myfile.is_open())
    {
        while (myfile.good())
        {
            string substr;
            getline(myfile, substr);
            keywords_line.push_back(substr);
        }
    }
    int count = 0;
    for (size_t i = 0; i < keywords_line.size(); i++)
    {
        if (pattern_search(".com", keywords_line[i]))
        {
            count++;
        }

    }
    websites1 = new Website[count];
    counter = count;
    for (int i = 0; i < keywords_line.size() - 1; i++)
    {
        tokenize(keywords_line[i], ',', website_keywords);

    }
    int j = -1;
    for (int i = 0; i < website_keywords.size(); i++)
    {
        if (pattern_search(".com", website_keywords[i]))
        {
            j++;
        }
        else
        {
            websites1[j].keywords.push_back(website_keywords[i]);
        }
    }
    myfile.close();
    
//    myfile.open("/Users/faridabey/Documents/ANL and DSGN of algorithms Lab/anl_lab_project/anl_lab_project/impressions.csv");
    myfile.open("impressions.csv");
    if (myfile.is_open())
    {
        while (myfile.good())
        {
            string substr;
            getline(myfile, substr, '\n');
            imp_data.push_back(substr);
        }
    }

    for (int i = 0; i < imp_data.size(); i++)
    {
        //token(imp_data[i], ',', website_name);
        string cp = imp_data[i];
        website_name.push_back(strtok(&cp[0], ",\n"));
        websites1[i].impressions = atoi(strtok(nullptr, ",\n"));
        websites1[i].websitename = website_name[i];
    }
    myfile.close();
    imp_data.clear();
    website_name.clear();

//    myfile.open("/Users/faridabey/Documents/ANL and DSGN of algorithms Lab/anl_lab_project/anl_lab_project/clicked.csv");
    myfile.open("clicked.csv");
    if (myfile.is_open())
    {
        while (myfile.good())
        {
            string substr;
            getline(myfile, substr, '\n');
            imp_data.push_back(substr);
        }
    }

    for (int i = 0; i < imp_data.size(); i++)
    {
//        token(imp_data[i], ',', website_name);
        string cp = imp_data[i];
        website_name.push_back(strtok(&cp[0], ",\n"));
        websites1[i].clicked = atoi(strtok(nullptr, ",\n"));
    }
    for (int i = 1; i < website_name.size(); i++)
    {
        if (i % 2 == 0)
        {
            website_name[i].erase(0, 1);
        }
    }
    int kz = -1;
    for (int i = 0; i < website_name.size(); i++)
    {
        if (pattern_search(".com", website_name[i]))
        {
            kz++;
        }
        else
        {
            //if (website_name[i].size() > 1)
                websites1[kz].clicked = atoi(website_name[i].c_str());
        }
    }
    myfile.close();
}
void tokenize(string const& str, const char delim, vector<string>& out){
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}
priority_queue<PAIR, vector<PAIR>, compare> make_map(vector<Website> resultswebsite) {
    priority_queue<PAIR, vector<PAIR>, compare> pq;
    unordered_map<string, float> dict;

    for ( Website& website : resultswebsite) {
        website.rank = website_score(website);
        dict.insert({website.websitename, website.rank});
    }

    for (const auto& entry : dict) {
        pq.push(entry);
    }

    return pq;
}

void preform_search(string search){
    priority_queue<PAIR, vector<PAIR>, compare> priorityQ;
    priority_queue<PAIR, vector<PAIR>, compare> priorityQ2;
    vector<string> words_array;
    vector<string> websitesww;
    vector<string> words_array_AND;
    vector<string> words_array_OR;
    vector<string> words_array_Quoted;
    vector<Website> resultwebsites;
    
    if (pattern_search("\"", search))
    {
        tokenize(search, '\"', words_array);
        for (auto& s : words_array)
        {
            tokenize(s, ' ', words_array_Quoted);
        }
        for (int i = 0; i < counter; i++)
        {
            if (has_QuotedWord(words_array_Quoted, websites1[i].keywords))
            {
                resultwebsites.push_back(websites1[i]);
            }
        }
        priorityQ = make_map(resultwebsites);
        priorityQ2 = priorityQ;
        int l = 0;
        while (!priorityQ.empty())
        {
            PAIR top = priorityQ.top();
            cout << l + 1 << " - " << top.first << endl;
            websitesww.push_back(top.first);
            priorityQ.pop();
            l++;
        }
        for (int i = 0; i < counter; i++)
        {
            for (int j = 0; j < websitesww.size(); j++)
            {
                if (websitesww[j] == websites1[i].websitename)
                {
                    websites1[i].impressions + 1;
                }
            }
        }

    }
    else if (pattern_search("&", search))
    {
        tokenize(search, '&', words_array);
        for (auto& s : words_array)
        {
            tokenize(s, ' ', words_array_AND);
        }
        for (int i = 0; i < counter; i++)
        {
            if (has_AND(words_array_AND, websites1[i].keywords))
            {
                resultwebsites.push_back(websites1[i]);
            }
        }
        priorityQ = make_map(resultwebsites);
        priorityQ2 = priorityQ;
        int u = 0;
        while (!priorityQ.empty())
        {
            PAIR top = priorityQ.top();
            cout << u + 1 << " - " << top.first << endl;
            websitesww.push_back(top.first);
            priorityQ.pop();
            u++;
        }
        for (int i = 0; i < counter; i++)
        {
            for (int j = 0; j < websitesww.size(); j++)
            {
                if (websitesww[j] == websites1[i].websitename)
                {
                    websites1[i].impressions + 1;
                }
            }
        }

    }
    else if (pattern_search("|", search))
    {
        tokenize(search, '|', words_array);
        for (auto& s : words_array)
        {
            tokenize(s, ' ', words_array_OR);
        }
        for (int i = 0; i < counter; i++)
        {
            if (has_OR(words_array_OR, websites1[i].keywords))
            {
                resultwebsites.push_back(websites1[i]);
            }
        }
        priorityQ = make_map(resultwebsites);
        priorityQ2 = priorityQ;
        int k = 0;
        while (!priorityQ.empty())
        {
            PAIR top = priorityQ.top();
            cout << k + 1 << " - " << top.first << endl;
            websitesww.push_back(top.first);
            priorityQ.pop();
            k++;
        }
        for (int i = 0; i < counter; i++)
        {
            for (int j = 0; j < websitesww.size(); j++)
            {
                if (websitesww[j] == websites1[i].websitename)
                {
                    websites1[i].impressions + 1;
                }
            }
        }
    }
    else
    {
        tokenize(search, ' ', words_array);
        for (auto& s : words_array)
        {
            tokenize(s, ' ', words_array_OR);
        }
        for (int i = 0; i < counter; i++)
        {
            if (has_OR(words_array_OR, websites1[i].keywords))
            {
                resultwebsites.push_back(websites1[i]);
            }
        }
        priorityQ = make_map(resultwebsites);
        priorityQ2 = priorityQ;
        int k = 0;
        while (!priorityQ.empty())
        {
            PAIR top = priorityQ.top();
            cout << k + 1 << " - " << top.first << endl;
            websitesww.push_back(top.first);
            priorityQ.pop();
            k++;
        }
        for (int i = 0; i < counter; i++)
        {
            for (int j = 0; j < websitesww.size(); j++)
            {
                if (websitesww[j] == websites1[i].websitename)
                {
                    websites1[i].impressions + 1;
                }
            }
        }
    }
    cout << endl << "Would you like to: " << endl;
    cout << "1. Choose a webpage to open" << endl;
    cout << "2. New search" << endl;
    cout << "3. Exit" << endl;
    int choice;
    cout << "Type in your choice: ";
    cin >> choice;
    while (choice < 1 || choice>3 || cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid Input!" << endl;
        cout << "Please Enter a valid input." << endl;
        cout << "Enter your choice : " << endl;
        cin >> choice;
    }
    cout << endl;
    if (choice == 1)
    {
        system("CLS");
//        system("clear");
        int i = 0;
        while (!priorityQ2.empty())
        {
            PAIR top = priorityQ2.top();
            cout << i + 1 << " " << top.first << " - " << top.second << endl;
            priorityQ2.pop();
            i++;
        }
        cout << "choose the website you want to open: ";
        int option;
        cin >> option;
        while (option < 1 || option> counter || cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid Input!" << endl;
            cout << "Please Enter a valid input." << endl;
            cout << "Enter your choice : " << endl;
            cin >> choice;
        }
        cout << endl;
        if (websitesww.size() == 0)
        {

        }
        else
        {
            for (int i = 0; i < counter; i++)
            {
                if (websitesww[option - 1] == websites1[i].websitename)
                {
                    websites1[i].clicked++;
//                    system("read -n 1 -s -r -p \"Press any key to continue...\"");
                    system("PAUSE"); //windows version
                }
            }
        }
        openwebsite(search);
    }
    else if (choice == 2)
    {
        system("CLS");
//        system("clear");
        mainmenu();
    }
    else if (choice == 3)
    {
        update_files(websites1);
        exit(0);
    }
}
void search_input(){
    string search;
    cout << "Enter a sentence: " << endl;
    cin.ignore();
    getline(cin, search);
    preform_search(search);
}
void mainmenu(){
    cout << "Welcome!" << endl;
    cout << "What would you like to do?" << endl;
    cout << "1. New Search" << endl;
    cout << "2. Exit" << endl;
    int choice;
    cout << "Type in your choice: ";
    cin >> choice;
    while (choice < 1 || choice>2 || cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid Input!" << endl;
        cout << "Please Enter a valid input." << endl;
        cout << "Enter your choice : " << endl;
        cin >> choice;
    }
    cout << endl;
    if (choice == 1)
    {
        system("CLS");
//        system("clear");
        search_input();
    }
    else if (choice == 2)
    {
        update_files(websites1);
        exit(0);
    }
}
void start_program()
{
    load_website_data();
    vector<Edge> edges1;
    Graph graph = Create_Graph(edges1);
    initialize_PR(graph, websites1);
    mainmenu();
    update_files(websites1);
}

