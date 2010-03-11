//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <stdexcept>
#include <boost/regex.hpp>
#include <src/pretensor.h>
#include <src/equation.h>
#include <src/dg/dg.h>
#include <src/ig/inputgenerator.h>
#include <src/block.h>
#include <src/spaces.h>

using namespace std;
using namespace boost;
using namespace DG;
using namespace IG;

list<RefListPreTensor>  read_input(string filename);
list<RefListPreTensor>  read_input(list<string> content);
Spaces generate_spaces() {
  const long num_occ_blocks = 1;
  const long num_vir_blocks = 1;
  const vector<long> num_occ_orb(num_occ_blocks, 4); 
  const vector<long> num_vir_orb(num_vir_blocks, 9); 
  const vector<long> sym_occ(num_occ_blocks, 0); 
  const vector<long> sym_vir(num_vir_blocks, 0); 

  Spaces sp(num_occ_blocks, num_vir_blocks, num_occ_orb, num_vir_orb, sym_occ, sym_vir);
  return sp;
};

int main(int argc, char* argv[]) {

  try {

    if (argc < 2) {
      throw std::runtime_error("Please specify an input file");
    } 
    string filename = argv[1];
  
   // those will be controlled by the suffix of the input files
    InputGenerator ig(filename);
    DiagramGenerator dg(ig.generate());
    //DiagramGenerator dg(filename);
    list<RefListPreTensor> listpretensor=read_input(dg.generate());
    //list<RefListPreTensor> listpretensor=read_input(filename);
    
    if (listpretensor.empty()) return 1;
    list<RefVecTensor> listvectensor;
   
    list<RefListPreTensor>::iterator iter;
    for (iter = listpretensor.begin(); iter != listpretensor.end() ;++iter) { 
      RefVecTensor newvectensor = (*iter)->analyze();
      listvectensor.push_back(newvectensor);
    }
    
    Equation equation(listvectensor);
    const bool opt_memory = false;
    equation.strength_reduction(opt_memory);
    equation.form_tree();
    equation.factorize();
    cout << equation.tree_root()->show() << endl;

    equation.startup();

  } catch (const runtime_error& e) {
    cout << "  ----" << endl;
    cout << "  Runtime error: " << e.what() << endl;
    cout << endl;
  } catch (const logic_error& e) {
    cout << "  ----" << endl;
    cout << "  Logic error: " << e.what() << endl;
    cout << endl;
  } catch (...) {
    cout << "  ----" << endl;
    cout << "  Error" << endl;
    cout << endl;
  }  
  return 0;
}



list<RefListPreTensor> read_input(string filename) {

  list<RefListPreTensor> out;
 
  ifstream ifs(filename.c_str());
  regex tensor_reg("(\\S+?)\\s\\(.*?\\)");
  if (!ifs.is_open()) {
    cout <<  "FILE " << filename << " CANNOT BE OPENED" << endl;
    return out;
  }
   
  while (!ifs.eof()) {
    string stmp;
    getline(ifs,stmp);
    if (stmp.empty()) continue;
 
    smatch what;
 
    string::const_iterator start = stmp.begin();
    string::const_iterator end   = stmp.end();
 
    list<RefPreTensor> pretensors;
    while ( regex_search(start, end, what, tensor_reg) ) {
 
      string symbol(what[1].first,what[1].second);
 
      if (symbol != "Sum" && symbol != "P" ) { 
        string tensor(what[0].first,what[0].second);
        pretensors.push_back(RefPreTensor(new PreTensor(tensor, generate_spaces())));
      }
      start = what[0].second;
      
    } 
 
    out.push_back(RefListPreTensor(new ListPreTensor(pretensors)));
 
  }
 
  return out;
}


list<RefListPreTensor> read_input(list<string> content) {

  list<RefListPreTensor> out;
  regex tensor_reg("(\\S+?)\\s\\(.*?\\)");
   
  for(list<string>::iterator i = content.begin(); i != content.end(); ++i) {
    const string stmp = *i;
 
    smatch what;
 
    string::const_iterator start = stmp.begin();
    string::const_iterator end   = stmp.end();
 
    list<RefPreTensor> pretensors;
    while ( regex_search(start, end, what, tensor_reg) ) {
 
      string symbol(what[1].first,what[1].second);
 
      if (symbol != "Sum" && symbol != "P" ) { 
        string tensor(what[0].first,what[0].second);
        pretensors.push_back(RefPreTensor(new PreTensor(tensor, generate_spaces())));
      }
      start = what[0].second;
      
    } 
 
    out.push_back(RefListPreTensor(new ListPreTensor(pretensors)));
 
  }
 
  return out;
}

