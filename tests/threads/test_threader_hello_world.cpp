
#include <iostream>
#include "madara/knowledge_engine/Knowledge_Base.h"
#include "madara/threads/Threader.h"

// some shortcuts for common MADARA namespaces
namespace engine = Madara::Knowledge_Engine;
namespace threads = Madara::Threads;

// 1: Extend a Base_Thread
class Hello_World : public threads::Base_Thread
{
  public:
    void execute (void)
    {
      std::cerr << "Hello World!\n";
    }
};

int main (int argc, char ** argv)
{
  // 2: Create a Knowledge Base
  engine::Knowledge_Base * knowledge = new engine::Knowledge_Base ();

  // 3: Create a Threader 
  threads::Threader * threader = new threads::Threader (knowledge);

  // 4: Run threads
  threader->run ("hello_world", new Hello_World ());

  // 5: Wait for threads to finish
  threader->wait ();

  delete threader;
  delete knowledge;

  return 0;
}
