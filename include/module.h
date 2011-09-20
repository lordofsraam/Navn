/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef MODULE_H
#define MODULE_H
#include <user.h>
#include <includes.h>
enum Implementation{
  I_BEGIN,
	I_OnPrivmsg,
  I_END
};
enum ModuleReturn{
  MOD_RUN,
  MOD_STOP
};
enum ModulePriority{
  PRIORITY_FIRST,
  PRIORITY_DONTCARE,
  PRIORITY_LAST
};
class module{ 
  Flux::string desc;
protected:
  void SetDesc(const Flux::string&);
public:
  void *handle;
  Flux::string name, filename;
  const Flux::string GetDesc() const;
  bool activated;
  ModulePriority priority;
  Flux::string author;
  module (const Flux::string&, bool, ModulePriority);
  int AddCommand(Command *c);
  int DelCommand(Command *c);
  
  virtual ModuleReturn run(CommandSource&, std::vector<Flux::string>&) =0;
  virtual void OnPrivmsg(const Flux::string&, const std::vector<Flux::string>&) { }
  
};

class ModuleHandler
{
public:
  static std::vector<module *> EventHandlers[I_END];
  static bool LoadModule(const Flux::string&);
  static bool DeleteModule(module*);
  static void SanitizeRuntime();
  static void UnloadAll();
  
  static bool Attach(Implementation i, module *mod);
  static bool Detach(Implementation i, module *mod);
};
#endif