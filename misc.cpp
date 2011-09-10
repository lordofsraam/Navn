/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "includes.h"
#include "user.h"

//General misc functions
/** \fn Flux::string strip(const Flux::string &buf)
 * \brief Strips \r and \n from a Flux::string
 * Takes out the '\r' and '\n' from a Flux::string. Mostly used in messing with irc replies.
 * \param &buf String to be stripped
 * \return \a &buf without the '\r' and '\n'.
 */
Flux::string strip(const Flux::string &buf){
	Flux::string newbuf = buf;
	char c = newbuf[newbuf.size() - 1];
	while (c == '\n' || c == '\r'){
		newbuf.erase(newbuf.end() - 1);
		c = newbuf[newbuf.size() - 1];
	}
	return newbuf;
}
/**Random Number Generator
 * This will generate a random number x is start number, y is the stop number.
 * @param randint(int x, int y)
 */
int randint(int x, int y){
  srand(time(NULL));
  return rand()%(y-x+1)+x;
}
/** 
 * \fn Flux::string make_pass()
 * \brief Makes a random password
 * This generates a 5 number random password for the bots
 * quit and other password protected commands.
 * \return A Flux::string containing the 5 digit password.
 */
Flux::string make_pass(){
  int p1,p2,p3,p4,p5;
  srand(time(NULL));
  p1 = rand()%10; 
  p2 = rand()%10;
  p3 = rand()%10;
  p4 = rand()%10;
  p5 = rand()%10;
  std::stringstream pass_ss;
  pass_ss << p1 << p2 << p3 << p4 << p5;
  return pass_ss.str();
}
/**
 * \fn bool IsValadChannel(const Flux::string nerp)
 * This function returns if the channel is valid or not.
 * \param nerp Channel sring to be tested.
 * \return True if the Flux::string is a valid channel, false otherwise.
 */
bool IsValidChannel(const Flux::string &chan){
 if (chan[0] != '#')
    return false;
 return true;
}
/**
  * \fn Flux::string isolate(char begin, char end, Flux::string msg)
  * \brief Isolates a Flux::string between two characters
  * Finds the first character, then begins to add every consecutive character from there to a Flux::string
  *  until it reaches the end character.
  * \param begin The character saying where the cut should begin.
  * \param end The character saying where the cut should end.
  * \param msg The Flux::string you are wanting to isolate from.
  */
Flux::string isolate(char begin, char end, Flux::string msg){
  Flux::string to_find;
  size_t pos = msg.find(begin);
  pos += 1;
  for (unsigned i = pos; i < msg.length(); i++){
    if (msg.at(i) == end){
      break;
    }else{to_find = to_find+msg.at(i);}
  }
  return to_find;
}
/** 
 * \fn void log(const char *fmt, ...)
 * This is what logs everything that goes on with the bot
 * \param *fmt
 */
void log(const char *fmt, ...){
  std::fstream log;
  try{
  log.open(logfile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
  if(!log.is_open())
     throw LogException("Failed to open log file.");
  }catch (LogException &e){
   std::cerr << "Log Exception Caught: " << e.GetReason() << std::endl;
  }
  va_list args;
  va_start(args, fmt);
  
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  
  char buf[512];
  strftime(buf, sizeof(buf) - 1, "[%b %d %H:%M:%S %Y] ", tm);
  log << buf;
  vsnprintf(buf, sizeof(buf), fmt, args);
  log << Flux::Sanitize(buf) << std::endl;
  va_end(args);
  va_end(args);
  log.close();
}
/** 
 * \fn std::vector<Flux::string> StringVector(const Flux::string &src, char delim)
 * \brief creates a vector that breaks down a string word-by-word using the delim as the seperater
 * \param src The source string for it to break down
 * \param delim The char used to seperate the words in the source string
 */
std::vector<Flux::string> StringVector(const Flux::string &src, char delim){
 sepstream tok(src, delim);
 Flux::string token;
 std::vector<Flux::string> ret;
 while(tok.GetToken(token))
   ret.push_back(token);
 return ret;
}
/** 
 * \fn void ProcessJoin(CommandSource &source, const Flux::string &chan)
 * \brief Processes the /who numeric (352), this should only be used in Process() unless its for something special
 * \param source CommandSource struct used to find all the information needed to make new users
 * \param chan The channel we're processing
 */
void ProcessJoin(CommandSource &source, const Flux::string &chan){
    std::vector<Flux::string> &params = source.params;
    if(params.size() < 7)
      return;
    Flux::string channel = params[1];
    Flux::string Ident = params[2];
    Flux::string Host = params[3];
    Flux::string Server = params[4];
    Flux::string Nickname = params[5];
    Flux::string opstatus = params[6];
    Flux::string realname = params[7].erase(0,2);
    /*******************************************************/
    User *u = finduser(Nickname);
    if(!u){
      if(!Host.empty() || !Nickname.empty() || !Ident.empty())
	u = new User(Nickname, Ident, Host, realname, Server);
    }
    Channel *c = findchannel(channel);
    if(!c){
     if(!channel.empty())
       c = new Channel(channel);
    }
}
/*********************************************************************************/

/** 
 * \fn void process(const Flux::string &buffer)
 * \brief Main Processing function
 * \param buffer The raw socket buffer
 */
void process(const Flux::string &buffer){
  Flux::string buf = buffer;
  buf = buf.replace_all_cs("  ", " ");
  if(buf.empty())
    return;
  Flux::string source;
  if(buf[0] == ':'){
   size_t space = buf.find_first_of(" ");
   if(space == Flux::string::npos)
     return;
   source = buf.substr(1, space - 1);
   buf = buf.substr(space + 1);
   if(source.empty() || buf.empty())
     return;
  }
  sepstream bufferseparator(buf, ' ');
  Flux::string bufferseparator_token;
  Flux::string command = buf;
  if(bufferseparator.GetToken(bufferseparator_token))
    command = bufferseparator_token;
  std::vector<Flux::string> params;
  
  while(bufferseparator.GetToken(bufferseparator_token)){
    if(bufferseparator_token[0] == ':'){
      if(!bufferseparator.StreamEnd())
	params.push_back(bufferseparator_token.substr(1)+" "+bufferseparator.GetRemaining());
      else
	params.push_back(bufferseparator_token.substr(1));
      break;
    }
    else
      params.push_back(bufferseparator_token);
  }
  if(protocoldebug){
   printf("--> %s\n", Flux::Sanitize(buffer).c_str());
   printf("Source: %s\n", source.empty() ? "No Source" : source.c_str());
   printf("%s: %s\n", command.is_number_only() ? "Numeric" : "Command", command.c_str());
   if(params.empty())
     printf("No Params\n");
   else
     for(unsigned i =0; i < params.size(); ++i)
       printf("Params %i: %s\n", i, Flux::Sanitize(params[i]).c_str());
  }
  /**************************************/
  const Flux::string &receiver = params.size() > 0 ? params[0] : "";
  Flux::string message = params.size() > 1? params[1] : "";
  
  if(command == "PRIVMSG"){
    if(protocoldebug){
    }else
      if(!receiver.empty() && !params[1].empty())
       printf("<%s-%s> %s\n", isolate(':','!',source).c_str(), receiver.c_str(), params[1].c_str());
  }else
    if(!protocoldebug) printf("--> %s\n", Flux::Sanitize(buffer).c_str());
  /* make local variables instead of global ones */
  Flux::string nickname = isolate(':','!',source),
  uident = isolate('!','@',source),
  uhost = isolate('@',' ',source);
  
  User *u = finduser(nickname);
  Channel *c = findchannel(receiver);
  
  if(!u){
    if(nickname.empty() || uident.empty() || uhost.empty()){ }else
      u = new User(nickname, uident, uhost);
  }
  if(command == "QUIT"){
   if(u)
     delete u;
  }
  if(command == "PART"){
    if(IsValidChannel(receiver)){
     c = findchannel(receiver);
     if(c && u && u->nick == nick)
       delete c;
    }else{
     delete u; 
    }
  }
  if(command == "NICK"){
   if(u){
     if(u->nick == nick){
       nick = params[0];
       delete u; //we shouldnt be a user in the 1st place (:
     }else if(u->nick == owner_nick){
       owner_nick = params[0];
     }
     else{
       delete u; //we delete the user because the above if statement makes a new one for the nick change
     }
   }
  }
  if (command == "PING"){
      Send->raw("PONG :%s", params[0].c_str());
  }
  if(command == "JOIN"){
    if(!u){
      if(nickname.empty() || uident.empty() || uhost.empty()){ }else
      u = new User(nickname, uident, uhost);
    }
     c = findchannel(receiver);
     if(!c){
       if(IsValidChannel(receiver))
         c = new Channel(receiver);
     }
     if(u->nick == nick)
       c->SendWho();
  }
  /**************************************/
  CommandSource Source;
  Source.u = u; //User class
  Source.c = c; //Channel class
  Source.command = command;
  Source.message = message;
  Source.params = params;
  Source.raw = buffer;
  if(command == "352"){
   ProcessJoin(Source, c->name); 
  }
  std::vector<Flux::string> params2 = StringVector(message, ' ');
  if(source.empty() || message.empty() || params2.empty())
    return;
  ProcessModules(Source, params2);
}
/* butt-plug?
 * http://www.albinoblacksheep.com/flash/plugs */