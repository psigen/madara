
//#define MADARA_NTRACE    0
//#define ACE_NTRACE    0
////#define ACE_NLOGGING  0
//#define ACE_NDEBUG    0

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#include "madara/knowledge_engine/Knowledge_Base.h"

// command line arguments
int parse_args (int argc, ACE_TCHAR * argv[]);

Madara::Knowledge_Record
  return_1 (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  return Madara::Knowledge_Record::Integer (1);
}

Madara::Knowledge_Record
  return_named_1 (const char * name, 
            Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  variables.set ("function_name", std::string (name));

  variables.print ("External named function call of {function_name} a SUCCESS\n", 0);

  return Madara::Knowledge_Record::Integer (1);
}

Madara::Knowledge_Record
  return_2 (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  return Madara::Knowledge_Record::Integer (2);
}

Madara::Knowledge_Record
  return_3 (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  return Madara::Knowledge_Record::Integer (3);
}

Madara::Knowledge_Record
  return_var1 (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  return variables.get (".var1");
}

Madara::Knowledge_Record
  set_var1_to_arg1 (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  variables.set (".var1", args[0]);
  return variables.get (".var1");
}


Madara::Knowledge_Record
  set_var1_to_arg2 (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  if (args.size () > 1)
  {
    variables.set (".var1", args[1]);
  }
  return variables.get (".var1");
}

Madara::Knowledge_Record
  check_vector (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  std::vector <Madara::Knowledge_Record> records;

  assert (variables.to_vector ("vector", 1, 8, records) == 8 &&
    records[0].to_string () == "10" &&
    records[1].to_string () == "9" &&
    records[2].to_string () == "8" &&
    records[3].to_string () == "7" &&
    records[4].to_string () == "6" &&
    records[5].to_string () == "5" &&
    records[6].to_string () == "4" &&
    records[7].to_string () == "3");

  return Madara::Knowledge_Record::Integer (records.size ());
}

Madara::Knowledge_Record
  check_map (Madara::Knowledge_Engine::Function_Arguments & args,
            Madara::Knowledge_Engine::Variables & variables)
{
  std::map <std::string, Madara::Knowledge_Record> records;

  assert (variables.to_map ("map*", records) == 8 &&
    records["map1"].to_string () == "10" &&
    records["map2"].to_string () == "9" &&
    records["map3"].to_string () == "8" &&
    records["map4"].to_string () == "7" &&
    records["map5"].to_string () == "6" &&
    records["map6"].to_string () == "5" &&
    records["map7"].to_string () == "4" &&
    records["map8"].to_string () == "3");

  return Madara::Knowledge_Record::Integer (records.size ());
}


// test functions
void test_to_vector (Madara::Knowledge_Engine::Knowledge_Base &  knowledge);
void test_to_map (Madara::Knowledge_Engine::Knowledge_Base &  knowledge);
void test_strings (Madara::Knowledge_Engine::Knowledge_Base &  knowledge);
void test_doubles (Madara::Knowledge_Engine::Knowledge_Base &  knowledge);
void test_logicals (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_implies (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_comparisons (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_conditionals (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_assignments (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_unaries (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_mathops (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_tree_compilation (
  Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_dijkstra_sync (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_both_operator (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_comments (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_functions (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_for_loops (Madara::Knowledge_Engine::Knowledge_Base & knowledge);
void test_simplification_operators (
  Madara::Knowledge_Engine::Knowledge_Base & knowledge);

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode = parse_args (argc, argv);

  if (retcode < 0)
    return retcode;

  ACE_TRACE (ACE_TEXT ("main"));

  Madara::Knowledge_Engine::Knowledge_Base knowledge;

  //knowledge.evaluate ("");

  // run tests
//  test_tree_compilation (knowledge);
  test_mathops (knowledge);
  test_functions (knowledge);
  test_to_vector (knowledge);
  test_to_map (knowledge);
  test_logicals (knowledge);
  test_comparisons (knowledge);
  test_strings (knowledge);
  test_doubles (knowledge);
  test_simplification_operators (knowledge);
  test_for_loops (knowledge);
  test_comments (knowledge);
  test_assignments (knowledge);
  test_unaries (knowledge);
  test_conditionals (knowledge);
  test_implies (knowledge);
  test_both_operator (knowledge);
  test_dijkstra_sync (knowledge);

  knowledge.print_knowledge ();

  return 0;
}

void test_to_vector (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_to_vector"));
  ACE_DEBUG ((LM_INFO, "Testing to_vector function\n"));
  
  std::vector <Madara::Knowledge_Record> records;

  knowledge.clear ();
  
  knowledge.evaluate ("vector1=10; vector2=9; vector3=8; vector4=7");
  knowledge.evaluate ("vector5=6; vector6=5; vector7=4; vector8=3");

  assert (knowledge.to_vector ("vector", 1, 8, records) == 8 &&
    records[0].to_string () == "10" &&
    records[1].to_string () == "9" &&
    records[2].to_string () == "8" &&
    records[3].to_string () == "7" &&
    records[4].to_string () == "6" &&
    records[5].to_string () == "5" &&
    records[6].to_string () == "4" &&
    records[7].to_string () == "3");

  knowledge.define_function ("check_vector", check_vector);

  knowledge.evaluate ("check_vector ()");
}

void test_to_map (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_to_map"));
  ACE_DEBUG ((LM_INFO, "Testing to_map function\n"));
  
  std::map <std::string, Madara::Knowledge_Record> records;

  knowledge.clear ();
  
  knowledge.evaluate ("map1=10; map2=9; map3=8; map4=7");
  knowledge.evaluate ("map5=6; map6=5; map7=4; map8=3");
  
  assert (knowledge.to_map ("map*", records) == 8 &&
    records["map1"].to_string () == "10" &&
    records["map2"].to_string () == "9" &&
    records["map3"].to_string () == "8" &&
    records["map4"].to_string () == "7" &&
    records["map5"].to_string () == "6" &&
    records["map6"].to_string () == "5" &&
    records["map7"].to_string () == "4" &&
    records["map8"].to_string () == "3");
  
  knowledge.define_function ("check_map", check_map);

  knowledge.evaluate ("check_map ()");
}

void test_comparisons (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_comparisons"));
  
  ACE_DEBUG ((LM_INFO, "Testing comparisons\n"));

  knowledge.clear ();
  
  ACE_DEBUG ((LM_INFO, "  Testing string to string comparisons\n"));

  knowledge.evaluate (".var1 = 'bob' < 'cat'; .var2 = 'dear' > 'abby';" \
    ".var3 = 'bob' <= 'cat'; .var4= 'dear' >= 'abby'; .var5 = 'bob' == 'bob'");
  assert (knowledge.get (".var1").to_integer () == 1 && 
    knowledge.get (".var2").to_integer () == 1 &&
    knowledge.get (".var3").to_integer () == 1 &&
    knowledge.get (".var4").to_integer () == 1 &&
    knowledge.get (".var5").to_integer () == 1);
  
  ACE_DEBUG ((LM_INFO, "  Testing int to int comparisons\n"));

  knowledge.evaluate (".var1 = 1 < 10; .var2 = 5 > 3;" \
    ".var3 = 2 <= 4; .var4= 5 >= 3; .var5 = 5 == 5");
  assert (knowledge.get (".var1").to_integer () == 1 && 
    knowledge.get (".var2").to_integer () == 1 &&
    knowledge.get (".var3").to_integer () == 1 &&
    knowledge.get (".var4").to_integer () == 1 &&
    knowledge.get (".var5").to_integer () == 1);
  
  ACE_DEBUG ((LM_INFO, "  Testing double to double comparisons\n"));

  knowledge.evaluate (".var1 = 1.0 < 10.0; .var2 = 5.0 > 3.0;" \
    ".var3 = 2.0 <= 4.0; .var4= 5.0 >= 3.0; .var5 = 5.0 == 5.0");
  assert (knowledge.get (".var1").to_integer () == 1 && 
    knowledge.get (".var2").to_integer () == 1 &&
    knowledge.get (".var3").to_integer () == 1 &&
    knowledge.get (".var4").to_integer () == 1 &&
    knowledge.get (".var5").to_integer () == 1);
  
  ACE_DEBUG ((LM_INFO, "  Testing double to double comparisons\n"));

  knowledge.evaluate (".var1 = 1.0 < 10.0; .var2 = 5.0 > 3.0;" \
    ".var3 = 2.0 <= 4.0; .var4= 5.0 >= 3.0; .var5 = 5.0 == 5.0");
  assert (knowledge.get (".var1").to_integer () == 1 && 
    knowledge.get (".var2").to_integer () == 1 &&
    knowledge.get (".var3").to_integer () == 1 &&
    knowledge.get (".var4").to_integer () == 1 &&
    knowledge.get (".var5").to_integer () == 1);
  
  ACE_DEBUG ((LM_INFO, "  Testing double to int comparisons\n"));

  knowledge.evaluate (".var1 = 9.0 < 10; .var2 = 5.0 > 3.0;" \
    ".var3 = 2.0 <= 4; .var4= 5.0 >= 3; .var5 = 5.0 == 5; .var6 = 9 < 9.5;" \
    ".var7 = 3 > 2.9; .var8 = 4 <= 4.1; .var9 = 4 >= 4.0; .var10 = 5 == 5.0");
  assert (knowledge.get (".var1").to_integer () == 1 && 
    knowledge.get (".var2").to_integer () == 1 &&
    knowledge.get (".var3").to_integer () == 1 &&
    knowledge.get (".var4").to_integer () == 1 &&
    knowledge.get (".var5").to_integer () == 1 &&
    knowledge.get (".var6").to_integer () == 1 && 
    knowledge.get (".var7").to_integer () == 1 &&
    knowledge.get (".var8").to_integer () == 1 &&
    knowledge.get (".var9").to_integer () == 1 &&
    knowledge.get (".var10").to_integer () == 1);
  
  ACE_DEBUG ((LM_INFO, "  Testing string to int/double comparisons\n"));

  knowledge.evaluate (".var1 = '9.0' < 10; .var2 = '5.0' > 3.0;" \
    ".var3 = '2.0' <= 4; .var4= '5.0' >= 3; .var5 = '5.0' == 5; .var6 = '9' < 9.5;" \
    ".var7 = '3' > 2.9; .var8 = '4' <= 4.1; .var9 = '4' >= 4.0; .var10 = '5' == 5.0");
  assert (knowledge.get (".var1").to_integer () == 1 && 
    knowledge.get (".var2").to_integer () == 1 &&
    knowledge.get (".var3").to_integer () == 1 &&
    knowledge.get (".var4").to_integer () == 1 &&
    knowledge.get (".var5").to_integer () == 1 &&
    knowledge.get (".var6").to_integer () == 1 && 
    knowledge.get (".var7").to_integer () == 1 &&
    knowledge.get (".var8").to_integer () == 1 &&
    knowledge.get (".var9").to_integer () == 1 &&
    knowledge.get (".var10").to_integer () == 1);
  
  ACE_DEBUG ((LM_INFO, "  Testing int/double to string comparisons\n"));

  knowledge.evaluate (".var1 = 10 < '10.5'; .var2 = 5.5 > '5.4';" \
    ".var3 = 2 <= '2.2'; .var4= 5 >= '4.9'; .var5 = 5 == '5.0'; .var6 = 9 < '9.5';" \
    ".var7 = 3 > '2.9'; .var8 = 4 <= '4.1'; .var9 = 4 >= '4.0'; .var10 = 5 == '5'");
  assert (knowledge.get (".var1").to_integer () == 1 && 
    knowledge.get (".var2").to_integer () == 1 &&
    knowledge.get (".var3").to_integer () == 1 &&
    knowledge.get (".var4").to_integer () == 1 &&
    knowledge.get (".var5").to_integer () == 1 &&
    knowledge.get (".var6").to_integer () == 1 && 
    knowledge.get (".var7").to_integer () == 1 &&
    knowledge.get (".var8").to_integer () == 1 &&
    knowledge.get (".var9").to_integer () == 1 &&
    knowledge.get (".var10").to_integer () == 1);


}

void test_doubles (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_doubles"));
  
  ACE_DEBUG ((LM_INFO, "Testing operations on doubles\n"));

  knowledge.clear ();

  knowledge.set (".var1", 0.5);
  knowledge.set (".var2", 1.0);
  knowledge.set (".var3", 10.0);

  knowledge.evaluate (".var4 = .var2 / .var1; .var5 = .var3 / .var1;" \
    ".var6 = .var3 / (.var1 + .var2)");
  assert (knowledge.get (".var4") == 2.0 && 
    knowledge.get (".var5") == 20.0 &&
    knowledge.get (".var6") == 10.0 / 1.5);
}

void test_strings (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_strings"));
  
  ACE_DEBUG ((LM_INFO, "Testing operations on strings\n"));

  knowledge.clear ();

  knowledge.set (".var1", "0.5");
  knowledge.set (".var2", "1.0");
  knowledge.set (".var3", "10.0");

  assert (knowledge.get (".var1") == "0.5");
  assert (knowledge.get (".var2") == "1.0");
  assert (knowledge.get (".var3") == "10.0");

  knowledge.evaluate (".var4 = \"bob jenkins\"; .var5 = 'joey smith';" \
    ".var6 = 'edward sullinger'");
  assert (knowledge.get (".var4") == "bob jenkins" && 
    knowledge.get (".var5") == "joey smith" &&
    knowledge.get (".var6") == "edward sullinger");
  
  ACE_DEBUG ((LM_INFO, "Testing string concatenation\n"));

  knowledge.evaluate (".var7 = .var4 + ' ' + .var5 + ' ' + .var6");
  knowledge.print ("  {.var7}\n");

  assert (knowledge.get (".var7") == "bob jenkins joey smith edward sullinger");
  
  knowledge.set (".var1", 0.5);
  knowledge.set (".var2", (Madara::Knowledge_Record::Integer)1);
  knowledge.set (".var3", 10.5);
  
  ACE_DEBUG ((LM_INFO, "Testing string, double, and integer concatenation\n"));

  knowledge.evaluate (".var7 = .var4 + .var1 + .var2 + .var3");
  knowledge.print ("  {.var7}\n");

  assert (knowledge.get (".var7") == "bob jenkins0.5110.5");
  
  knowledge.evaluate (".var7 = .var4 + ' ' + .var1 + ' ' + .var2 + ' ' + .var3");
  knowledge.print ("  {.var7}\n");

  assert (knowledge.get (".var7") == "bob jenkins 0.5 1 10.5");

}

/// Tests logicals operators (&&, ||)
void test_logicals (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_assignments"));
  
  ACE_DEBUG ((LM_INFO, "Testing logical operations\n"));

  knowledge.clear ();

  knowledge.evaluate (".var1 = .var2 = 0; .var3 = 1; .var4 = 0");
  knowledge.evaluate (
    "(.var1 => (.var2 || .var2)) ||"
    "(.var2 => (.var1 || .var1)) ||"
    "(.var3 => (.var4 = 1))");
  assert (knowledge.get (".var4").to_integer () == 1);

  // test variables in conditionals
  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = .var1 && .var2");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = .var1 || .var2");
  assert (knowledge.get (".var3").to_integer () == 1);


  // test literals in conditionals
  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = 1 && 0");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = 1 || 0");
  assert (knowledge.get (".var3").to_integer () == 1);


  // test assignments in conditionals
  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = (.var1 = 1) && (.var2 = 0)");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = (.var1 = 1) || (.var2 = 0)");
  assert (knowledge.get (".var3").to_integer () == 1);
  
  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = (.var1 = 1 && 0) || (.var2 = 0)");
  assert (knowledge.get (".var3").to_integer () == 0);
  
  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = (.var1 = 1 && 0) || (.var2 = 1 || 0)");
  assert (knowledge.get (".var3").to_integer () == 1);


  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = (++.var1) || (++.var2)");
  assert (knowledge.get (".var3").to_integer () == 1);

  knowledge.evaluate (".var1 = 1; .var2 = 0; .var3 = (++.var1) && (++.var2)");
  assert (knowledge.get (".var3").to_integer () == 1);

  knowledge.evaluate (".var1 = 1; .var2 = -1; .var3 = (++.var1) && (++.var2)");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var1 = 5; .var2 = !.var1");
  assert (knowledge.get (".var2").to_integer () == 0);

  knowledge.evaluate (".var1 = 5; .var2 = !!.var1");
  assert (knowledge.get (".var2").to_integer () == 1);
  
  knowledge.evaluate (".var1 = 5; .var2 = !!!.var1");
  assert (knowledge.get (".var2").to_integer () == 0);

  knowledge.evaluate (".var1 = 5; .var2 = !!!!!!.var1");
  assert (knowledge.get (".var2").to_integer () == 1);

}

/// Tests Dijkstra Synchronization algorithms 
void test_dijkstra_sync (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_dijkstra_sync"));

  knowledge.clear ();

  std::string s0_logic = "(S0 + 1) % 3 == S1 => S0 = (S0 + 3 - 1) % 3;";

  std::string s1_logic = "(S1+1) % 3 == S0 => S1 = S0; (S1+1) % 3 == S2 => S1 = S2;";

  std::string s2_logic = "S1 == S0 && (S1 + 1) % 3 != S2 => S2 = (S1 + 1) % 3;";

  ACE_DEBUG ((LM_INFO, 
    "Evaluating Dijkstra 3-state Synchronizations in order: S0->S1->S2\n"));

  // set the beginning state
  knowledge.evaluate (s0_logic + s1_logic + s2_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 0 
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s0_logic + s1_logic + s2_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 1 
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s0_logic + s1_logic + s2_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 2 
                                    && knowledge.get ("S2").to_integer () == 0);

  knowledge.evaluate (s0_logic + s1_logic + s2_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 0 
                                    && knowledge.get ("S2").to_integer () == 0);

  knowledge.evaluate (s0_logic + s1_logic + s2_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 1 && knowledge.get ("S1").to_integer () == 1 
                                    && knowledge.get ("S2").to_integer () == 2);

  knowledge.evaluate (s0_logic + s1_logic + s2_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 1 && knowledge.get ("S1").to_integer () == 2 
                                    && knowledge.get ("S2").to_integer () == 2);

  knowledge.evaluate (s0_logic + s1_logic + s2_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 0 
                                    && knowledge.get ("S2").to_integer () == 1);



  ACE_DEBUG ((LM_INFO, 
    "Evaluating Dijkstra 3-state Synchronizations in order: S1->S2->S0\n"));

  knowledge.clear ();

  // set the beginning state

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 0 
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 1 
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 2 
                                    && knowledge.get ("S2").to_integer () == 0);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 1 && knowledge.get ("S1").to_integer () == 0 
                                    && knowledge.get ("S2").to_integer () == 0);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 1 && knowledge.get ("S1").to_integer () == 1 
                                    && knowledge.get ("S2").to_integer () == 2);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 2
                                    && knowledge.get ("S2").to_integer () == 2);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 0
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 1
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 2
                                    && knowledge.get ("S2").to_integer () == 0);

  knowledge.evaluate (s1_logic + s2_logic + s0_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 1 && knowledge.get ("S1").to_integer () == 0
                                    && knowledge.get ("S2").to_integer () == 0);



  /////////////////////////////////////////////////////////////////////////////

  ACE_DEBUG ((LM_INFO, 
    "Evaluating Dijkstra 3-state Synchronizations in order: S2->S0->S1\n"));

  knowledge.clear ();

  // set the beginning state

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 1
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 2
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 0
                                    && knowledge.get ("S2").to_integer () == 0);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 1 && knowledge.get ("S1").to_integer () == 1
                                    && knowledge.get ("S2").to_integer () == 0);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 1 && knowledge.get ("S1").to_integer () == 2
                                    && knowledge.get ("S2").to_integer () == 2);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 0
                                    && knowledge.get ("S2").to_integer () == 2);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 0 && knowledge.get ("S1").to_integer () == 1
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 2
                                    && knowledge.get ("S2").to_integer () == 1);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 2 && knowledge.get ("S1").to_integer () == 0
                                    && knowledge.get ("S2").to_integer () == 0);

  knowledge.evaluate (s2_logic + s0_logic + s1_logic);

  ACE_DEBUG ((LM_INFO, "  %d %d %d\n", 
    knowledge.get ("S0").to_integer (), knowledge.get ("S1").to_integer (), knowledge.get ("S2").to_integer ()));

  assert (knowledge.get ("S0").to_integer () == 1 && knowledge.get ("S1").to_integer () == 1
                                    && knowledge.get ("S2").to_integer () == 0);

  
  assert (knowledge.get (".var3").to_integer () == 0);

}

/// Tests assignment operator (=)
void test_assignments (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_assignments"));

  ACE_DEBUG ((LM_INFO, "Testing the assignment operator\n"));

  knowledge.clear ();
  // test assignment
  knowledge.evaluate (".var1 = 1; .var2 = 0");
  assert (knowledge.get (".var1").to_integer () == 1 && knowledge.get (".var2").to_integer () == 0);

  knowledge.evaluate (".var3 = .var2 = .var1 = 4");
  assert (knowledge.get (".var1").to_integer () == 4 && knowledge.get (".var2").to_integer () == 4 &&
          knowledge.get (".var3").to_integer () == 4);

  knowledge.evaluate (".var2 = 8; .var5 = .var3 = .var2 = .var1 = -1; .var2 = 0");
  assert (knowledge.get (".var1").to_integer () == -1 && knowledge.get (".var2").to_integer () == 0 &&
          knowledge.get (".var3").to_integer () == -1 && knowledge.get (".var5").to_integer () == -1);
}

/// Tests the unaries (++, --, -, !)
void test_unaries (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_unaries"));

  ACE_DEBUG ((LM_INFO, "Testing unary operators\n"));

  Madara::Knowledge_Record result;

  knowledge.clear ();

  // test increments
  knowledge.evaluate ("++.var1");
  assert (knowledge.get (".var1").to_integer () == 1);

  knowledge.evaluate ("++.var1");
  assert (knowledge.get (".var1").to_integer () == 2);

  knowledge.evaluate ("++.var1");
  assert (knowledge.get (".var1").to_integer () == 3);

  knowledge.evaluate (".var2 = 1 + (++.var1)");
  assert (knowledge.get (".var1").to_integer () == 4);
  assert (knowledge.get (".var2").to_integer () == 5);

  assert (knowledge.evaluate ("++5").to_integer () == 6);

  // the following is not allowed
  //knowledge.evaluate ("++++.var1");
  //assert (knowledge.get (".var1").to_integer () == 5);

  // test decrements
  knowledge.evaluate ("--.var4");
  assert (knowledge.get (".var4").to_integer () == -1);

  knowledge.evaluate (".var1 = 3; --.var1");
  assert (knowledge.get (".var1").to_integer () == 2);

  assert (knowledge.evaluate ("--5").to_integer () == 4);

  // the following is not allowed
  //knowledge.evaluate ("----.var1");
  //assert (knowledge.get (".var1").to_integer () == 0);

  // test logical not
  knowledge.evaluate (".var1 = !.var3");
  assert (knowledge.get (".var1").to_integer () == 1);

  knowledge.evaluate (".var2 = !.var1");
  assert (knowledge.get (".var2").to_integer () == 0);

  knowledge.evaluate (".var1 = 8; .var2 = !.var1");
  assert (knowledge.get (".var2").to_integer () == 0);

  knowledge.evaluate (".var2 = !.var2");
  assert (knowledge.get (".var2").to_integer () == 1);

  // test negation
  knowledge.evaluate (".var1 = 1; .var2 = -.var1");
  assert (knowledge.get (".var2").to_integer () == -1);

  knowledge.evaluate (".var1 = -.var2");
  assert (knowledge.get (".var1").to_integer () == 1);


}

/// Tests the conditionals (==, !=, <, <=, >, >=)
void test_conditionals (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_conditionals"));

  ACE_DEBUG ((LM_INFO, "Testing conditional operators\n"));

  knowledge.clear ();

  // test assignment
  knowledge.evaluate (".var1 = 1; .var2 = 0");
  assert (knowledge.get (".var1").to_integer () == 1 && knowledge.get (".var2").to_integer () == 0);

  // test greater thans
  knowledge.evaluate (".var3 = .var1 > .var2");
  assert (knowledge.get (".var3").to_integer () == 1);

  knowledge.evaluate (".var3 = .var1 >= .var2");
  assert (knowledge.get (".var3").to_integer () == 1);

  knowledge.evaluate (".var3 = .var2 > .var1");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var3 = .var2 >= .var1");
  assert (knowledge.get (".var3").to_integer () == 0);

  // test lesser thans

  knowledge.evaluate (".var3 = .var1 < .var2");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var3 = .var1 <= .var2");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var3 = .var2 < .var1");
  assert (knowledge.get (".var3").to_integer () == 1);

  knowledge.evaluate (".var3 = .var2 <= .var1");
  assert (knowledge.get (".var3").to_integer () == 1);

  // test equality and inequality

  knowledge.evaluate (".var3 = .var1 == .var2");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var3 = .var1 != .var2");
  assert (knowledge.get (".var3").to_integer () == 1); 
}


/// Tests the implication ops (+, -, *, /)
void test_implies (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  knowledge.clear ();

  ACE_DEBUG ((LM_INFO, "Testing implies operator\n"));

  // test implication
  knowledge.evaluate (".var1 = 1; .var2 = 0; .var1 => .var2 = 1");
  assert (knowledge.get (".var2").to_integer () == 1);

  knowledge.evaluate (".var1 = 0; .var2 = 0; .var1 => .var2 = 1");
  assert (knowledge.get (".var2").to_integer () == 0);

  knowledge.evaluate (".var1 = 0; .var2 = 0; .var3 = (!.var1 => .var2 = 1)");
  assert (knowledge.get (".var2").to_integer () == 1 && knowledge.get (".var3").to_integer () != 0);

  knowledge.evaluate (
    ".var1 = 0; .var2 = 1;" \
    ".var3 = (.var1 => .var2 = 0) || .var2");
  assert (knowledge.get (".var2").to_integer () == 1 && knowledge.get (".var3").to_integer () == 1);

}

/// Tests the math ops (+, -, *, /)
void test_mathops (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  knowledge.clear ();
  
  Madara::Knowledge_Record result;

  ACE_DEBUG ((LM_INFO, "Testing integer mathematical operators\n"));

  // 
  knowledge.evaluate (".var1 = 8; .var2 = 3");
  
  knowledge.evaluate (".var3 = .var1 + .var2");
  assert (knowledge.get (".var3").to_integer () == 11);

  knowledge.evaluate (".var3 = .var1 - .var2");
  assert (knowledge.get (".var3").to_integer () == 5);

  knowledge.evaluate (".var3 = .var1 -(- .var2)");
  assert (knowledge.get (".var3").to_integer () == 11);

  knowledge.evaluate (".var3 = .var1 * .var2");
  assert (knowledge.get (".var3").to_integer () == 24);

  knowledge.evaluate (".var3 = .var1 / .var2");
  assert (knowledge.get (".var3").to_integer () == 2);

  knowledge.evaluate (".var3 = 9 * .var1 / .var2");
  assert (knowledge.get (".var3").to_integer () == 24);

  knowledge.evaluate (".var3 = .var1 / .var2 * 9");
  assert (knowledge.get (".var3").to_integer () == 18);

  knowledge.evaluate (".var3 = .var1 / -.var2");
  assert (knowledge.get (".var3").to_integer () == -2);

  knowledge.evaluate (".var3 = -.var1 / -.var2");
  assert (knowledge.get (".var3").to_integer () == 2);

  knowledge.evaluate (".var3 = -.var1 / .var2");
  assert (knowledge.get (".var3").to_integer () == -2);

  knowledge.evaluate (".var2 = 2; .var1 = 8; .var3 = .var1 + (++.var2)");
  assert (knowledge.get (".var3").to_integer () == 11);

  knowledge.evaluate (".var2 = 2; .var1 = 8; .var3 = .var1 % .var2");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var2 = 3; .var1 = 8; .var3 = .var1 % .var2");
  assert (knowledge.get (".var3").to_integer () == 2);

  knowledge.evaluate (".var2 = 3; .var1 = 8; .var3 = (.var1 + 1 ) % .var2");
  assert (knowledge.get (".var3").to_integer () == 0);

  knowledge.evaluate (".var2 = 3; .var1 = 8; .var3 = (.var1 + 1 - 1) % .var2");
  assert (knowledge.get (".var3").to_integer () == 2);

  knowledge.evaluate (".var2 = 8; .var3 = .var2 * 3 / 8");
  assert (knowledge.get (".var3").to_integer () == 3);
  
  knowledge.evaluate (".var2 = 8; .var3 = .var2 / 8 * 3");
  assert (knowledge.get (".var3").to_integer () == 3);
  
  knowledge.evaluate (".var3 = 24 / 8 * 3");
  assert (knowledge.get (".var3").to_integer () == 9);
  
  result = knowledge.evaluate ("24/8*3");
  assert (result.to_integer () == 9);
  
  result = knowledge.evaluate ("12*24/8*3");
  assert (result.to_integer () == 108);

  knowledge.evaluate (".var3 = 12 * 24 / 8 * 3");
  assert (knowledge.get (".var3").to_integer () == 108);

  knowledge.evaluate (".var2 = 8; .var3 = .var2 * 3 / 3");
  assert (knowledge.get (".var3").to_integer () == 8);

  knowledge.evaluate (".var1 = 5; .var2 = -.var1");
  assert (knowledge.get (".var2").to_integer () == -5);

  knowledge.evaluate (".var1 = 5; .var2 = -(-.var1)");
  assert (knowledge.get (".var2").to_integer () == 5);

  knowledge.evaluate (".var1 = 5; .var2 = -(-(-.var1))");
  assert (knowledge.get (".var2").to_integer () == -5);
  
  knowledge.evaluate (".var1 = 5; .var2 = -(-(-(-.var1)))");
  assert (knowledge.get (".var2").to_integer () == 5);

  knowledge.evaluate (".var0 = .var1 = 0; ++.var{.var1}");
  assert (knowledge.get (".var0").to_integer () == 1);

}

/// Tests the both operator (;)
void test_both_operator (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_DEBUG ((LM_INFO, "Testing both operator (;)\n"));

  Madara::Knowledge_Record result;

  knowledge.clear ();
  knowledge.evaluate (";;;;;.var2 = 3;;;.var3 = 4;;;");
  assert (knowledge.get (".var2").to_integer () == 3 && knowledge.get (".var3").to_integer () == 4);

  knowledge.evaluate (";.var2 == 3 => .var4 = 1; .var4 == 1 => .var5 = 10;;; ; ;");
  assert (knowledge.get (".var4").to_integer () == 1 && knowledge.get (".var5").to_integer () == 10);

  // test ordering to make sure separator is working properly
  knowledge.evaluate (".var6 = (.var2; .var4; .var3)");
  assert (knowledge.get (".var6").to_integer () == 4);

  knowledge.evaluate (".var6 = (.var4; .var3; .var2)");
  assert (knowledge.get (".var6").to_integer () == 4);

  knowledge.evaluate (".var6 = (.var3; .var4; .var2)");
  assert (knowledge.get (".var6").to_integer () == 4);

  knowledge.evaluate (".var6 = (1; 3; 5; .var5)");
  assert (knowledge.get (".var6").to_integer () == 10);

  knowledge.evaluate (".id=1;Running1=0;Running2=0");
  result = knowledge.evaluate (
    "(Running{.id} = 0); 1 && !Running1 && !Running2");
  
  ACE_DEBUG ((LM_INFO, "Testing sequence operator (,)\n"));

  knowledge.clear ();
  knowledge.evaluate (".var1 = (1, 3, 5); .var2 = (0, 2, 4)");
  assert (knowledge.get (".var1").to_integer () == 1 &&
    knowledge.get (".var2").to_integer () == 0);
  
  ACE_DEBUG ((LM_INFO, "Testing return right operator (;>)\n"));

  knowledge.evaluate (".var1 = (1 ;> 3 ;> 5); .var2 = (0 ;> 2 ;> 4)");
  assert (knowledge.get (".var1").to_integer () == 5 &&
    knowledge.get (".var2").to_integer () == 4);
}

/// Tests the both operator (;)
void test_comments (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  knowledge.clear ();
  std::string expression;
  expression = "\
               ++.i;\
               // this should still be 1. \n";

  knowledge.evaluate (expression);
  assert (knowledge.get (".i").to_integer () == 1);

  expression = "\
               // this should still be 1\n \
               // ++.i; \
               // but this should be 2\n \
               ++.i;";

  knowledge.evaluate (expression);
  assert (knowledge.get (".i").to_integer () == 2);

  expression = "\
               /* \n \
                 we are going to take out this old code\n \
                 that added 4 extra increments\n \
                 ++.i; \n \
                 ++.i; \n \
                 ++.i; \n \
                 ++.i; \n \
               */ \n\n \
               // but we leave this one to make it 3 \n \
               ++.i;";

  knowledge.evaluate (expression);
  assert (knowledge.get (".i").to_integer () == 3);



}

/// Test the ability to compile expressions into the cache
void test_tree_compilation (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_tree_compilation"));
  knowledge.clear ();
  Madara::Knowledge_Record result;

  ACE_DEBUG ((LM_INFO, "Testing expression tree compilation and caching\n"));

  knowledge.set (".var1", (Madara::Knowledge_Record::Integer)5);

  result = knowledge.evaluate ("1 * 1 + 2 * 2 + 3 * 3 + 4 * 4 + 5 * 5 - 18");
  assert (result.to_integer () == 37);

  // 15*200 = 3000 + 16 = 3016
  result = knowledge.evaluate ("15 * 200 - 18 + 33 + (5 == 5)");
  assert (result.to_integer () == 3016);

  // 8*5 = 40 * 5 = 200 * (7) = 1400 - 8 = 1392
  result = knowledge.evaluate ("(8 * 5) * 5 * (4 + 3) - 8");
  assert (result.to_integer () == 1392);

  result = knowledge.evaluate ("(8 * .var1) * .var1 * (4 + 3) - 8");
  assert (result.to_integer () == 1392);

}


/// Test the ability to use external functions
void test_functions (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_functions"));

  knowledge.clear ();
  Madara::Knowledge_Record result;

  // test the external C functions
  ACE_DEBUG ((LM_INFO, "Testing embedded external functions\n"));

  knowledge.set (".var1", (Madara::Knowledge_Record::Integer)5);

  knowledge.define_function ("function1", return_1);
  knowledge.define_function ("function2", return_2);
  knowledge.define_function ("function3", return_3);

  result = knowledge.evaluate (".var2 = function1()");
  assert (result.to_integer () == 1);

  result = knowledge.evaluate (".var3 = function2(.var1,.var2,.var3); .var6=3");
  assert (result.to_integer () == 3 &&
    knowledge.get (".var6").to_integer () == 3);

  result = knowledge.evaluate (".var4 = function3 (); .var7 = (200, 100, 96)");
  assert (result.to_integer () == 96
    && knowledge.get (".var4").to_integer () == 3
    && knowledge.get (".var7").to_integer () == 96);

  knowledge.define_function ("function1", return_var1);
  result = knowledge.evaluate (".var5 = function1 (8)");
  assert (result.to_integer () == 5);
  
  knowledge.define_function ("function1", set_var1_to_arg1);
  result = knowledge.evaluate (".var5 = function1 (8)");
  assert (result.to_integer () == 8 &&
          knowledge.get (".var1").to_integer () == 8);
  
  knowledge.define_function ("function1", set_var1_to_arg2);
  result = knowledge.evaluate (".var5 = function1 (8,7)");
  assert (result.to_integer () == 7 && 
          knowledge.get (".var1").to_integer () == 7);
  
  knowledge.define_function ("function1", set_var1_to_arg2);
  knowledge.define_function ("function2", return_2);

  result = knowledge.evaluate (".var4 = function1 (17 / 3, 105 / 5 * 3)");
  assert (result.to_integer () == 63 && 
          knowledge.get (".var1").to_integer () == 63 &&
          knowledge.get (".var4").to_integer () == 63);
  

  result = knowledge.evaluate (".var5 = function1 ((5 + 3),(3 * 8, 14));"
    ".var2 = function2 (); .var4 = function1 (17 / 3, 105 / 5 * 3)");
  assert (result.to_integer () == 63 && 
          knowledge.get (".var1").to_integer () == 63 &&
          knowledge.get (".var2").to_integer () == 2 &&
          knowledge.get (".var4").to_integer () == 63 &&
          knowledge.get (".var5").to_integer () == 14);
  
  knowledge.print ("function results: .var2={.var2}, .var3={.var3}," \
    ".var4={.var4}, .var5={.var5}, .var6={.var6}, .var7={.var7}\n");

  // test the KaRL expression functions
  knowledge.print ("Testing embedded KaRL expression functions...\n");
  knowledge.define_function ("hello", "'hello'");
  knowledge.define_function ("world", "' world'");
  result = knowledge.evaluate ("hello_world = hello () + world ()");
  assert (result == "hello world" &&
    knowledge.get ("hello_world").to_string () == "hello world");


  
  knowledge.define_function ("function1", return_named_1);
  result = knowledge.evaluate (".var2 = function1()");
  assert (result.to_integer () == 1);

}

/// Test the ability to use for loops
void test_for_loops (Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_for_loops"));

  knowledge.clear ();
  Madara::Knowledge_Record result;

  ACE_DEBUG ((LM_INFO, "Testing embedded for loops\n"));
  
  result = knowledge.evaluate ("max = 1 ;> .i[ 0 -> 4 ) (agent{.i}.state=.i ; max = (agent{.i}.state ; max))");
  assert (result.to_integer () == 4 &&
    knowledge.get ("agent3.state").to_integer () == 3 &&
    knowledge.get ("max").to_integer () == 3);
  
  result = knowledge.evaluate (".i[ 0 -> 10 ) (agent{.i}.state=1)");
  assert (result.to_integer () == 10 && knowledge.get ("agent3.state").to_integer () == 1);
  
  result = knowledge.evaluate (".i[0->10] (agent{.i}.state=0)");
  assert (result.to_integer () == 11 && knowledge.get ("agent3.state").to_integer () == 0);
  
  result = knowledge.evaluate (".i[0-2>10] (agent{.i}.state=2)");
  assert (result.to_integer () == 6 && knowledge.get ("agent4.state").to_integer () == 2);
  
  result = knowledge.evaluate (".i[ 1 -2> 9 ] (agent{.i}.state=1)");
  assert (result.to_integer () == 5 && knowledge.get ("agent3.state").to_integer () == 1);
  
  result = knowledge.evaluate (".i[9)");
  assert (result.to_integer () == 9 && knowledge.get (".i").to_integer () == 9);

  result = knowledge.evaluate (".i[9]");
  assert (result.to_integer () == 10 && knowledge.get (".i").to_integer () == 10);

  result = knowledge.evaluate (
    "agent11.state = 15; .i[9); .j[9]; .k[0->10) (agent{.k}.state=20)");
  assert (result.to_integer () == 15 && knowledge.get (".i").to_integer () == 9 &&
    knowledge.get (".j").to_integer () == 10 && knowledge.get (".k").to_integer () == 10 &&
    knowledge.get ("agent3.state").to_integer () == 20);
}

/// Test the ability to use +=, -=, *=, /=
void test_simplification_operators (
  Madara::Knowledge_Engine::Knowledge_Base & knowledge)
{
  ACE_TRACE (ACE_TEXT ("test_simplification_operators"));

  knowledge.clear ();
  Madara::Knowledge_Record result;

  ACE_DEBUG ((LM_INFO, "Testing simplification operators (+=, -=, *=, /=)\n"));

  result = knowledge.evaluate (".i=0; .i+=5; .i+=10");
  assert (result.to_integer () == 15 && knowledge.get (".i").to_integer () == 15);

  result = knowledge.evaluate (".i=200; .i-=125; .i-=10");
  assert (result.to_integer () == 200 && knowledge.get (".i").to_integer () == 65);

  result = knowledge.evaluate (".i=5; .i*=3; .i*=10");
  assert (result.to_integer () == 150 && knowledge.get (".i").to_integer () == 150);

  result = knowledge.evaluate (".i=200; .i/=10; .i/=4");
  assert (result.to_integer () == 200 && knowledge.get (".i").to_integer () == 5);
}


int parse_args (int argc, ACE_TCHAR * argv[])
{
  // options string which defines all short args
  ACE_TCHAR options [] = ACE_TEXT ("h");

  // create an instance of the command line args
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // set up an alias for '-n' to be '--name'
  cmd_opts.long_option (ACE_TEXT ("help"), 'h', ACE_Get_Opt::ARG_REQUIRED);
 
  // temp for current switched option
  int option;
//  ACE_TCHAR * arg;

  // iterate through the options until done
  while ((option = cmd_opts ()) != EOF)
  {
    //arg = cmd_opts.opt_arg ();
    switch (option)
    {
    case ':':
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("ERROR: -%c requires an argument"), 
           cmd_opts.opt_opt ()), -2); 
    case 'h':
    default:
      ACE_DEBUG ((LM_DEBUG, "Program Summary for %s:\n\n\
      Test all operators and basic functions of the knowledge engine.\n\
      Tests fail if assertions are thrown. If no assertion messages are\n\
      reported, then the test succeeded.\n\n \
      -h (--help)      print this menu             \n\n", argv[0]));
      ACE_ERROR_RETURN ((LM_ERROR, 
        ACE_TEXT ("Returning from Help Menu\n")), -1); 
      break;
    }
  }

  return 0;
}

