/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef defs_h
#define defs_h
#include "../inireader/INIReader.h"
#include "flux.h"
#define GetCurrentDir getcwd

/*List the constants used throughout the script.
If you wanna edit the server, channel, nick, etc that the bot
connects to, do so here, NOT in the main code.
*/
char **my_av, **my_envp;
int days, hours, mins;
bool nofork, dev, protocoldebug, IsOper, quitting = false;
const Flux::string nl = "\n";
Flux::string binary_path, bot_bin, binary_dir, quitmsg;
const Flux::string password = make_pass();
/**********************Version Variables*************************/
#define welcome_msg "%s has joined. Type '/msg %s help' to see a list of commands."
#define ACCESS_DENIED "Access is Denied."
#define VERSION_MAJOR "2"
#define VERSION_MINOR "0"
#define VERSION_PATCH "1"
#define VERSION_EXTRA "-with-netflix-and-chicken"
const Flux::string VERSION_SHORT = Flux::stringify(VERSION_MAJOR)+"."+Flux::stringify(VERSION_MINOR)+"."+Flux::stringify(VERSION_PATCH);
const Flux::string VERSION = Flux::stringify(VERSION_MAJOR)+"."+Flux::stringify(VERSION_MINOR)+"."+Flux::stringify(VERSION_PATCH)+Flux::stringify(VERSION_EXTRA);
/******************Configuration variables***********************/
Flux::string logfile, nsacc, nspass, owner_nick, realname, usrname, nick, channel, port, server, LogChannel, pid_file, usrpass, ouser, opass;
/**Rehash void
 * This will re-read the config file values when told to do so
 * @param ReadConfig(INIReader &config)
 */
void ReadConfig(INIReader &config){
logfile = config.Get("Log","Log_File","navn.log");
nsacc = config.Get("Bot","NickServ_Account","");
nspass = config.Get("Bot","NickServ_Password","");
owner_nick = config.Get("Bot","Owner","Derp");
realname = config.Get("Connect","Realname","The Navn Bot "+VERSION_SHORT.tostd());
usrname = config.Get("Connect","Ident","Navn");
nick = config.Get("Bot","Nick","Navn");
channel = config.Get("Bot","Channel","#Test");
port = config.Get("Connect","Port","6667");
server = config.Get("Connect", "Server", "irc.flux-net.net");
pid_file = config.Get("Log","PID_File","navn.pid");
usrpass = config.Get("Bot","Password","Navn");
ouser = config.Get("Oper","Oper_Username","");
opass = config.Get("Oper","Oper_Password","");
LogChannel = config.Get("Modules", "LogChannel","");
std::cout << "\033[22;31mReading Config File\033[22;30m...\033[22;36m" << std::endl;
}
/******************End Configuration variables********************/
#endif

