#include "walksat.h"

int main(int argc, char* argv[]) { 
  char* file_path;
	if (argc = 2)
        file_path = argv[1];
  else{
        cout << "Greska, niste prosledili putanju";
        return -1;
  }

  cout << "Implementacija Walksat algoritma u C++" << endl;
  cout << "Resavanje problema " << file_path << endl;

  load_CNF_file(file_path);
  Valuation result = walksat(20000);

  int a = result.size();
  if(a==1) cout<<"UNSAT";
  else
    display_sat_valuation(result);
  return 0;
}