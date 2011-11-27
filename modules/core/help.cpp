/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#include "flux_net_irc.hpp"
/**
 * \file help.cpp Module file holding the \a help function.
 * \author Lordofsraam. Polished by Justasic.
 */

/**
 * \defgroup helpM Help Module
 * This is the module for the help_m function.
 * Returns help information about controlling Navn.
 * \section commands Commands associated with this module.
 * \subsection help Private message: help
 * Send a private message saying \a help to get information.
 * @{
 */

/**
 * \fn void help_m(Socket &sock, irc_string *reply)
 * \brief Returns help info on Navn.
 * Tells you most of the commands you can use on Navn.
 */
class CommandHelp : public Command
{
public:
  CommandHelp():Command("HELP", 0, 1)
  {
   this->SetDesc("Displays help messages");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    int c=0;
    if(!params.empty()){
      Command *com = FindCommand(params[1], COMMAND_PRIVATE);
      if(com && !com->OnHelp(source, ""))
	source.Reply("No help available for \2%s\2", params[1].c_str());
      else if(!com)
	source.Reply("No help available for \2%s\2", params[1].c_str());
      Log(source.u) << "used help command on " << params[1];
    }else{
#ifdef _CXX11
      for(auto val : Commandsmap)
      {
	source.Reply("\2%-16s\2 %s", val.second->name.c_str(), val.second->GetDesc().c_str());
	++c;
      }
#else
      for(CommandMap::iterator it = Commandsmap.begin(), it_end = Commandsmap.end(); it != it_end; ++it)
      {
	  source.Reply("\2%-16s\2 %s", it->second->name.c_str(), it->second->GetDesc().c_str());
	  ++c;
      }
#endif
      source.Reply("Total of \2%i\2 commands", c);
      Log(source.u) << "used help command";
    }
  }
};

class CommandCHelp : public Command
{
public:
  CommandCHelp():Command("!HELP", 0,1)
  {
   this->SetDesc("Displays Channel help messages");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    Flux::string cmds;
    if(!params.empty()){
      Command *c = FindCommand(params[1], COMMAND_CHANNEL);
      if(c && !c->OnHelp(source, ""))
	source.Reply("No help available for \2%s\2", params[1].c_str());
      else if(!c)
	source.Reply("No help available for \2%s\2", params[1].c_str());
      Log(source.u) << "used help command on " << params[1];
    }else{
#ifdef _CXX11
      for(auto val : ChanCommandMap) //As you can see C++11 is MUCH better than C++98
	cmds += val.second->name+" ";
#else
      for(CommandMap::iterator it = ChanCommandMap.begin(), it_end = ChanCommandMap.end(); it != it_end; ++it)
      {
	if(it->second != NULL)
	{
	    cmds += it->second->name+" ";
	}
      }
#endif
      cmds.trim();
      cmds.tolower();
      source.c->SendMessage("Local %s Commands:", source.c->name.c_str());
      source.c->SendMessage(cmds);
      Log(source.u) << "used help command";
    }
  }
};
class help_m:public module
{
  CommandHelp help;
  CommandCHelp chelp;
public:
  help_m(const Flux::string &Name):module(Name){ 
    this->AddCommand(&help);
    this->AddChanCommand(&chelp);
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_FIRST);
    this->SetAuthor("Justasic");
  }
};
MODULE_HOOK(help_m)
/**
 * @}
 */