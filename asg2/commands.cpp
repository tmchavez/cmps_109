#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
};

command_fn find_command_fn (const string& cmd) {
  // Note: value_type is pair<const key_type, mapped_type>
  // So: iterator->first is key_type (string)
  // So: iterator->second is mapped_type (command_fn)
  DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

void fn_cat (inode_state& state, const wordvec& words){
   inode_ptr temp = state.getcwd();
   inode act = *temp;
   base_file_ptr dir = act.getfile();
   
   inode_ptr desfile = dir->searchmap(words.at(1)); 
   if(desfile == nullptr){
     cout << "no such file in directory " << endl;
     return;
   }
   inode desnode = *desfile;
   base_file_ptr dfile = desnode.getfile();
   wordvec pvec = dfile->readfile();   
   int i = 0;
   int j = pvec.size();
   for(i= 0; i<j; i++){
     cout<< pvec[i] << " ";
   }
   cout << endl;
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_cd (inode_state& state, const wordvec& words){
   inode_ptr temp = state.getcwd();
   inode_ptr temp2 = state.getroot();
   if(words.size() == 1){
      state.setcwd(temp2);
   }
   inode act = *temp;
   base_file_ptr dir = act.getfile();
   inode_ptr newdir = dir->searchmap(words.at(1));
   if(newdir == nullptr){
     cout << "no such file in directory " << endl;
     return;
   }
   state.pushdir(words.at(1));
   state.setcwd(newdir); 
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words){
   inode_ptr temp = state.getcwd();
   inode act = *temp;
   base_file_ptr dir = act.getfile();
   dir->printmap();
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_lsr (inode_state& state, const wordvec& words){
   inode_ptr temp = state.getcwd();
   inode act = *temp;
   base_file_ptr dir = act.getfile();
   dir->printmap(); 
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_make (inode_state& state, const wordvec& words){
   wordvec::const_iterator first = words.begin() + 2;
   wordvec::const_iterator last = words.begin() + words.size();
   wordvec qualitycontent(first,last); 
   
   inode_ptr temp = state.getcwd();
   inode act = *temp;
   base_file_ptr dir = act.getfile();
   inode_ptr newfile = dir->mkfile(words.at(1));
   inode newnode = *newfile;
   base_file_ptr fil = newnode.getfile();
   fil->writefile(qualitycontent);
   wordvec qc = fil->readfile();
   
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_mkdir (inode_state& state, const wordvec& words){
   inode_ptr temp = state.getcwd();
   inode act = *temp;
   base_file_ptr dir = act.getfile();
   inode_ptr newdir = dir->mkdir(words.at(1));
   
   inode ndir = *newdir;
   base_file_ptr nwdir = ndir.getfile();
   nwdir->mkdirref(".", newdir);
   nwdir->mkdirref("..",temp);
   
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_prompt (inode_state& state, const wordvec& words){
   state.setprompt(words.at(1));
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_pwd (inode_state& state, const wordvec& words){
   inode_ptr temp = state.getcwd();
   inode_ptr temp2 = state.getroot();
   if(temp = temp2){
     cout<< "/" << endl;
   }else{
     state.popdirs();
   }
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rm (inode_state& state, const wordvec& words){
   inode_ptr temp = state.getcwd();
   inode act = *temp;
   base_file_ptr dir = act.getfile();
   int rec = dir->checktype();
   cout << rec << endl;
   dir->removekey(words.at(1));
   
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words){
   inode_ptr temp = state.getcwd();
   inode act = *temp;
   base_file_ptr dir = act.getfile();

   dir->removekey(words.at(1));
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

