#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <iomanip>

using namespace std;

#include "debug.h"
#include "file_sys.h"

int inode::next_inode_nr {1};

struct file_type_hash {
   size_t operator() (file_type type) const {
      return static_cast<size_t> (type);
   }
};

ostream& operator<< (ostream& out, file_type type) {
   static unordered_map<file_type,string,file_type_hash> hash {
      {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
      {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
   };
   return out << hash[type];
}
//
//inode state functions start
//
inode_state::inode_state() {
   file_type first;
   first = file_type::DIRECTORY_TYPE; 
   root = make_shared<inode>(first);
   inode dirroot = *root;
   base_file_ptr nwdir = dirroot.getfile();
   nwdir->mkdirref(".", root);
   nwdir->mkdirref("..",root);
   
   cwd = root;
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");
}

inode_ptr inode_state::getcwd(){
  return cwd;
}

inode_ptr inode_state::getroot(){
  return root;
}

inode_ptr inode_state::makeplain(){
   file_type first;
   first = file_type::PLAIN_TYPE;
   inode_ptr newfile = make_shared<inode>(first);
   return newfile;
}

inode_ptr inode_state::makedir(){
   file_type first;
   first = file_type::DIRECTORY_TYPE;
   inode_ptr newfile = make_shared<inode>(first);
   return newfile;
}

const string& inode_state::prompt() const { return prompt_; }

void inode_state::setcwd(inode_ptr newcwd){
  cwd = newcwd;
  return;
}

void inode_state::setprompt(const string& promptname){
  prompt_ = promptname;
  prompt_ = prompt_ + " ";
}

void inode_state::pushdir(const string& nextdir){
  dirs.push_back (nextdir);
  return;
}

void inode_state::popdirs(){
   cout << dirs.back() << endl;
}

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}
//
//inode functions start
//
inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}


base_file_ptr inode::getfile(){
  return contents;
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

file_error::file_error (const string& what):
            runtime_error (what) {
}
//
//plain files
//
size_t plain_file::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   int sum = 0;
   int i = 0;
   int j = data.size();
   for(i = 0; i<j; i++){
     sum += data[i].size();
   }
   return sum;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   data = words;
   DEBUGF ('i', words);
}

void plain_file::remove (const string&) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::mkdir (const string&) {
   throw file_error ("is a plain file");
}

void plain_file::mkdirref (const string& dirname, inode_ptr par) {
   throw file_error ("is a plain file");
}


inode_ptr plain_file::mkfile (const string&) {
   throw file_error ("is a plain file");
}

inode_ptr plain_file::searchmap(const string& filename){
   throw file_error ("is a plain file");
}

void plain_file::removekey(const string& keyname){
   throw file_error ("is a plain file");
}

void plain_file::printmap() {
  throw file_error ("is a plain file");
}

int plain_file::checktype() {
  return 0;
}
//
//directory functiona
//
size_t directory::size() const {
   size_t size {0};
   DEBUGF ('i', "size = " << size);
   return dirents.size();
}

const wordvec& directory::readfile() const {
   throw file_error ("is a directory");
}

void directory::writefile (const wordvec&) { 
  throw file_error ("is a directory");
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
}

inode_ptr directory::mkdir (const string& dirname) {
   file_type first;
   first = file_type::DIRECTORY_TYPE;
   inode_ptr nf = make_shared<inode>(first);
   dirents.insert(pair<string, inode_ptr>(dirname,nf) );
   DEBUGF ('i', dirname);
   return nf;
}

void directory::mkdirref (const string& dirname, inode_ptr par) {
   dirents.insert(pair<string, inode_ptr>(dirname, par) );
   return;
}


inode_ptr directory::mkfile (const string& filename) {
   file_type first;
   first = file_type::PLAIN_TYPE;
   inode_ptr nf = make_shared<inode>(first);
   dirents.insert(pair<string, inode_ptr>(filename,nf) );
   DEBUGF ('i', filename);
   return nf;
}


inode_ptr directory::searchmap(const string& filename){
  inode_ptr temp  {nullptr};
  int bewl = dirents.count(filename);
  //cout << "bewl = " << bewl << endl;
  if(bewl == 0){
    return temp;
  }
  temp = dirents.find(filename)->second;
  if(temp == nullptr){
    cout<< "not found" << endl;
  }
  return temp;
}

void directory::removekey(const string& keyname){
  dirents.erase(keyname);
  return;
}

void directory::printmap() {
  for(map<string,inode_ptr>::iterator it = dirents.begin(); 
      it != dirents.end(); ++it){
    inode_ptr curr = it->second;
    inode curren = *curr;
    base_file_ptr curnode = curren.getfile();
    
    int num = curren.get_inode_nr();
    cout << setw(6)<<  num << setw(6)<< curnode->size() << " ";
    cout << it->first << endl;
  }
  //cout << endl;
  return;
}

int directory::checktype() {
  return 1;
}
