/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef defs_h
#define defs_h
#include "INIReader.h"
#include "extern.h"
#include "flux.h"
/**
 * \include INIReader.h
 * \include extern.h
 * \include flux.h
 */

/**
 * \file defs.h
 * List the constants used throughout the script.
 * If you wanna edit the server, channel, nick, etc that the bot
 * connects to, do so here, NOT in the main code.
*/
#ifdef HAVE_SETJMP_H
jmp_buf sigbuf;
#endif
char **my_av, **my_envp;
bool nofork, dev, protocoldebug, IsOper, quitting, started, nocolor, istempnick = false;
Flux::string binary_path, bot_bin, binary_dir, quitmsg, server_name, LastBuf;
const Flux::string password = make_pass();
char segv_location[255];
time_t starttime = 0;
/**********************Version Variables*************************/
const Flux::string VERSION_LONG = value_cast<Flux::string>(VERSION)+"-"+value_cast<Flux::string>(VERSION_GIT)+value_cast<Flux::string>(VERSION_EXTRA);
#endif

