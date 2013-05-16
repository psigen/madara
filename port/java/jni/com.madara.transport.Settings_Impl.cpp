/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.transport.Settings_Impl.h"
#include "madara/transport/Transport.h"
#include <string>

void jni_Settings_impl_RV(void** ret)
{
	*ret = new Madara::Transport::Settings();
}

void jni_Settings_impl_RP(void** ret, void* original)
{
	*ret = new Madara::Transport::Settings(*(Madara::Transport::Settings*)original);
}

void jni_Settings_set_domains_impl_PS(void* cptr, const char* domains)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->domains = std::string(domains);
}

const char* jni_Settings_get_domains_impl_P(void* cptr)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	return settings->domains.c_str();
}

void jni_Settings_set_queue_length_impl_PI(void* cptr, int queue_length)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->queue_length = queue_length;
}

int jni_Settings_get_queue_length_impl_P(void* cptr)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	return settings->queue_length;
}

void jni_Settings_set_deadline_impl_PI(void* cptr, int deadline)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->deadline = deadline;
}

int jni_Settings_get_deadline_impl_P(void* cptr)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	return settings->deadline;
}

void jni_Settings_set_type_impl_PI(void* cptr, int type)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->type = type;
}

int jni_Settings_get_type_impl_P(void* cptr)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	return settings->type;
}

void jni_Settings_set_reliability_impl_PI(void* cptr, int reliability)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->reliability = reliability;
}

int jni_Settings_get_reliability_impl_P(void* cptr)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	return settings->reliability;
}

void jni_Settings_set_id_impl_PI(void* cptr, int id)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->id = id;
}

int jni_Settings_get_id_impl_P(void* cptr)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	return settings->id;
}

void jni_Settings_set_processes_impl_PI(void* cptr, int processes)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->processes = processes;
}

int jni_Settings_get_processes_impl_P(void* cptr)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	return settings->processes;
}

void jni_Settings_set_on_data_received_logic_impl_PS(void* cptr, const char* on_data_received_logic)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->on_data_received_logic = std::string(on_data_received_logic);
}

const char* jni_Settings_get_on_data_received_logic_impl_P(void* cptr)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	return settings->on_data_received_logic.c_str();
}

void jni_Settings_set_hosts_impl_PAI(void* cptr, const char** hosts, int len)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	settings->hosts_.clear();
	int x = 0;
	for (x; x < len; x++)
	{
		settings->hosts_.push_back(std::string(hosts[x]));
	}
}

const char** jni_Settings_get_hosts_PI(void* cptr, int* len)
{
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)cptr;
	*len = settings->hosts_.size();
	
	const char* ret[*len];
	
	int x = 0;
	for (x; x < *len; x++)
	{
		ret[x] = settings->hosts_[x].c_str();
	}
	return ret;
}



