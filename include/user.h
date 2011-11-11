/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef USER_H
#define USER_H
#include "ircproto.h"
#include "channel.h"
/**
 * \class User "user.h" USER_H
 * \brief A Class for IRC Users
 * This class wraps around IRC Users and includes several useful commands as well as User information.
 */
typedef std::map<Channel*, User*> CList;
class User : public Base
{
public:
  User(const Flux::string&, const Flux::string&, const Flux::string&, const Flux::string &realname = "", const Flux::string &server ="");
  virtual ~User();
  CList ChannelList;
  Flux::string nick,host,realname,ident,fullhost,server;
  void kick(const Flux::string&, const Flux::string&);
  void kick(Channel*, const Flux::string&);
  void kill(const Flux::string&);
  bool IsOwner();
  void AddChan(Channel*);
  void DelChan(Channel*);
  Channel *findchannel(const Flux::string&);
  void SetNewNick(const Flux::string&);
  void SendWho();
  void SendMessage(const Flux::string&);
  void SendMessage(const char*, ...);
  void SendPrivmsg(const Flux::string&);
  void SendPrivmsg(const char*, ...);
};
#endif