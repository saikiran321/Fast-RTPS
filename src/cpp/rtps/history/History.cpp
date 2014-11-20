/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file History.cpp
 *
 */


#include "eprosimartps/rtps/history/History.h"

#include "eprosimartps/rtps/common/CacheChange.h"


#include "eprosimartps/utils/RTPSLog.h"

#include <boost/thread/recursive_mutex.hpp>

namespace eprosima {
namespace rtps {

static const char* const CLASS_NAME = "History";

typedef std::pair<InstanceHandle_t,std::vector<CacheChange_t*>> t_pairKeyChanges;
typedef std::vector<t_pairKeyChanges> t_vectorPairKeyChanges;

History::History(const HistoryAttributes & att):
		m_att(att),
		m_isHistoryFull(false),
		mp_invalidCache(nullptr),
		m_changePool(att.initialReservedCaches,att.payloadMaxSize,att.maximumReservedCaches),
		mp_minSeqCacheChange(nullptr),
		mp_maxSeqCacheChange(nullptr),
		mp_mutex(new boost::recursive_mutex())

{
	//const char* const METHOD_NAME = "History";
	mp_invalidCache = m_changePool.reserve_Cache();
	mp_invalidCache->writerGUID = c_Guid_Unknown;
	mp_invalidCache->sequenceNumber = c_SequenceNumber_Unknown;
	mp_minSeqCacheChange = mp_invalidCache;
	mp_maxSeqCacheChange = mp_invalidCache;
	//logInfo(RTPS_HISTORY,"History created");
}

History::~History()
{
	const char* const METHOD_NAME = "~History";
	logInfo(RTPS_HISTORY,"");
}


bool History::remove_all_changes()
{
	boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
	if(!m_changes.empty())
	{
		for(std::vector<CacheChange_t*>::iterator it = m_changes.begin();it!=m_changes.end();++it)
		{
			this->remove_change(*it);
		}
		m_changes.clear();
		m_isHistoryFull = false;
		updateMaxMinSeqNum();
		return true;
	}
	return false;
}

//bool History::remove_change(CacheChange_t* ch)
//{
//	boost::lock_guard<boost::recursive_mutex> guard(*mp_mutex);
//	if(mp_Endpoint->getTopic().topicKind == WITH_KEY)
//	{
//		for(std::vector<std::pair<InstanceHandle_t,std::vector<CacheChange_t*>>>::iterator kchit = m_keyedChanges.begin();
//				kchit!=m_keyedChanges.end();++kchit)
//		{
//			if(kchit->first == ch->instanceHandle)
//			{
//				for(std::vector<CacheChange_t*>::iterator chit = kchit->second.begin();
//						chit!=kchit->second.end();++chit)
//				{
//					if((*chit)->sequenceNumber == ch->sequenceNumber)
//					{
//						kchit->second.erase(chit);
//						break;
//					}
//				}
//				break;
//			}
//		}
//	}
//	for(std::vector<CacheChange_t*>::iterator chit = m_changes.begin();
//			chit!=m_changes.end();++chit)
//	{
//		if((*chit)->sequenceNumber == ch->sequenceNumber)
//		{
//			m_changePool.release_Cache(ch);
//			m_changes.erase(chit);
//			updateMaxMinSeqNum();
//			return true;
//		}
//	}
//	return false;
//}
//
//bool History::find_Key(CacheChange_t* a_change,t_vectorPairKeyChanges::iterator* vit_out)
//{
//	const char* const METHOD_NAME = "find_Key";
//	t_vectorPairKeyChanges::iterator vit;
//	bool found = false;
//	for(vit= m_keyedChanges.begin();vit!=m_keyedChanges.end();++vit)
//	{
//		if(a_change->instanceHandle == vit->first)
//		{
//			*vit_out = vit;
//			return true;
//		}
//	}
//	if(!found)
//	{
//		if((int)m_keyedChanges.size() < m_resourceLimitsQos.max_instances)
//		{
//			t_pairKeyChanges newpair;
//			newpair.first = a_change->instanceHandle;
//			m_keyedChanges.push_back(newpair);
//			*vit_out = m_keyedChanges.end()-1;
//			return true;
//		}
//		else
//			logWarning(RTPS_HISTORY,"History has reached the maximum number of instances"<<endl;)
//	}
//	return false;
//}

bool History::get_min_change(CacheChange_t** min_change)
{
	if(mp_minSeqCacheChange->sequenceNumber != mp_invalidCache->sequenceNumber)
	{
		*min_change = mp_minSeqCacheChange;
		return true;
	}
	return false;

}
bool History::get_max_change(CacheChange_t** max_change)
{
	if(mp_maxSeqCacheChange->sequenceNumber != mp_invalidCache->sequenceNumber)
	{
		*max_change = mp_maxSeqCacheChange;
		return true;
	}
	return false;
}

}
}

#include <sstream>

namespace eprosima{
namespace rtps{

void History::print_changes_seqNum2()
{
	std::stringstream ss;
	for(std::vector<CacheChange_t*>::iterator it = m_changes.begin();
			it!=m_changes.end();++it)
	{
		ss << (*it)->sequenceNumber.to64long() << "-";
	}
	ss << endl;
	cout << ss.str();
}



} /* namespace rtps */
} /* namespace eprosima */