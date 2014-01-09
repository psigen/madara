
#include "madara/knowledge_engine/containers/Vector.h"
#include "madara/knowledge_engine/containers/Vector_N.h"
#include "madara/knowledge_engine/containers/Map.h"
#include "madara/knowledge_engine/Knowledge_Base.h"
#include <iostream>

namespace engine = Madara::Knowledge_Engine;
namespace containers = engine::Containers;

void test_vector (void)
{
  std::cout << "************* VECTOR: CREATING VECTOR*************\n";
  engine::Knowledge_Base knowledge;
  containers::Vector vector ("test_vector", 10, knowledge);
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
  
  if (vector.get_name () == "test_vector")
    std::cout << "SUCCESS. vector.name () returned test_vector.\n";
  else
    std::cout << "FAIL. vector.name () did not return test_vector.\n";

  knowledge.print ();
}

void test_vector_n (void)
{
  std::cout << "************* VECTOR_N: CREATING 3-D VECTOR*************\n";
  engine::Knowledge_Base knowledge;
  containers::Array_N test_vector ("test_vector", knowledge);

  // create a 3-dimensional vector
  containers::Vector_N::Index index (3);

  // set the elements of the 3 dimensional array to be easily identifiable
  for (Madara::Knowledge_Record::Integer i = 0; i < 10; ++i)
  {
    for (Madara::Knowledge_Record::Integer j = 0; j < 10; ++j)
    {
      for (Madara::Knowledge_Record::Integer k = 0; k < 10; ++k)
      {
        index[0] = i;
        index[1] = j;
        index[2] = k;
        test_vector.set (index, i * 100 + j * 10 + k);
      }
    }
  }

  /**
   * Contents of array should look like this:
   * test_vector.0.0.0 = 0;
   * ...
   * test_vector.1.0.0 = 100;
   * ...
   * test_vector.3.3.3 = 333;
   * ...
   **/

  bool failed = false;

  
  /**
   * Test the contents of the array
   **/
  for (Madara::Knowledge_Record::Integer i = 0; i < 10; ++i)
  {
    for (Madara::Knowledge_Record::Integer j = 0; j < 10; ++j)
    {
      for (Madara::Knowledge_Record::Integer k = 0; k < 10; ++k)
      {
        index[0] = i;
        index[1] = j;
        index[2] = k;

        if (test_vector[index] != i * 100 + j * 10 + k)
        {
          failed = true;
        }
      }
    }
  }

  if (failed)
    std::cout << "FAIL. Vector_N did not set array contents properly.\n";
  else
    std::cout << "SUCCESS. Vector_N set the array contents properly\n";

  knowledge.print ("\nVector_N results snapshot:\n");
  knowledge.print ("test_vector.1.2.3 = {test_vector.1.2.3}\n");
  knowledge.print ("test_vector.2.3.4 = {test_vector.2.3.4}\n");
  knowledge.print ("test_vector.8.7.2 = {test_vector.8.7.2}\n");
}

void test_map (void)
{
  std::cout << "************* MAP: SETTING KEY:VALUE PAIRS*************\n";
  engine::Knowledge_Base knowledge;
  std::vector <std::string> keys;
  containers::Map map ("test_map", knowledge);
  
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
  
  std::cout << "************* MAP: COPYING MAP FROM KB*************\n";
  containers::Map copy ("test_map", knowledge);
  
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

  
  std::cout << "************* MAP: SYNCING KEYS*************\n";

  knowledge.set ("test_map.wife", "Mary Helen MacGregor");

  std::vector <std::string> new_keys (map.sync_keys ());

  if (map.exists ("wife") && !copy.exists ("wife") &&
      new_keys.size () == 1 && new_keys[0] == "wife")
    std::cout << "SUCCESS. map.sync_keys () worked.\n";
  else
    std::cout << "FAIL. map.sync_keys () did not work.\n";

  if (map.get_name () == "test_map" && copy.get_name () == "test_map")
    std::cout << "SUCCESS. map.name () returned test_map.\n";
  else
    std::cout << "FAIL. map.name () did not return test_map.\n";

  knowledge.print ();
}

int main (int argc, char * argv[])
{
  test_vector ();
  test_map ();
  test_vector_n ();

  return 0;
}
