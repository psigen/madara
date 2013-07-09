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


const Madara::Knowledge_Engine::Eval_Settings
  Madara::Knowledge_Engine::TREAT_AS_LOCAL_NO_SIGNALS_EVAL_SETTINGS (
  false, true, false);

const Madara::Knowledge_Engine::Eval_Settings
  Madara::Knowledge_Engine::NO_SIGNALS_EVAL_SETTINGS (
  false, false, false);

const Madara::Knowledge_Engine::Eval_Settings
  Madara::Knowledge_Engine::DELAY_TREAT_AS_LOCAL_NO_SIGNALS_EVAL_SETTINGS (
  true, true, false);

const Madara::Knowledge_Engine::Eval_Settings
  Madara::Knowledge_Engine::DELAY_NO_SIGNALS_EVAL_SETTINGS (
  true, false, false);
