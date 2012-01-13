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
//#include <src/storage/memorygrp.h>

using namespace std;
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

int main(int argc, char** argv) {

#if 0
  // to be initialized by reading input
  const size_t memory_each = 200000000LU;
  shared_ptr<MemoryGrp> memgrp(new MemoryGrp(argc, argv, memory_each)); 
  const int myrank = memgrp->n();
  cout << myrank << endl;
  memgrp->wait();
#else
  const int myrank = 0;
#endif

  try {
    
    if (argc < 2) {
      throw std::runtime_error("Please specify an input file");
    } 
    string filename = argv[1];
  
    // those will be controlled by the suffix of the input files
    if (myrank == 0) {
      InputGenerator ig(filename);
      DiagramGenerator dg(ig.generate());
      //DiagramGenerator dg(filename);
      list<RefListPreTensor> listpretensor=read_input(dg.generate());
      //list<RefListPreTensor> listpretensor=read_input(filename);
    
      if (listpretensor.empty()) return 1;
      list<RefVecTensor> listvectensor;
   
      for (auto iter = listpretensor.begin(); iter != listpretensor.end() ;++iter) { 
        RefVecTensor newvectensor = (*iter)->analyze();
        listvectensor.push_back(newvectensor);
      }
    
      Equation equation(listvectensor);
      const bool opt_memory = false;
      equation.strength_reduction(opt_memory);
      equation.form_tree();
      equation.factorize();
      cout << equation.tree_root()->show() << endl;

#if 0
      equation.startup();
#endif
    }

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
  boost::regex tensor_reg("(\\S+?)\\s\\(.*?\\)");
  if (!ifs.is_open()) {
    cout <<  "FILE " << filename << " CANNOT BE OPENED" << endl;
    return out;
  }
   
  while (!ifs.eof()) {
    string stmp;
    getline(ifs,stmp);
    if (stmp.empty()) continue;
 
    boost::smatch what;
 
    string::const_iterator start = stmp.begin();
    string::const_iterator end   = stmp.end();
 
    list<RefPreTensor> pretensors;
    while ( boost::regex_search(start, end, what, tensor_reg) ) {
 
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
  boost::regex tensor_reg("(\\S+?)\\s\\(.*?\\)");
   
  for(auto i = content.begin(); i != content.end(); ++i) {
    const string stmp = *i;
 
    boost::smatch what;
 
    string::const_iterator start = stmp.begin();
    string::const_iterator end   = stmp.end();
 
    list<RefPreTensor> pretensors;
    while ( boost::regex_search(start, end, what, tensor_reg) ) {
 
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

