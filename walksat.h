#ifndef __WALKSAT_H__ 
#define __WALKSAT_H__

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;
using Valuation = map<int, bool>;
using Clause = vector<int>;

Valuation walksat( int max_changes); 
void load_CNF_file(char* file_path);    
void display_sat_valuation(const Valuation& val);    



//proveravamo nezadovoljive klauze
void check_current_valuation(const Valuation& val, int changed_literal);
// provera da li imamo konflikt
bool no_conflict(const Clause& clause, const Valuation& val);

//provera da li je literal postoji
bool in_clause(const Clause& clause, int literal);

//pomocne funkcije
//delimo stringove po delimiteru u vektor substringova.
vector<string> split_clause(const string& sentence, char delimiter);
//konvertovanje klauze u string
string convert_clause_to_string(const Clause& clause);


#endif