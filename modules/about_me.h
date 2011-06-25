#ifndef ABOUT_ME_H
#define ABOUT_ME_H
#include "../includes.h"
#include "../flux_net_irc.hpp"
/* About Me Module
 * This module was created by lordofsraam but functions
 * coded by Justasic
 * 
 * the module is more of a test module for classes but 
 * could be used for something else.
 */
using namespace std;
using namespace flux_net_irc;
void about_me(Socket &sock, irc_string *reply){

  if(reply->said("about me")){
    sock << notice(unick, "Raw: "+reply->raw_string);
    sock << notice(unick, "message: "+msg);
    sock << notice(unick, "Nickname: "+unick);
    sock << notice(unick, "Ident: "+reply->user);
    sock << notice(unick, "Host: "+host);
    sock << notice(unick, "Channel: "+chan);
    sock << notice(unick, "Fullhost: "+fullhost);
    log("%s requested information about themself.", unick.c_str());
   }
    if(reply->said("!decodehost")){
	string nerp = reply->params(1);
	IsoHost* Host = new IsoHost(nerp);
	sock << privmsg(chan, "Nick: %s", Host->nick.c_str());
	sock << privmsg(chan, "User: %s", Host->user.c_str());
	sock << privmsg(chan, "Host: %s", Host->host.c_str());
	sock << privmsg(chan, "Raw: %s", Host->raw.c_str());
	delete Host;
    }
}
#endif