#include "walksat.h"

map<string, Clause> clauses_;
set<string> unsat_clause_;
int total_clauses;
int total_variables;


Valuation walksat(int max_changes) 
{
  Valuation val; 
 
  // dodeljivanje slucajnih vrednosti Literalima kao True ili False
  for (int i = 1; i < total_variables +1; i++){
    int random_ = rand() % 2;
    if (random_ == 1)
      val[i] = true;
    else
      val[i] = false;
  }
    //proveravamo da li posotoji neki konflikt i ako ima dodajemo u listu nezadovoljivih klauza
  check_current_valuation(val, 0);

  int number_of_changes = 0;
  while (number_of_changes < max_changes){
    //ako ne postoje nezadovoljive klauze vracamo valuaciju
    if (unsat_clause_.empty())
      return val;
    // u suprotnom ako postoje
    auto iterator = unsat_clause_.begin();

    //Pomeramo iterator na sledecu nasumicnu poziciju u nizu nezadovoljivih klauza.
    
    advance(iterator, rand()%unsat_clause_.size());
    //biramo nasumicno nezadovoljivu klauzu
    Clause choosen_clause = clauses_[*iterator];

    //odredjujemo nasumicni literal iz klauze
    int random_variable = choosen_clause[rand() % choosen_clause.size()];
    int selected_variable = abs(random_variable);

    //menjamo vrednost literala
    val[selected_variable] = !val[selected_variable];
    // proveravamo da li je valuacija sada dobra
    check_current_valuation(val, selected_variable);
    // povecavamo broj promena znaka
    number_of_changes++;
  }
    // ako nismo uspeli da nadjemo zadovoljivu valuaciju vracamo valuaciju koja ima jedan literal koji je negativan
  val.clear(); 
  val[0] = false;
  return val;
}
//Ucitavanje CNF file 
void load_CNF_file(char* file_path) {
  string line;
  char delimiter = ' ';
  ifstream input_file(file_path); 
  if (input_file.is_open()) { 
    getline(input_file, line); //ignorise komentare
    getline(input_file, line); //citanje konfiguracije
    vector<string> tokens = split_clause(line, delimiter);

    total_variables = stoi (tokens[2]);
    total_clauses = stoi (tokens[3]);

    for (int i = 0; i < total_clauses; i++) {
      getline(input_file, line);
      tokens = split_clause(line, delimiter);
      Clause clause;
      for (int j = 0; j < tokens.size()-1; j++) { 
        clause.push_back( stoi(tokens[j]));
      }
      pair<string, Clause> entry;
      entry = make_pair(convert_clause_to_string(clause), clause);
      clauses_.insert(entry);
    }

    input_file.close();
  }
  else
    cout << "Neuspesno otvaranje fajla, proverite da li je dobro uneta putanja" << endl;
}

void display_sat_valuation(const Valuation& val) {
  Clause solution;
  for(auto var = val.begin(); var != val.end(); ++var){
    if (var->second == 1)
        solution.push_back(var->first);
    cout << var->first << " " << var->second << endl;
  }

  cout << "Resenje problema: " ;
  for (int i = 0; i < solution.size(); i++)
      cout << "P" << i+1 << "=" << solution[i] << ", ";

  cout << endl;
}




void check_current_valuation(const Valuation& val, int literal) 
{
  if (literal != 0)
  {
    for (auto i = clauses_.begin(); i != clauses_.end(); i++) {
        //ako literal postoji u klauzi
      if (in_clause(i->second, literal)) {
        // ako nema konflikta
        if(no_conflict(i->second, val)){ 
            //brisi iz nezadovoljivih
            unsat_clause_.erase(i->first);
        }
        else 
        // u suprotnom dodaj u nezadovoljive
          unsat_clause_.insert(i->first);
      }
    }
  }
  else{
    for (auto i = clauses_.begin(); i != clauses_.end(); i++) { 
      //ako ima konflikta
      if (!no_conflict(i->second, val)) 
        // dodaj u nezadovoljive
        unsat_clause_.insert(i->first);
    }
  }
}

//provera da li imamo konflikt 
bool no_conflict(const Clause& clause, const Valuation& val) 
{
  for(auto i = clause.begin(); i != clause.end(); i++){ 
    if (((*i > 0) && (val.at(abs(*i)))) || ((*i < 0) && (!val.at(abs(*i))))) 
      return true;
  }
  return false;
}

//provera da li literal postoji u klauzi
bool in_clause(const Clause& clause, int literal){
  for (int i = 0; i < clause.size(); i++) {
    if (abs(clause[i]) == literal)
     return true;
  }
  return false;
}

vector<string> split_clause(const string& string_, char delimiter) { 
  vector<string> tokens;

  int start_index = 0;
  int word_length = 0;
  bool inword_ = false;

  for (int i = 0; i < string_.length(); i++) { 
    if (string_[i] == delimiter) {
      if (inword_ == true) { 
        string token = string_.substr(start_index, word_length);
        tokens.push_back(token);
      }
      start_index = i+1;
      word_length = 0;
      inword_ = false;
    }
    else { 
      word_length++;
      inword_ = true;
    }
  }
  if (inword_ == true)
    tokens.push_back(string_.substr(start_index, word_length));

  return tokens;
}
 
string convert_clause_to_string(const Clause& clause) {
  string encoding = "C";
  for (int i = 0; i < clause.size(); i++) 
     encoding = encoding + " " + to_string(clause[i]);
  return encoding;
}