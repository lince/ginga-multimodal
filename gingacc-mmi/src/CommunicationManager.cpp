/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
lince@dc.ufscar.br
mgp@icmc.usp.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
http://www.icmc.usp.br/php/laboratorio.php?origem=icmc&id_lab=3&nat=icmc
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
lince@dc.ufscar.br
mgp@icmc.usp.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
*******************************************************************************/

/**
* @file CommunicationManager.cpp
* @author Diogo de Carvalho Pedrosa
* @author José Augusto Costa Martins Júnior
* @date 27-05-10
*/

#include "../include/CommunicationManager.h"

#include "../include/EnhancedInputManager.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace core{
namespace mmi{

	CommunicationManager* CommunicationManager::_instance = NULL;

	CommunicationManager::CommunicationManager() {
		logger = LoggerUtil::getLogger(
				"br.ufscar.lince.ginga.core.mmi.CommunicationManager");
		LoggerUtil_info(logger, "Construtor");

		this->group = NULL;
		this->simplePoll = NULL;
		this->name = NULL;
	}

	CommunicationManager::~CommunicationManager() {
		LoggerUtil_info(logger, "Destrutor");

		if (_instance != NULL) {
			delete _instance;
			_instance = NULL;
		}
	}

	CommunicationManager* CommunicationManager::getInstance() {
		if (_instance == NULL) {
			_instance = new CommunicationManager();
		}
		return _instance;
	}

	void CommunicationManager::run() {
		AvahiClient *client = NULL;
		int error;
		struct timeval tv;

		/* Allocate main loop object */
		if (!(simplePoll = avahi_simple_poll_new())) {
			LoggerUtil_error(logger, "Failed to create simple poll object.\n");
		} else {
			name = avahi_strdup("Ginga Multimodal Event");

			/* Allocate a new client */
			client = avahi_client_new(avahi_simple_poll_get(simplePoll),
					(AvahiClientFlags)0, clientCallback, NULL, &error);

			/* Check whether creating the client object succeeded */
			if (!client) {
				LoggerUtil_error(logger, "Failed to create client: " <<
						avahi_strerror(error));
			} else {
				// Entra num loop para ficar aceitando conexões e recebendo
				// um xml em cada conexão estabelecida.
				startSocket();
				/* Run the main loop */
//				avahi_simple_poll_loop(simplePoll);
			}
		}

		/* Cleanup things */
		if (client) {
			avahi_client_free(client);
		}

		if (simplePoll) {
			avahi_simple_poll_free(simplePoll);
		}

		avahi_free(name);
	}

	void CommunicationManager::entryGroupCallback(AvahiEntryGroup *g,
			AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata) {

		CommunicationManager* cm = CommunicationManager::getInstance();
		cm->group = g;

		/* Called whenever the entry group state changes */

		switch (state) {
			case AVAHI_ENTRY_GROUP_ESTABLISHED: {
				/* The entry group has been established successfully */
				cout << "Service '" << cm->name << "' successfully established."
						<< endl;
				break;
			}
			case AVAHI_ENTRY_GROUP_COLLISION: {
				char *n;

				/* A service name collision with a remote service
				 * happened. Let's pick a new name */
				n = avahi_alternative_service_name(cm->name);
				avahi_free(cm->name);
				cm->name = n;

				cout << "Service name collision, renaming service to '" <<
						cm->name << "'" << endl;

				/* And recreate the services */
				cm->createServices(avahi_entry_group_get_client(g));
				break;
			}
			case AVAHI_ENTRY_GROUP_FAILURE: {
				cout << "Entry group failure: " << avahi_strerror(
						avahi_client_errno(avahi_entry_group_get_client(g)))
						<< endl;

				/* Some kind of failure happened while we were registering our
				 * services */
				avahi_simple_poll_quit(cm->simplePoll);
				break;
			}
			case AVAHI_ENTRY_GROUP_UNCOMMITED:
			case AVAHI_ENTRY_GROUP_REGISTERING: {
				break;
			}
		}
	}

	void CommunicationManager::createServices(AvahiClient *c) {
		char *n, r[128];
		int ret;

		/* If this is the first time we're called, let's create a new
		 * entry group if necessary */

		if (!this->group) {
			if (!(group = avahi_entry_group_new(c,
					CommunicationManager::entryGroupCallback, NULL))) {
				cout << "avahi_entry_group_new() failed: " <<
						avahi_strerror(avahi_client_errno(c)) << endl;
				avahi_simple_poll_quit(simplePoll);
				return;
			}
		}

		/* If the group is empty (either because it was just created, or
		 * because it was reset previously, add our entries.  */

		if (avahi_entry_group_is_empty(group)) {
			cout << "Adding service '" << name << "'" << endl;;

			/* Create some random TXT data */
			snprintf(r, sizeof(r), "random=%i", rand());

			/* We will now add two services and one subtype to the entry
			 * group. The two services have the same name, but differ in
			 * the service type (IPP vs. BSD LPR). Only services with the
			 * same name should be put in the same entry group. */

			/* Add the service for GingaMultimodalEvent*/
			if ((ret = avahi_entry_group_add_service(group, AVAHI_IF_UNSPEC,
					AVAHI_PROTO_UNSPEC, (AvahiPublishFlags)0, name,
					"_GingaMultimodalEvent._tcp", NULL, NULL, 1234, "test=blah",
					r, NULL)) < 0) {

				if (ret == AVAHI_ERR_COLLISION) {
					/* A service name collision with a local service happened.
					 * Let's pick a new name */
					n = avahi_alternative_service_name(name);
					avahi_free(name);
					name = n;

					cout << "Service name collision, renaming service to '" <<
							name << "'" << endl;

					avahi_entry_group_reset(group);
					createServices(c);
					return;
				}

				cout << "Failed to add _GingaMultimodalEvent._tcp service: " <<
						avahi_strerror(ret) << endl;
				avahi_simple_poll_quit(simplePoll);
				return;
			}

			/* Tell the server to register the service */
			if ((ret = avahi_entry_group_commit(group)) < 0) {
				cout << "Failed to commit entry group: " << avahi_strerror(ret)
						<< endl;
				avahi_simple_poll_quit(simplePoll);
			}
		}
	}

	void CommunicationManager::clientCallback(AvahiClient *c, AvahiClientState state,
			AVAHI_GCC_UNUSED void * userdata) {

		/* Called whenever the client or server state changes */

		CommunicationManager* cm = CommunicationManager::getInstance();

		switch (state) {
			case AVAHI_CLIENT_S_RUNNING: {
				/* The server has startup successfully and registered its host
				 * name on the network, so it's time to create our services */
				cm->createServices(c);
				break;
			}
			case AVAHI_CLIENT_FAILURE: {
				cout << "Client failure: " << avahi_strerror(
						avahi_client_errno(c)) << endl;
				avahi_simple_poll_quit(cm->simplePoll);
				break;
			}
			case AVAHI_CLIENT_S_COLLISION:
				/* Let's drop our registered services. When the server is back
				 * in AVAHI_SERVER_RUNNING state we will register them
				 * again with the new host name. */
			case AVAHI_CLIENT_S_REGISTERING: {
				/* The server records are now being established. This
				 * might be caused by a host name change. We need to wait
				 * for our own records to register until the host name is
				 * properly esatblished. */
				if (cm->group) {
					avahi_entry_group_reset(cm->group);
				}
				break;
			}
			case AVAHI_CLIENT_CONNECTING: {
				break;
			}
		}
	}

	int CommunicationManager::startSocket() {
		int sock;
		int connected;
		int bytes_received;
		int trueVar = 1;
		int port = 1234;
		char* recv_data;
		int bytesToBeReceived;
		struct sockaddr_in server_addr;
		struct sockaddr_in client_addr;
		int sin_size = sizeof(struct sockaddr_in);;

		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			LoggerUtil_error(logger, "Não foi possível criar um socket");
			return 1;
		}

		// int setsockopt(int socket, int level, int option_name, const void
		// *option_value, socklen_t option_len);
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &trueVar, sizeof(int))
				== -1) {
			LoggerUtil_error(logger, "Não foi possível setar opção de permitir"
					"reuso do endereço do socket");
			return 2;
		}

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = INADDR_ANY;
		bzero(&(server_addr.sin_zero),8);

		if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
				== -1) {
			LoggerUtil_error(logger, "Unable to bind");
			return 3;
		}

		if (listen(sock, 5) == -1) {
			LoggerUtil_error(logger, "Erro na chamada listen");
			return 4;
		}

		// TODO Existe alguma forma de interromper a chamada accept enquanto ela
		// estiver travada e dar um break nesse laço?
		while (true) {
			LoggerUtil_info(logger, "Waiting for client on port " << port);

			if (! (connected = accept(sock, (struct sockaddr*)&client_addr,
					(socklen_t*)&sin_size))){
				LoggerUtil_info(logger, "Unable to connect");
				return 5;
			}

			// TODO O número da porta que está sendo mostrado é diferente toda
			// vez que um evento é recebido. Pensei que seria sempre 1234. Isso
			// está certo?
			LoggerUtil_info(logger, "I got a connection from (" <<
					inet_ntoa(client_addr.sin_addr) << ", " <<
					ntohs(client_addr.sin_port) << ")");

			bytes_received = recv(connected, &bytesToBeReceived, sizeof(int), 0);

			LoggerUtil_info(logger, "bytes_received = " << bytes_received);
			LoggerUtil_info(logger, "bytesToBeReceived = " << bytesToBeReceived);
			recv_data = (char*) calloc(bytesToBeReceived, sizeof(char));
			bytes_received = recv(connected, recv_data, bytesToBeReceived, 0);

			close(connected);

			recv_data[bytes_received] = '\0';

			// Nesse caso, não é possível usar o logger porque ele faz com que o
			// terminal passe a mostrar apenas caracteres estranhos. Não entendo
			// o motivo.
	//		LoggerUtil_info(logger, "Dados recebidos = " << recv_data);
			cout << "Dados recebidos = " << recv_data << endl;

			EnhancedInputManager::getInstance()->postMultimodalEvent(recv_data);
		}

		close(sock);
		return 0;
	}

}
}
}
}
}
}
