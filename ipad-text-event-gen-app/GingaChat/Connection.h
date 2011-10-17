/*
 *  connection.h
 *  HelloWorld
 *
 *  Created by Jose Augusto Costa Martins Jr on 31/07/09.
 *  Copyright 2009 Universidade de Sao Paulo. All rights reserved.
 *
 */
/* tcpclient.c */

#import <sys/socket.h>
#import <sys/types.h>
#import <netinet/in.h>
#import <netdb.h>
#import <stdio.h>
#import <string.h>
#import <stdlib.h>
#import <unistd.h>
#import <errno.h>
#import <arpa/inet.h>

@interface Connection : NSObject
{
	int connectionStatus;
	NSString* errorInfo;
}
@property(nonatomic) int connectionStatus;
@property(nonatomic, retain) NSString* errorInfo;

- (int)initWithNetService:(NSNetService*)_service;
- (int)initWithHostAddress:(NSString *)_host andPort:(uint16_t)_port;

- (int)sendSingleStringMessage:(NSString*)data toNetService:(NSNetService *)_service;
- (void)sendStringMessage:(NSString*)data;

- (int)sendSingleFileMessage:(NSData *)data toNetService:(NSNetService *)_service;
- (void)sendFileMessage:(NSData *)data;

@end