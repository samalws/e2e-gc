#ifndef chatapp_h
#define chatapp_h

#include <map>
#include <list>
#include <string>
// #include <optional>
#include "aes.h"
#include "rsa.h"

// TODO do using std::...; you degenerate
// TODO maybe make it so multiple people can't have the same nickname

class message {
public:
  rsa_pubkey sender;
  std::string text;
  // TODO signature
};

class groupchat {
public:
  groupchat(const aes_key key, groupchat* prevchat, const std::map<rsa_pubkey,std::string> members);

  void delete_previous(); // delete prevchat, its prevchat, and so on

  groupchat* get_prevchat();

  void add_message(message);
  void add_messages(std::list<message>);

  const std::list<message> get_messages() const;
  std::list<message>::const_iterator get_messages_iter() const;

  bool has_member(rsa_pubkey) const;
  const std::map<rsa_pubkey,std::string> get_members() const;
  std::string lookup_member(rsa_pubkey) const; // TODO make this std::optional
  // TODO std::optional<rsa_pubkey> reverse_lookup_member(std::string) const;

  groupchat* set_members(const std::map<rsa_pubkey,std::string>);
  groupchat* add_member(rsa_pubkey key,std::string name);
  groupchat* remove_member(rsa_pubkey key);

private:
  const aes_key key;
  groupchat* prevchat; // whenever a member is added or removed, or a member changes name; first one should have NULL as its prevchat
  const std::map<rsa_pubkey,std::string> members;
  std::list<message> messages;
};

#endif
