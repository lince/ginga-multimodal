/***
  This file is part of avahi.
 
  avahi is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.
 
  avahi is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
  Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public
  License along with avahi; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/
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
/*******************************************************************************
As seguintes modificações foram feitas em cima do código original:
- Acréscimo da função "enviar" e de sua chamada em loop. 
*******************************************************************************/
/*
 * Para compilar:
 * g++ client-browse-services.c -o main `pkg-config avahi-client --cflags --libs`
 */

// Para Avahi
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>

// Para socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
//#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// Pare leitura do arquivo xml
#include <string>
#include <fstream>
using namespace std;


static AvahiSimplePoll *simple_poll = NULL;


int enviar(char* address, uint16_t port,
		AVAHI_GCC_UNUSED AvahiProtocol protocol) {
	int sock;  
	struct hostent *host;
	struct sockaddr_in server_addr;  

	string str;
	string strTotal;
	ifstream in;
	in.open("eventoMultimodal.xml");
	getline(in, str);

	while (in) {
		strTotal += str + '\n';
		getline(in, str);
	}

	
	if (protocol == AVAHI_PROTO_INET6) {
		perror("IPV6 not supported");
		exit(1);
	}

	host = gethostbyname(address);
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

	if (connect(sock, (struct sockaddr *)&server_addr,
			sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect");
		exit(1);
	}

	int xmlSize = strlen(strTotal.c_str());
//	header = htonl(header);
	send(sock, &xmlSize, sizeof(int), 0);
	send(sock, strTotal.c_str(), xmlSize, 0); 
}


static void resolve_callback(
    AvahiServiceResolver *r,
    AVAHI_GCC_UNUSED AvahiIfIndex interface,
    AVAHI_GCC_UNUSED AvahiProtocol protocol,
    AvahiResolverEvent event,
    const char *name,
    const char *type,
    const char *domain,
    const char *host_name,
    const AvahiAddress *address,
    uint16_t port,
    AvahiStringList *txt,
    AvahiLookupResultFlags flags,
    AVAHI_GCC_UNUSED void* userdata) {

    assert(r);

    /* Called whenever a service has been resolved successfully or timed out */

    switch (event) {
        case AVAHI_RESOLVER_FAILURE:
            fprintf(stderr, "(Resolver) Failed to resolve service '%s' of type "
            		"'%s' in domain '%s': %s\n", name, type, domain,
            		avahi_strerror(avahi_client_errno(
            				avahi_service_resolver_get_client(r))));
            break;

        case AVAHI_RESOLVER_FOUND: {
            char a[AVAHI_ADDRESS_STR_MAX], *t;
            
            fprintf(stderr, "Service '%s' of type '%s' in domain '%s':\n", name,
            		type, domain);
            
            avahi_address_snprint(a, sizeof(a), address);
            t = avahi_string_list_to_string(txt);
            fprintf(stderr,
                    "\t%s:%u (%s)\n"
                    "\tTXT=%s\n"
                    "\tcookie is %u\n"
                    "\tis_local: %i\n"
                    "\tour_own: %i\n"
                    "\twide_area: %i\n"
                    "\tmulticast: %i\n"
                    "\tcached: %i\n",
                    host_name, port, a,
                    t,
                    avahi_string_list_get_service_cookie(txt),
                    !!(flags & AVAHI_LOOKUP_RESULT_LOCAL),
                    !!(flags & AVAHI_LOOKUP_RESULT_OUR_OWN),
                    !!(flags & AVAHI_LOOKUP_RESULT_WIDE_AREA),
                    !!(flags & AVAHI_LOOKUP_RESULT_MULTICAST),
                    !!(flags & AVAHI_LOOKUP_RESULT_CACHED));

            fprintf(stderr, "Aperte enter para enviar um xml\n");
            
            while (getchar()) {
    			// Usa o IP e a porta para criar um socket e enviar o xml.
    			enviar(a, port, protocol);
                fprintf(stderr, "Enviei\n");
            }
                
            avahi_free(t);
        }
    }

    avahi_service_resolver_free(r);
}

static void browse_callback(
    AvahiServiceBrowser *b,
    AvahiIfIndex interface,
    AvahiProtocol protocol,
    AvahiBrowserEvent event,
    const char *name,
    const char *type,
    const char *domain,
    AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
    void* userdata) {
    
    AvahiClient *c = (AvahiClient *)userdata;
    assert(b);

    /* Called whenever a new services becomes available on the LAN or is removed
     * from the LAN */

    switch (event) {
        case AVAHI_BROWSER_FAILURE:
            
            fprintf(stderr, "(Browser) %s\n", avahi_strerror(avahi_client_errno(
            		avahi_service_browser_get_client(b))));
            avahi_simple_poll_quit(simple_poll);
            return;

        case AVAHI_BROWSER_NEW:
            fprintf(stderr, "(Browser) NEW: service '%s' of type '%s' in "
            		"domain '%s'\n", name, type, domain);

            /* We ignore the returned resolver object. In the callback
               function we free it. If the server is terminated before
               the callback function is called the server will free
               the resolver for us. */

            if (!(avahi_service_resolver_new(c, interface, protocol, name, type,
            		domain, AVAHI_PROTO_UNSPEC, (AvahiLookupFlags)0,
            		resolve_callback, c)))
                fprintf(stderr, "Failed to resolve service '%s': %s\n", name,
                		avahi_strerror(avahi_client_errno(c)));
            
            break;

        case AVAHI_BROWSER_REMOVE:
            fprintf(stderr, "(Browser) REMOVE: service '%s' of type '%s' in "
            		"domain '%s'\n", name, type, domain);
            break;

        case AVAHI_BROWSER_ALL_FOR_NOW:
        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            fprintf(stderr, "(Browser) %s\n", event ==
            		AVAHI_BROWSER_CACHE_EXHAUSTED ? "CACHE_EXHAUSTED" :
            		"ALL_FOR_NOW");
            break;
    }
}

static void client_callback(AvahiClient *c, AvahiClientState state,
		AVAHI_GCC_UNUSED void * userdata) {
    assert(c);

    /* Called whenever the client or server state changes */

    if (state == AVAHI_CLIENT_FAILURE) {
        fprintf(stderr, "Server connection failure: %s\n", avahi_strerror(
        		avahi_client_errno(c)));
        avahi_simple_poll_quit(simple_poll);
    }
}

int main(AVAHI_GCC_UNUSED int argc, AVAHI_GCC_UNUSED char*argv[]) {
    AvahiClient *client = NULL;
    AvahiServiceBrowser *sb = NULL;
    int error;
    int ret = 1;

    /* Allocate main loop object */
    if (!(simple_poll = avahi_simple_poll_new())) {
        fprintf(stderr, "Failed to create simple poll object.\n");
        goto fail;
    }

    /* Allocate a new client */
    client = avahi_client_new(avahi_simple_poll_get(simple_poll),
    		(AvahiClientFlags)0, client_callback, NULL, &error);

    /* Check wether creating the client object succeeded */
    if (!client) {
        fprintf(stderr, "Failed to create client: %s\n", avahi_strerror(error));
        goto fail;
    }
    
    /* Create the service browser */
    if (!(sb = avahi_service_browser_new(client, AVAHI_IF_UNSPEC,
    		AVAHI_PROTO_UNSPEC, "_GingaMultimodalEvent._tcp", NULL,
    		(AvahiLookupFlags)0, browse_callback, client))) {
        fprintf(stderr, "Failed to create service browser: %s\n",
        		avahi_strerror(avahi_client_errno(client)));
        goto fail;
    }

    /* Run the main loop */
    avahi_simple_poll_loop(simple_poll);
    
    ret = 0;
    
fail:
    
    /* Cleanup things */
    if (sb)
        avahi_service_browser_free(sb);
    
    if (client)
        avahi_client_free(client);

    if (simple_poll)
        avahi_simple_poll_free(simple_poll);

    return ret;
}

