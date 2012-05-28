/* Navn IRC bot -- Example module
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "modules.h"

/**
 * \example dummy.cpp
 * This is an example module showing a simple if-said-that-reply-this situation.
 * \file dummy.cpp Source file holding the \a dummy function.
 * \author Lordofsraam
 */

/**
 * \defgroup dummyM Dummy (Example) Module
 * This is a template or example module.
 * \section commands Commands associated with this module.
 * \subsection test testing testing
 * Say \a test to see if the bot replies :P
 * @{
 */

/**
 * \fn class commanddummy() : Command("TEST", 0, 0)
 * \brief Replies to a test
 * We will try to put as many examples here in the future as we can.
 */
class commanddummy : public Command
{
public:
  commanddummy(module *m) : Command(m, "TEST", C_PRIVATE, 0, 0) //The 0's are how many params the command gets, they're not required and can be removed.
  {
   this->SetDesc("Test for the modules");
   this->SetSyntax("\37TEST\37");
  }
  void Run(CommandSource &source, const Flux::vector &params)
  {
    source.Reply("YAY!");
  }
  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    this->SendSyntax(source);
    source.Reply(" ");
    source.Reply("This command simply emmits 'YAY!' to the user.\n"
		 "This command is mostly only used to test other core\n"
		 "functions or modules in the bot");
    return true;
  }
};

class dummy : public module
{
  commanddummy cmddmy; //Declare our command
public:
  dummy(const Flux::string &Name):module(Name), cmddmy(this) //Add our command to teh bot
  {
    this->SetAuthor("Lordofsraam"); //Set the author
    this->SetVersion(VERSION);
    this->SetPriority(PRIORITY_LAST);

    //Implementation i[] = {  }; //Add that we have a module hook, this can be done in 2 ways
    ModuleHandler::Attach(I_OnPrivmsgChannel, this);
    /*or you can do the easy way
     * ModuleHandler::Attach(i, this, sizeof(i)/sizeof(Implementation));
     */
  }
  virtual void OnPrivmsgChanenl(User *u, Channel *c, const Flux::vector &params)
  {
    Flux::string s;
    for(unsigned i=0; i < params.size(); ++i)
      s = params[i]+' ';
    s.trim();
    if(s.search_ci("Blah Blah"))
      u->SendMessage("Derp DERp");
   if(params[0].equals_ci("!userlist"))
   {
     Flux::string users;
    for(Flux::insensitive_map<User*>::iterator it = UserNickList.begin(); it != UserNickList.end(); ++it)
      users += it->second->nick+" ";
    u->SendMessage(users);
   }
  }
};

MODULE_HOOK(dummy)
/**
 * @}
 */