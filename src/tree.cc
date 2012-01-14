//
// Author:: Toru Shiozaki
// Date  :: Feb 2009
//
#include <src/tree.h>
#include <cassert>
#include <stdexcept>
#include <vector>

using namespace std;

Tree::Tree(const BinaryContraction& bc, const pair<list<RefTree>, list<RefTree> >& sub, RefTree par) 
 : binc_(bc), subtrees_(sub), parent_(par) {

  root_ = false;
}


Tree::Tree(const BinaryContraction& bc, const pair<list<RefTree>, list<RefTree> >& sub) 
 : binc_(bc), subtrees_(sub) {

  root_ = false;
}


Tree::Tree(RefVecTensor input, shared_ptr<Tensor> par) 
{

  vector<shared_ptr<Tensor> > vectensor = input->tensor();
  init(vectensor, par);
  root_ = false;

}


void Tree::init(vector<shared_ptr<Tensor> >& vectensor, shared_ptr<Tensor> par) {
  shared_ptr<Tensor> current; 
  shared_ptr<Tensor> previous = vectensor.back();
  list<RefTree> a;
  pair<list<RefTree>, list<RefTree> > prevtree = make_pair(a, a);

  auto titer_end = vectensor.rend();
  --titer_end;

  for (auto titer = vectensor.rbegin(); titer != titer_end; ++titer) { 
    if (titer != vectensor.rbegin()) {
      current = *titer; 
      shared_ptr<Tensor> target(new Tensor("i"));
      target->push_back_regtensors(target);
      vector<shared_ptr<Tensor> > btensors;
      btensors.push_back(current);
      btensors.push_back(previous);
      BinaryContraction bc(btensors, target);
      target->push_back_regtensors(current->regtensors());
      target->push_back_regtensors(previous->regtensors());
    
      RefTree tmptree(new Tree(bc, prevtree));
      
      list<RefTree> first = prevtree.first;
      for (auto titer = first.begin(); titer != first.end(); ++titer) 
        (*titer)->set_parent(tmptree); 
      list<RefTree> second = prevtree.second;
      for (auto titer = second.begin(); titer != second.end(); ++titer) 
        (*titer)->set_parent(tmptree); 

      list<RefTree> b(1, tmptree);
      prevtree = make_pair(a, b); 
      previous = target;
    }
  }

  if (vectensor.size() > 1) { 
    vector<shared_ptr<Tensor> > btensors;
    btensors.push_back(*titer_end);
    btensors.push_back(previous);
    BinaryContraction binc(btensors, par); 
    binc_ = binc;
  } else { 
    vector<shared_ptr<Tensor> > btensors;
    btensors.push_back(previous);
    BinaryContraction binc(btensors, par); 
    binc_ = binc;
  }

  subtrees_ = prevtree;
}


Tree::~Tree() {

}


void Tree::factorize() {

  /// So far, we are assuming that the first tensor be an input tensor.
  /// therefore, we are now starting with the second subtrees

  vector<list<RefTree>::iterator> remove_list;

  for (auto i = subtrees_.second.begin(); i != subtrees_.second.end(); ++i) { 
    if (find(remove_list.begin(), remove_list.end(), i) != remove_list.end()) continue;
    auto tmpi = i;
    ++tmpi;
    RefTree mysubtree = *i; 
    BinaryContraction mybinc = mysubtree->binc();

    for (auto j = tmpi; j != subtrees_.second.end(); ++j) {
      if (find(remove_list.begin(), remove_list.end(), j) != remove_list.end()) continue;
      RefTree othersubtree = *j;
      BinaryContraction otherbinc = othersubtree->binc();

      if (mybinc.factorizable(otherbinc)) {
        mysubtree->merge_second_subtree(othersubtree);
        remove_list.push_back(j);
      }
    }
  }

  for (auto k = remove_list.begin(); k != remove_list.end(); ++k)
    subtrees_.second.erase(*k);


  /// recursive calls
  for (auto i = subtrees_.second.begin(); i != subtrees_.second.end(); ++i)  (*i)->factorize();
  assert(subtrees_.first.empty());
  

}


void Tree::merge_second_subtree(RefTree other) {

  if (second_is_input()) 
    prepare_merge_second();
  if (other->second_is_input()) 
    other->prepare_merge_second();

  list<RefTree> othersub = other->subtrees_.second;

  subtrees_.second.merge(othersub);
  shared_ptr<Tensor> target = subtrees_.second.front()->target_tensor();

  list<SmartIndex> target_permutables = permutables_in_factorize(other);
  target->set_permutables(target_permutables);

  for (auto i = subtrees_.second.begin(); i != subtrees_.second.end(); ++i) 
    (*i)->renew_binc_target(target); 

}


const list<SmartIndex> Tree::permutables_in_factorize(RefTree other) {
  list<RefTree> othersub = other->subtrees_.second;
  list<SmartIndex> permutables2 = othersub.front()->target_tensor()->permutables()->si();
  list<SmartIndex> permutables1 = subtrees_.second.front()->target_tensor()->permutables()->si();


  list<SmartIndex> primitive1;
  for (auto i = permutables1.begin(); i != permutables1.end(); ++i) {
    list<SmartIndex> ps = i->extract();
    primitive1.insert(primitive1.end(), ps.begin(), ps.end());
  }

  list<SmartIndex> primitive2;
  for (auto i = permutables2.begin(); i != permutables2.end(); ++i) {
    list<SmartIndex> ps = i->extract();
    primitive2.insert(primitive2.end(), ps.begin(), ps.end());
  }

  list<SmartIndex> target_permutables;

  assert(primitive1.size() == primitive2.size());

  SmartIndex s1 = primitive1.front();
  SmartIndex s2 = primitive2.front();
  auto ia = primitive1.begin(); ++ia;
  auto ja = primitive2.begin(); ++ja;
  for (auto i = ia, j = ja; i != primitive1.end(); ++i, ++j) {
    if (s1.permutable(*i) && s2.permutable(*j)) {
      s1.merge(*i);
      s2.merge(*j);
    } else {
      target_permutables.push_back(s1);
      s1 = *i;
      s2 = *j;
    } 
  }
  target_permutables.push_back(s1);

  return target_permutables;
}


void Tree::prepare_merge_second() {

  vector<shared_ptr<Tensor> > b(1, binc_.second());
  shared_ptr<Tensor> target(new Tensor("i"));
  target->push_back_regtensors(target);
  BinaryContraction bc(b, target); 

  list<RefTree> dummy;
  RefTree parent(this);
  RefTree sub(new Tree(bc, make_pair(dummy, dummy), parent)); 

  binc_.set_second(target);
  subtrees_.second.push_back(sub); 

}


const int Tree::num_nodes() const {
  int out = 1;
//if (!is_root()) 
//  cout << binc_.show() << endl; 
  list<RefTree> subtree1 = subtrees_.first;
  list<RefTree> subtree2 = subtrees_.second;

  for (auto titer = subtree1.begin(); titer != subtree1.end(); ++titer) out += (*titer)->num_nodes();
  for (auto titer = subtree2.begin(); titer != subtree2.end(); ++titer) out += (*titer)->num_nodes();

  return out;
}


const string Tree::show(const int i) const {
  string out;
  string indent(i*2, ' '); 

  out += indent;
  out += binc_.show();
  out += "\n";

  list<RefTree> s1, s2;
  s1 = subtrees_.first; 
  s2 = subtrees_.second;

  int next = i;
  ++next;
  for (auto titer = s1.begin(); titer != s1.end(); ++titer) out += (*titer)->show(next);
  for (auto titer = s2.begin(); titer != s2.end(); ++titer) out += (*titer)->show(next);

  return out;
}

