/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef HELP_H
#define HELP_H

/**
 * \file help.h Header file holding the \a help function.
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
class help_m:public module{
public:
  help_m(bool a):module("Help", a, PRIORITY_DONTCARE){ this->SetDesc("The Help module"); }
  ModuleReturn run(CommandSource &source, std::vector<Flux::string> &params){
    Flux::string cmd = params.empty()?"":params[0];
  //help replies
    if(cmd.equals_ci("help")){
      source.Reply("There are 10 commands:");
      source.Reply("QUIT \t \t \tQuits the bot (password needed)");
      source.Reply("PASS\t \t \t Gets the quit password for the bot (must be a bot admin)");
      source.Reply("CHOWN \t \t Changes ownership over the bot (must be a bot admin)");
      source.Reply("JOIN \t \t \tTells the bot to join the specified channel.");
      source.Reply("PART \t \t \tParts the channel");
      source.Reply("KICK \t \t \tkicks a user from the channel (must be bot owner)");
      source.Reply("RESTART \t Restarts the Bot (Password needed)");
      source.Reply("REHASH\t \t Rehashes the Bot (Password needed)");
      source.Reply("STATS \t \t Shows system statistics.");
      source.Reply("NICK \t \t \tSets the bots nickname");
      
      log(LOG_NORMAL, "%s used help command", source.u->nick.c_str());
   }
   return MOD_RUN;
  }
};
/**
 * @}
 */
#endif