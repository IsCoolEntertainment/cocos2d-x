/****************************************************************************
 Copyright (c) 2013-2017 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "network/HttpAsynConnection-apple.h"

@interface HttpAsynConnection ()

@property (readwrite) NSString *statusString;

@end

@implementation HttpAsynConnection

@synthesize srcURL = srcURL;
@synthesize sslFile = sslFile;
@synthesize responseHeader = responseHeader;
@synthesize responseData = responseData;
@synthesize getDataTime = getDataTime;
@synthesize responseCode = responseCode;
@synthesize statusString = statusString;
@synthesize responseError = responseError;
@synthesize connError = connError;
@synthesize conn = conn;
@synthesize runLoop = runLoop;

- (void)dealloc
{
    [srcURL release];
    [sslFile release];
    [responseHeader release];
    [responseData release];
    [responseError release];
    [conn release];
    [runLoop release];
    [connError release];

    [super dealloc];
}

- (void) startRequest:(NSURLRequest *)request
{
#if defined( COCOS2D_DEBUG ) && ( COCOS2D_LOG_LEVEL != 0 )
    NSLog(@"Starting to load %@", srcURL);
#endif
    
    getDataTime = 0;

    self.responseError = nil;

    NSHTTPURLResponse* response = nil;
    NSError* error = nil;
    self.responseData =
        [ NSURLConnection sendSynchronousRequest:request
                               returningResponse:&response
                                           error:&error ];
    self.responseHeader = [response allHeaderFields];
    responseCode = response.statusCode;
    self.statusString =
        [ NSHTTPURLResponse localizedStringForStatusCode:responseCode ];

    if( error != nil )
    {
        self.connError = error;
        return;
    }

#if defined( COCOS2D_DEBUG ) && ( COCOS2D_LOG_LEVEL != 0 )
    NSLog(@"Received response from request to url %@", srcURL);
#endif
    
    if(responseCode == 200)
        self.statusString = @"OK";
 
    /*The individual values of the numeric status codes defined for HTTP/1.1
    | "200"  ; OK
    | "201"  ; Created
    | "202"  ; Accepted
    | "203"  ; Non-Authoritative Information
    | "204"  ; No Content
    | "205"  ; Reset Content
    | "206"  ; Partial Content
    */
    if (responseCode < 200 || responseCode >= 300)
    {// something went wrong, abort the whole thing
        self.responseError =
            [ NSError errorWithDomain:@"CCBackendDomain"
                      code:responseCode
                      userInfo:
                          @{ NSLocalizedDescriptionKey:
                             @"Bad HTTP Response Code" } ]; 
    }
}

@end

#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
