#include "Eval_Settings.h"

const Madara::Knowledge_Engine::Eval_Settings
  Madara::Knowledge_Engine::DEFAULT_EVAL_SETTINGS;

const Madara::Knowledge_Engine::Eval_Settings
  Madara::Knowledge_Engine::TREAT_AS_LOCAL_EVAL_SETTINGS (
    false, true);

const Madara::Knowledge_Engine::Eval_Settings
  Madara::Knowledge_Engine::DELAY_AND_TREAT_AS_LOCAL_EVAL_SETTINGS (
    true, true);

const Madara::Knowledge_Engine::Eval_Settings
  Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS (
    true);
