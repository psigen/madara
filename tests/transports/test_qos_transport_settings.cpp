

#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/transport/QoS_Transport_Settings.h"
#include "madara/utility/Log_Macros.h"
#include <iostream>
#include <iomanip>
#include <string>

void test_rebroadcast_settings (void)
{
  // variables used in this test
  Madara::Transport::QoS_Transport_Settings settings;
  uint32_t current_mask, old_mask, current_types;
  uint32_t current_ttl, old_ttl;
  
  std::cerr << std::hex;

  // get the old values
  old_mask = settings.add_rebroadcastable_type (0);
  current_types = Madara::Knowledge_Record::ALL_TYPES;

  // add all types
  current_mask = settings.add_rebroadcastable_type (current_types);
  std::cerr << "\nAdding types " << current_types << " to " << old_mask;
  std::cerr << " resulted in " << current_mask << "\n";

  // remove jpeg images from rebroadcastable types
  old_mask = current_mask;
  current_types = Madara::Knowledge_Record::IMAGE_JPEG;
  current_mask = settings.remove_rebroadcastable_type (current_types);
  std::cerr << "Removing types " << current_types << " from " << old_mask;
  std::cerr << " resulted in " << current_mask << "\n";

  if (settings.is_rebroadcastable_type (current_types))
  {
    std::cerr << "  Removal failed.\n";
  }
  else
  {
    std::cerr << "  Removal succeeded.\n";
  }

  // remove all file types
  old_mask = current_mask;
  current_types = Madara::Knowledge_Record::ALL_FILE_TYPES;
  current_mask = settings.remove_rebroadcastable_type (current_types);
  std::cerr << "Removing types " << current_types << " from " << old_mask;
  std::cerr << " resulted in " << current_mask << "\n";
  
  if (settings.is_rebroadcastable_type (current_types))
  {
    std::cerr << "  Removal failed.\n";
  }
  else
  {
    std::cerr << "  Removal succeeded.\n";
  }

  // remove all types
  old_mask = current_mask;
  current_types = Madara::Knowledge_Record::ALL_TYPES;
  current_mask = settings.remove_rebroadcastable_type (current_types);
  std::cerr << "Removing types " << current_types << " from " << old_mask;
  std::cerr << " resulted in " << current_mask << "\n";

  if (settings.is_rebroadcastable_type (
    Madara::Knowledge_Record::INTEGER | Madara::Knowledge_Record::DOUBLE))
  {
    std::cerr << "  Removal failed.\n";
  }
  else
  {
    std::cerr << "  Removal succeeded.\n";
  }
  
  // add integers and doubles to rebroadcastable types
  old_mask = current_mask;
  current_types =
    Madara::Knowledge_Record::INTEGER | Madara::Knowledge_Record::DOUBLE;
  current_mask = settings.add_rebroadcastable_type (current_types);
  std::cerr << "Adding types " << current_types << " to " << old_mask;
  std::cerr << " resulted in " << current_mask << "\n";
  
  if (settings.is_rebroadcastable_type (
    Madara::Knowledge_Record::INTEGER | Madara::Knowledge_Record::DOUBLE))
  {
    std::cerr << "  Addition succeeded.\n";
  }
  else
  {
    std::cerr << "  Addition failed.\n";
  }
  
  std::cerr << "Setting rebroadcast ttl to 3... ";
  // set the time to live for rebroadcast messages
  old_ttl = settings.set_rebroadcast_ttl (3);
  current_ttl = settings.get_rebroadcast_ttl ();

  if (old_ttl == current_ttl && current_ttl == 3)
    std::cerr << "succeeded.\n";
  else
    std::cerr << "failed.\n";
}

void test_peer_list (void)
{
  Madara::Transport::QoS_Transport_Settings settings;

  std::string peer1 = "127.0.0.1:13000";
  std::string peer2 = "127.0.0.1:13001";
  std::string peer3 = "127.0.0.1:13002";
  std::string peer4 = "127.0.0.1:13003";
  std::string peer5 = "127.0.0.1:13004";

  // check the defaults (all peers trusted)
  std::cerr << "Checking default trust:\n";
  std::cerr << "  " << peer1 << "=" << settings.is_trusted (peer1) << "\n";
  std::cerr << "  " << peer2 << "=" << settings.is_trusted (peer2) << "\n";
  std::cerr << "  " << peer3 << "=" << settings.is_trusted (peer3) << "\n";
  std::cerr << "  " << peer4 << "=" << settings.is_trusted (peer4) << "\n";
  std::cerr << "  " << peer5 << "=" << settings.is_trusted (peer5) << "\n";
  
  std::cerr << "Adding " << peer5 << " and " << peer2 <<
    " to the banned list:\n";
  settings.add_banned_peer (peer5);
  settings.add_banned_peer (peer2);
  std::cerr << "  " << peer1 << "=" << settings.is_trusted (peer1) << "\n";
  std::cerr << "  " << peer2 << "=" << settings.is_trusted (peer2) << "\n";
  std::cerr << "  " << peer3 << "=" << settings.is_trusted (peer3) << "\n";
  std::cerr << "  " << peer4 << "=" << settings.is_trusted (peer4) << "\n";
  std::cerr << "  " << peer5 << "=" << settings.is_trusted (peer5) << "\n";
  
  std::cerr << "Removing " << peer2 <<
    " from the banned list:\n";
  settings.remove_banned_peer (peer2);
  std::cerr << "  " << peer1 << "=" << settings.is_trusted (peer1) << "\n";
  std::cerr << "  " << peer2 << "=" << settings.is_trusted (peer2) << "\n";
  std::cerr << "  " << peer3 << "=" << settings.is_trusted (peer3) << "\n";
  std::cerr << "  " << peer4 << "=" << settings.is_trusted (peer4) << "\n";
  std::cerr << "  " << peer5 << "=" << settings.is_trusted (peer5) << "\n";
  
  std::cerr << "Adding " << peer5 <<
    " to the trusted list:\n";
  settings.add_trusted_peer (peer5);
  std::cerr << "  " << peer1 << "=" << settings.is_trusted (peer1) << "\n";
  std::cerr << "  " << peer2 << "=" << settings.is_trusted (peer2) << "\n";
  std::cerr << "  " << peer3 << "=" << settings.is_trusted (peer3) << "\n";
  std::cerr << "  " << peer4 << "=" << settings.is_trusted (peer4) << "\n";
  std::cerr << "  " << peer5 << "=" << settings.is_trusted (peer5) << "\n";
  
  std::cerr << "Removing " << peer5 <<
    " from the trusted list:\n";
  settings.remove_trusted_peer (peer5);
  std::cerr << "  " << peer1 << "=" << settings.is_trusted (peer1) << "\n";
  std::cerr << "  " << peer2 << "=" << settings.is_trusted (peer2) << "\n";
  std::cerr << "  " << peer3 << "=" << settings.is_trusted (peer3) << "\n";
  std::cerr << "  " << peer4 << "=" << settings.is_trusted (peer4) << "\n";
  std::cerr << "  " << peer5 << "=" << settings.is_trusted (peer5) << "\n";

  settings.add_banned_peer (peer3);
  settings.add_banned_peer (peer4);

  Madara::Transport::QoS_Transport_Settings settings_copy (settings);
  Madara::Transport::QoS_Transport_Settings settings_assignment;
  settings_assignment = settings;

  std::cerr << "Testing copy constructors and assignments.\n";
  std::cerr << "  Copy constructor = ";

  if (!settings_copy.is_trusted (peer3) &&
      !settings_copy.is_trusted (peer4) &&
      settings_copy.is_trusted (peer2))
    std::cerr << "SUCCESS\n";
  else
    std::cerr << "FAILURE\n";
  
  std::cerr << "  Assignment operator = ";

  if (!settings_assignment.is_trusted (peer3) &&
      !settings_assignment.is_trusted (peer4) &&
      settings_assignment.is_trusted (peer1))
    std::cerr << "SUCCESS\n";
  else
    std::cerr << "FAILURE\n";
}

int main (int argc, char * argv[])
{
  test_rebroadcast_settings ();
  test_peer_list ();

  return 0;
}
