/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file ThreadListen.h
 *  ThreadListen class.
 *  Created on: Feb 25, 2014
 *      Author: Gonzalo Rodriguez Canosa
 *      email:  gonzalorodriguez@eprosima.com
 *      		grcanosa@gmail.com
 */

#ifndef THREADLISTEN_H_
#define THREADLISTEN_H_

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>


#include "eprosimartps/rtps_all.h"

#include "eprosimartps/MessageReceiver.h"




namespace eprosima {
namespace rtps {

class RTPSReader;
class RTPSWriter;
class Participant;

/**
 * Class ThreadListen, used to listen to a specific socket for RTPS messages. Each instance, when initialized, launches
 * a new thread that listen to a specific port (all possible IP addresses in this machine.). Multiple writers and readers can be associated
 * with the same ThreadListen. The MessageReceiver instance interprets where the messages need to be forwarded (which Writer or Reader, or both).
 * @ingroup MANAGEMENTMODULE
 */
class ThreadListen
{
public:
	ThreadListen();
	virtual ~ThreadListen();
	/**
	 * This functions blocks the execution until a new message is received. The threads are launched with this function.
	 */
	void listen();
	/**
	 * Method to initialize the thread.
	 */
	bool init_thread();
	//! Vector of pointers to the associated writers.
	std::vector<RTPSWriter*> m_assoc_writers;
	//! Vector of pointer to the associated readers.
	std::vector<RTPSReader*> m_assoc_readers;
	//! Pointer to the participant.
	Participant* m_participant_ptr;
	//!Vector containing the locators that are being listened with this thread. Currently the thread only listens to one Locator.
	std::vector<Locator_t> m_locList;
	//!Pointer to the thread.
	boost::thread* mp_thread;
	boost::asio::io_service m_io_service;
	//!Listen socket.
	boost::asio::ip::udp::socket m_listen_socket;
	//!MessageReceiver used in the thread.
	MessageReceiver m_MessageReceiver;
	bool m_first;
	//boost::asio::ip::udp::resolver resolver;
	Locator_t m_send_locator;
	boost::asio::ip::udp::endpoint m_sender_endpoint;
	//! Variable indicating whether the listen thread is Multicast.
	bool m_isMulticast;

};

} /* namespace rtps */
} /* namespace eprosima */

#endif /* THREADLISTEN_H_ */