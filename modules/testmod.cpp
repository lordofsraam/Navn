#include "flux_net_irc.hpp"

/*This module setup needs serious work!
 * Justasic will work on it but its going to be hard with los's module class.
 */
class commanddummy : public Command
{
public:
  commanddummy() : Command("ModuleTest", 1, 1)
  {
   this->SetDesc("Test for the modules");
   this->SetSyntax("\2ModuleTest\2");
  }
  void Run(CommandSource &source, const std::vector<Flux::string> &params)
  {
    source.Reply("YAY!");
  }
  
};
class dummy : public module
{
  commanddummy cmddmy;
public:
  dummy():module("Dummy_bin", PRIORITY_LAST)
  { 
    this->AddCommand(&cmddmy);
    this->SetAuthor("Justasic");
    
    ModuleHandler::Attach(I_OnReload, this);
  }
  void OnReload(bool)
  {
   printf("RELOAD! YAAY!\n"); 
  }
  
};

MODULE_HOOK(dummy)