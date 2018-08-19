#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", cix_command::EXIT},
   {"help", cix_command::HELP},
   {"ls"  , cix_command::LS  },
   {"rm"  , cix_command::RM  },
   {"get" , cix_command::GET },
   {"put" , cix_command::PUT },
};

static const string help = R"||(
exit         - Exit the program.  Equivalent to EOF.
get filename - Copy remote file to local host.
help         - Print help summary.
ls           - List names of files on remote server.
put filename - Copy local file to remote host.
rm filename  - Remove file from remote server.
)||";

void cix_help() {
   cout << help;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = cix_command::LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::LSOUT) {
      log << "sent LS, server did not return LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

void cix_rm (client_socket& server, string fname1) {
   cix_header header;
   header.command = cix_command::RM;
   header.nbytes = 0;
   //header.filename = fname1;
   for(size_t i = 0; i<fname1.size(); i++){
     header.filename[i] = fname1.at(i);
   }
   
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "recieved header " << header << endl;
   if(header.command != cix_command::ACK) {
     log << "sent RM, server did not return ACK" << endl;
     log << "server returned " << header << endl;
   }else {
     log << "sent RM, server returned ACK" << endl;
     log << "server returned " << header << endl;
   }
}

void cix_get(client_socket& server, string fname1){
   cix_header header;
   header.command = cix_command::GET;
   header.nbytes = 0;
   for(size_t i = 0; i<fname1.size(); i++){
     header.filename[i] = fname1.at(i);
   }
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "recieved header " << header << endl;
   if(header.command != cix_command::FILE){
     log << "sent GET, server did not return FILE" << endl;
     log << "server returned " << header << endl;
   }else{
     ofstream nfile1{header.filename};
     char buffer[header.nbytes + 1];
     recv_packet (server, buffer, header.nbytes);
     log << "received " << header.nbytes << " bytes" << endl;
     buffer[header.nbytes] = '\0';
     nfile1.write(buffer, header.nbytes);
     nfile1.close();
   }
}

void cix_put(client_socket& server, string fname1){
   cix_header header;
   header.command = cix_command::PUT;
   header.nbytes = 0;
   for(size_t i = 0; i<fname1.size(); i++){
     header.filename[i] = fname1.at(i);
   }
   ifstream copyfile {header.filename};
   string outs = "";
   while(not copyfile.eof()){
     char buffer[0x1000];
     copyfile.read (buffer, sizeof buffer);
     outs += buffer;
   }
   copyfile.close();
   header.nbytes = outs.size();
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   send_packet (server, outs.c_str(), outs.size()); 
   recv_packet (server, &header, sizeof header);
   log << "recieved header " << header << endl;
   if(header.command != cix_command::ACK) {
     log << "sent PUT, server did not return ACK" << endl;
     log << "server returned " << header << endl;
   }else {
     log << "sent PUT, server returned ACK" << endl;
     log << "server returned " << header << endl;
   }
}


void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cix_server_host (args, 0);
   in_port_t port = get_cix_server_port (args, 1);
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         int check = 32;
         string com = line; string fname = "name";
         if (cin.eof()) throw cix_exit();
         for(unsigned i = 0; i < line.size(); i++){
           if(check == line.at(i)){
             com.assign(line.begin(),line.begin()+i);
             cout << com << endl;
             fname.assign(line.begin()+i+1,line.end());
             cout << fname << endl;
           }
         }
         log << "command " << com << endl;
         const auto& itor = command_map.find (com);
         cix_command cmd = itor == command_map.end()
                         ? cix_command::ERROR : itor->second;
         //cout << "cmd: " << cmd << endl;
         switch (cmd) {
            case cix_command::EXIT:
               throw cix_exit();
               break;
            case cix_command::HELP:
               cix_help();
               break;
            case cix_command::LS:
               cix_ls (server);
               break;
            case cix_command::RM:
               cix_rm(server,fname);
               break;
            case cix_command::GET:
               cix_get(server,fname);
               break;
            case cix_command::PUT:
               cix_put(server,fname);
               break;
            default:
               log << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}

