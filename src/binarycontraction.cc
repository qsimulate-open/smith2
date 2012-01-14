//
// Author : Toru Shiozaki
// Date   : Feb 2009
//

#include <src/binarycontraction.h>
#include <src/smartindex.h>
#include <iostream>
#include <cassert>

using namespace std;

BinaryContraction::BinaryContraction(vector<shared_ptr<Tensor> >& tensr, shared_ptr<Tensor> targt)
 : tensor_(tensr), target_tensor_(targt) {

  cost_ = cost_evaluater(tensr); 
  determine_target_indices(tensor_);
  if (!one()) refresh_indices();

}


BinaryContraction::~BinaryContraction(){

}


pair<PCost, PCost>  BinaryContraction::cost_evaluater(const vector<shared_ptr<Tensor> >& tensors) {
  PCost pcop;
  PCost pcmem;

  if (one())  
    return make_pair(pcop, pcmem);

  list<SmartIndex> si_first = tensors.at(0)->smartindices()->si();
  for (auto siter = si_first.begin(); siter != si_first.end(); ++siter) {
    if (siter->target_tensor().get() != tensors.at(1).get()) { // if not contraction indices
      siter->add_to_pcost(pcmem);
      siter->add_to_pcost(pcop);
    } 
  }
  list<SmartIndex> si_second = tensors.at(1)->smartindices()->si();
  for (auto siter = si_second.begin(); siter != si_second.end(); ++siter) {
    siter->add_to_pcost(pcop);
    if (siter->target_tensor() != tensors.at(0))  // if not contraction indices 
      siter->add_to_pcost(pcmem);
  }
 
  return make_pair(pcop, pcmem);
}


const string BinaryContraction::show() const {
  string out = "[";
  if (tensor_.at(0)) { 
    const shared_ptr<Tensor> a = tensor_.at(0);
    out += a->show() + " "; 
  }
  if (!one() && tensor_.at(1)) { 
    const shared_ptr<Tensor> a = tensor_.at(1);
    out += a->show() + " "; 
  }
  out += "-> " + target_tensor_->show();
  out += "]";
  return out;
}


void BinaryContraction::determine_target_indices(const vector<shared_ptr<Tensor> >& tensors) {
  list<SmartIndex> target_indices, prod, prod2;
  list<SmartIndex> si_first = tensors.at(0)->smartindices()->si();

  if (one()) {
    target_tensor_->set_smartindices(si_first);
    target_tensor_->set_permutables(si_first);
    shared_ptr<SmartIndexList> tmp(new SmartIndexList(si_first));
  } else { 

    // collecting indices that are not going to be summed...
    int counter = 0;
    for (auto siter = si_first.begin(); siter != si_first.end(); ++siter, ++counter) {
      if (!siter->target_tensor() || !siter->target_tensor()->in_list(tensors.at(1)->regtensors()) )  // if not contraction indices
        target_indices.push_back(*siter);
      else
        prod.push_back(*siter);
    }
    list<SmartIndex> si_second = tensors.at(1)->smartindices()->si();
    counter = 0;
    for (auto siter = si_second.begin(); siter != si_second.end(); ++siter, ++counter) {
      if (!siter->target_tensor() || !siter->target_tensor()->in_list(tensors.at(0)->regtensors()) ) // if not contraction indices 
        target_indices.push_back(*siter);
      else
        prod2.push_back(*siter);
    }

    list<SmartIndex> target_indices_p(target_indices.begin(), target_indices.end());
  
    // merging indices
    vector<list<SmartIndex>::iterator> removelist;
    for (auto siter = target_indices_p.begin(); siter != target_indices_p.end(); ++siter) {
      auto tmp = siter;
      for (auto siter2 = (++tmp); siter2 != target_indices_p.end(); ++siter2) {
        if (siter->permutable(*siter2)) {
          removelist.push_back(siter2);
          siter->merge(*siter2);
        } 
      }
    }
    
    for (auto i = removelist.begin(); i != removelist.end(); ++i) 
      target_indices_p.erase(*i);
  
    target_tensor_->set_smartindices(target_indices_p);

    // sort here!
    target_tensor_->sort_indices();
  }

  
}


void BinaryContraction::refresh_indices() {
  shared_ptr<Tensor> t1 = first();
  shared_ptr<Tensor> t2 = second();
  vector<shared_ptr<Tensor> > r1 = t1->regtensors();  
  vector<shared_ptr<Tensor> > r2 = t2->regtensors();
  shared_ptr<list<SmartIndex> > s1 = t1->smartindices_pointer();
  shared_ptr<list<SmartIndex> > s2 = t2->smartindices_pointer();
  for (auto siter = s1->begin(); siter != s1->end(); ++siter) {
    if (find(r2.begin(), r2.end(), siter->target_tensor()) != r2.end())
      siter->set_target_tensor(t2); 
  } 
  vector<list<SmartIndex>::iterator> removelist;
  for (auto siter = s1->begin(); siter != s1->end(); ++siter) {
    auto tmp = siter;
    ++tmp;
    for (auto siter2 = tmp; siter2 != s1->end(); ++siter2) {
      if (siter->permutable(*siter2)) { 
        siter->merge(*siter2);
        removelist.push_back(siter2); 
      }
    }
  }
  for (auto i = removelist.begin(); i != removelist.end(); ++i)
    s1->erase(*i);
  removelist.clear();

  for (auto siter = s2->begin(); siter != s2->end(); ++siter) {
    if (find(r1.begin(), r1.end(), siter->target_tensor()) != r1.end())
      siter->set_target_tensor(t1); 
  } 
  for (auto siter = s2->begin(); siter != s2->end(); ++siter) {
    auto tmp = siter;
    ++tmp;
    for (auto siter2 = tmp; siter2 != s2->end(); ++siter2) {
      if (siter->permutable(*siter2)) { 
        siter->merge(*siter2);
        removelist.push_back(siter2); 
      }
    }
  }
  for (auto i = removelist.begin(); i != removelist.end(); ++i)
    s2->erase(*i);

  t1->sort_indices(); 
  t2->sort_indices(); 
}


const bool BinaryContraction::factorizable(BinaryContraction& other) {
  assert(target_tensor() == other.target_tensor());

  bool out = true;
  if (tensor().size() != other.tensor().size() 
    || tensor().size() == 1 || other.tensor().size() == 1) out = false;

  shared_ptr<Tensor> f1 = first();  
  shared_ptr<Tensor> f2 = other.first();

  if (out && !f1->identical(f2)) out = false; 

  return out;
}



