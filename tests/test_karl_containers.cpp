
#include "madara/knowledge_engine/Vector.h"
#include "madara/knowledge_engine/Map.h"
#include "madara/knowledge_engine/Knowledge_Base.h"
#include <iostream>

namespace engine = Madara::Knowledge_Engine;

void test_vector (void)
{
  engine::Knowledge_Base knowledge;
  engine::Vector vector ("test_vector", 10, knowledge);
  unsigned int size = vector.size ();

  vector.set (1, "value.at.1");
  vector.set (7, 7.7);

  std::cout << "Vector results:\n";

  for (unsigned int i = 0; i < size; ++i)
  {
    std::cout << "  ";
    std::cout << vector[i];
    std::cout << "\n";
  }
  
  std::cout << "\n";

  if (size != 10)
    std::cout << "FAIL. Vector.size != 4\n";
  else
    std::cout << "SUCCESS. Vector.size == 4\n";

  if (vector[1].to_string () == "value.at.1" &&
      vector[7].to_double () == 7.7)
    std::cout << "SUCCESS. Vector[1] and [7] were set and retrieved.\n";
  else
    std::cout << "FAIL. Vector[1] and [7] were not set and retrieved.\n";

  knowledge.print ();
}

void test_map (void)
{
  engine::Knowledge_Base knowledge;
  std::vector <std::string> keys;
  engine::Map map ("test_map", knowledge);
  
  map.set ("name", "Rob Roy");
  map.set ("occupation", "Lord of the MacGregors");
  map.set ("age", Madara::Knowledge_Record::Integer (63));
  map.set ("wives", Madara::Knowledge_Record::Integer (1));

  unsigned int size = map.size ();
  map.keys (keys);
  
  std::cout << "\nMap results:\n";

  for (std::vector <std::string>::iterator i = keys.begin ();
       i != keys.end (); ++i)
    std::cout << "  " << *i << "=" << map[*i] << "\n";
  
  std::cout << "\n";

  if (size != 4)
    std::cout << "FAIL. Map.size != 4\n";
  else
    std::cout << "SUCCESS. Map.size == 4\n";
  
  if (keys.size () != 4)
    std::cout << "FAIL. Map.keys ().size != 4\n";
  else
    std::cout << "SUCCESS. Map.keys ().size == 4\n";
  
  if (map["name"].to_string () == "Rob Roy" &&
      map["occupation"].to_string () == "Lord of the MacGregors" &&
      map["age"].to_integer () == 63 &&
      map["wives"].to_integer () == 1)
    std::cout << "SUCCESS. map values were set and retrieved.\n";
  else
    std::cout << "FAIL. map values were not set and retrieved.\n";

  engine::Map copy ("test_map", knowledge);
  
  size = map.size ();
  map.keys (keys);
  
  if (size != 4)
    std::cout << "FAIL. Map Copy.size != 4\n";
  else
    std::cout << "SUCCESS. Map Copy.size == 4\n";
  
  if (keys.size () != 4)
    std::cout << "FAIL. Map Copy.keys ().size != 4\n";
  else
    std::cout << "SUCCESS. Map Copy.keys ().size == 4\n";
  
  if (copy["name"].to_string () == "Rob Roy" &&
      copy["occupation"].to_string () == "Lord of the MacGregors" &&
      copy["age"].to_integer () == 63 &&
      copy["wives"].to_integer () == 1)
    std::cout << "SUCCESS. map copy values were set and retrieved.\n";
  else
    std::cout << "FAIL. map copy values were not set and retrieved.\n";

  knowledge.print ();
}

int main (int argc, char * argv[])
{
  test_vector ();
  test_map ();

  return 0;
}
