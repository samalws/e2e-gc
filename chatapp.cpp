#include "chatapp.h"

groupchat::groupchat(const aes_key key, groupchat* prevchat, const std::map<rsa_pubkey,std::string> members) :
  key(key),
  prevchat(prevchat),
  members(members)
{}

void groupchat::delete_previous() {
  if (!prevchat) return;
  prevchat->delete_previous();
  delete prevchat;
  prevchat = NULL;
}

groupchat* groupchat::get_prevchat() { return prevchat; }

void groupchat::add_message(message msg) {
  messages.push_front(msg);
}
void groupchat::add_messages(std::list<message> msgs) {
  messages.insert(messages.begin(), msgs.begin(),msgs.end());
}

const std::list<message> groupchat::get_messages() const {
  return messages;
}
std::list<message>::const_iterator groupchat::get_messages_iter() const {
  return messages.begin();
}
const std::map<rsa_pubkey,std::string> groupchat::get_members() const { return members; }

bool groupchat::has_member(rsa_pubkey key) const {
  return members.find(key) != members.end(); // return members.contains(key); but c++20 is too new apparently
}
std::string groupchat::lookup_member(rsa_pubkey key) const {
  return members.find(key)->second;
}
/* std::optional<rsa_pubkey> groupchat::reverse_lookup_member(std::string name) const {
  for (auto it : members)
    if (it->second == name)
      return it->first;
  return {};
} */

groupchat* groupchat::set_members(const std::map<rsa_pubkey,std::string> new_members) {
  return new groupchat(key, this, new_members);
}
groupchat* groupchat::add_member(rsa_pubkey key,std::string name) {
  auto new_members = members;
  new_members[key] = name;
  return set_members(new_members);
}
groupchat* groupchat::remove_member(rsa_pubkey key) {
  auto new_members = members;
  new_members.erase(new_members.find(key));
  return set_members(new_members);
}
