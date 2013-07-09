#include "Knowledge_Update_Settings.h"

const Madara::Knowledge_Engine::Knowledge_Update_Settings
  Madara::Knowledge_Engine::DEFAULT_KNOWLEDGE_UPDATE_SETTINGS;

const Madara::Knowledge_Engine::Knowledge_Update_Settings
  Madara::Knowledge_Engine::TREAT_AS_LOCAL_UPDATE_SETTINGS (true);

const Madara::Knowledge_Engine::Knowledge_Update_Settings
  Madara::Knowledge_Engine::TREAT_AS_LOCAL_NO_SIGNALS_UPDATE_SETTINGS (true, false);

const Madara::Knowledge_Engine::Knowledge_Update_Settings
  Madara::Knowledge_Engine::NO_SIGNALS_UPDATE_SETTINGS (false, false, false);
