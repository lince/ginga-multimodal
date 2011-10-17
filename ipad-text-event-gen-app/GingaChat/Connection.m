//
//  Connection.m
//  HelloWorld
//
//  Created by Jose Augusto Costa Martins Jr on 09/08/09.
//  Copyright 2009 Universidade de Sao Paulo. All rights reserved.
//

#import "Connection.h"

// Private properties and methods
@interface Connection ()

- (int)setupSocket:(struct sockaddr_in *)_socket;

@end

@implementation Connection

@synthesize connectionStatus, errorInfo;

int sock, bytes_received;
char send_data[1024],recv_data[1024];

- (int)setupSocket:(struct sockaddr_in *)_socket{
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		[self setErrorInfo:@"Socket Error"];
		perror("Socket");
		close(sock);
		[self setConnectionStatus:1];
		return connectionStatus; //error
	}
	
	if (_socket->sin_family == AF_INET) {
		NSLog(@"Protocol Family = IPv4 (AF_INET)");
	} else if (_socket->sin_family == AF_INET6) {
		NSLog(@"Protocol Family = IPv6 (AF_INET6)");
	} else {
		NSLog(@"Error: Unknown Protocol Family");
	}
	
	
	if (connect(sock, 
				(struct sockaddr *)_socket, sizeof(struct sockaddr)) == -1) 
	 {
		[self setErrorInfo:@"Connect Error"];
		perror("Connect");
		close(sock);
		//exit(1);
		[self setConnectionStatus:2];
		return connectionStatus; //error
	 }
	
	[self setConnectionStatus:0];
	return connectionStatus; //everything is fine
}


-(int)initWithHostAddress:(NSString *)_host andPort:(uint16_t)_port{
	
	struct hostent *host;
	struct sockaddr_in socketAddress;
	char host_address[15];
	
	[_host getCString:host_address maxLength:15 encoding:4];
	host = gethostbyname(host_address);
	
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(_port);
	socketAddress.sin_addr = *((struct in_addr *) host->h_addr) ;
	bzero(&(socketAddress.sin_zero),8);
	return [self setupSocket:&socketAddress];
	
	
}

- (int)initWithNetService:(NSNetService*)_service{
	
	struct sockaddr_in *socketAddress = nil;
	
	NSString           *name = nil;
	NSData             *address = nil;


	
	// A service is published on ALL network addresses,
	// so -addresses may contain more than one IP
	
	for(int i = 0 ; i < [[_service addresses] count]; i++ )
	 {
		name = [_service name];
		address = [[_service addresses] objectAtIndex: i];
		// IPs are sockaddr_in structures encapsulated in NSData...
		socketAddress = (struct sockaddr_in *) [address bytes];
		NSLog(@"Connection Address: Host= %s, Port= %i", \
			  inet_ntoa(socketAddress->sin_addr), 
			  htons(socketAddress->sin_port));

	 }
	
	return [self setupSocket: socketAddress];
	
	
}

- (int)sendSingleStringMessage:(NSString*)data toNetService:(NSNetService *)_service{
	
	if ([self initWithNetService:_service]<1) {
		NSLog(@"Sending Message");
		[self sendStringMessage:data];
		NSLog(@"Message Sent");
	}
	close(sock);
	NSLog(@"socket Closed");
	return connectionStatus;
	
}

- (void)sendStringMessage:(NSString*)data {
	NSLog(@"Dados = %@", data);
	[data getCString:send_data maxLength:[data length]+1 encoding:4];
	NSLog(@"Dados2 = %s", send_data);
	int header = strlen(send_data);
	NSLog(@"Header = %i", header);
	header = htonl(header);
	send(sock,&header,sizeof(int), 0);
	send(sock,send_data,strlen(send_data), 0);
	
}

- (int)sendSingleFileMessage:(NSData *)data toNetService:(NSNetService *)_service {
	
	if ([self initWithNetService:_service]<1) {
		NSLog(@"Sending Message");
		[self sendFileMessage:data];
		NSLog(@"Message Sent");
	}
	close(sock);
	NSLog(@"socket Closed");
	return connectionStatus;
	
}

- (void)sendFileMessage:(NSData *)data {
	NSLog(@"Dados = %@", data);
	[data getBytes:send_data length:[data length]];
	NSLog(@"Dados2 = %s", send_data);
	int header = strlen(send_data);
	NSLog(@"Header = %i", header);
	header = htonl(header);
	send(sock,&header,sizeof(int), 0);
	send(sock,send_data,strlen(send_data), 0);
	
}

- (void)closeConnection{
	close(sock);
	NSLog(@"Connection Closed");
}

@end
