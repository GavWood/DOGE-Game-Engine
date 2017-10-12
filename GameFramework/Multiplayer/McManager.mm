// MCManager.h
#import "McManager.h"
#import "PeerToPeer.h"

//Service name must be:
// 1–15 characters long.
// contain only ASCII lowercase letters, numbers, and hyphens.

NSString *ServiceName = @"gavins-test";

@implementation MCManager

-(id)init{
    self = [super init];
    
    if (self)
    {
        _peerID = nil;
        _session = nil;
        _browser = nil;
        _advertiser = nil;
    }
    return self;
}
-(void)setup
{
    [self setupPeerAndSessionWithDisplayName];
    [self setupBrowser];
    [self setupAdvertiser];
    
    // This can be moved when the app moves to the foreground
    [self startConnecting];
}

-(void)setupPeerAndSessionWithDisplayName
{
    // Cache the current device name and setup the peer
    NSString *displayName = [UIDevice currentDevice].name;
    _peerID = [[MCPeerID alloc] initWithDisplayName:displayName];
    
    // Cache the IOS network name
    BtU8 *networkName = (BtU8*)( [displayName UTF8String] );
    
    // Set the host name on our peer to peer class
    MpPeer peer;
    strcpy( (char*)peer.m_networkName, (char*)networkName );
    MpPeerToPeer::SetHost(peer);
    
    // Setup the session
    _session = [[MCSession alloc] initWithPeer:_peerID];
    _session.delegate = self;
}

-(void)setupBrowser
{
    // Create the service browser
    _browser = [[MCNearbyServiceBrowser alloc] initWithPeer:self.peerID
                                                       serviceType:ServiceName];
    self.browser.delegate = self;
}

-(void)setupAdvertiser
{
    // Create the service advertiser
    _advertiser = [[MCNearbyServiceAdvertiser alloc] initWithPeer:self.peerID
                                                           discoveryInfo:nil
                                                             serviceType:ServiceName];
    self.advertiser.delegate = self;
}

-(void)startConnecting
{
    NSLog(@"MCManager. Start connecting");
    [self.advertiser startAdvertisingPeer];
    [self.browser startBrowsingForPeers];
}

- (NSArray *)connectedPeers
{
    return self.session.connectedPeers;
}

-(void)sendData:(NSData*)data
{
    NSError *error;
    [self.session sendData:data toPeers:self.connectedPeers withMode:MCSessionSendDataReliable error:&error];
}

-(int)getNumConnectedPeers
{
    int numPeers = (int)[self.session.connectedPeers count];
    return numPeers;
}

- (void)browser:(MCNearbyServiceBrowser *)browser foundPeer:(MCPeerID *)peerID withDiscoveryInfo:(NSDictionary *)info
{
    NSString *remotePeerName = peerID.displayName;
    NSLog(@"MCManager. Browser found %@", remotePeerName);
    
    // Make sure you don't invite yourself
    BOOL shouldInviteOneself = ([self.session.myPeerID.displayName compare:remotePeerName] == NSOrderedDescending);
    if (!shouldInviteOneself)
    {
        NSLog(@"MCManager. No to invitePeer");
    }
    else
    {
        NSLog(@"MCManager. invitePeer %@", remotePeerName);
        [browser invitePeer:peerID toSession:self.session withContext:nil timeout:30.0];
    }
}

- (void)browser:(MCNearbyServiceBrowser *)browser lostPeer:(MCPeerID *)peerID
{
    NSLog(@"MCManager. lostPeer %@", peerID.displayName);
}

- (void)advertiser:(MCNearbyServiceAdvertiser *)advertiser didReceiveInvitationFromPeer:(MCPeerID *)peerID withContext:(NSData *)context invitationHandler:(void(^)(BOOL accept, MCSession *session))invitationHandler
{
    NSLog(@"MCManager. didReceiveInvitationFromPeer %@", peerID.displayName);
    
    // Call back the invitation handler
    invitationHandler(YES, self.session);
}

- (void)advertiser:(MCNearbyServiceAdvertiser *)advertiser didNotStartAdvertisingPeer:(NSError *)error
{
    NSLog(@"MCManager. didNotStartAdvertisingForPeers: %@", error);
}

- (void)session:(MCSession *)session peer:(MCPeerID *)peerID didChangeState:(MCSessionState)state
{
    NSLog(@"MCManager. didChangeState");
    
    switch (state)
    {
        case MCSessionStateConnecting:
        {
            NSLog(@"MCManager. MCSessionStateConnecting");
            break;
        }
            
        case MCSessionStateConnected:
        {
            NSLog(@"MCManager. MCSessionStateConnected");
            
            MpEvent event;
            strcpy( event.m_peer.m_networkName, [[peerID displayName] UTF8String] );
            event.m_eventType = MpEventType_ClientConnected;
            event.m_length = 0;
            event.m_pData = 0;
            event.m_dataForDelloc = event.m_pData;
            MpPeerToPeer::AddEvent(event);
            
            // Add the peer
            BtU8 *networkName = (BtU8*)( [[peerID displayName] UTF8String] );
            MpPeer peer;
            peer.m_address = 0;
            peer.m_port = 0;
            peer.m_startTime = MpPeerToPeer::GetTimeStarted();
            strcpy( (char*)peer.m_networkName, (char*)networkName );
            MpPeerToPeer::AddPeer( peer );
            break;
        }
            
        case MCSessionStateNotConnected:
        {
            MpEvent event;
            strcpy( event.m_peer.m_networkName, [[peerID displayName] UTF8String] );
            event.m_eventType = MpEventType_ClientDisconnected;
            event.m_length = 0;
            event.m_pData = 0;
            event.m_dataForDelloc = event.m_pData;
            MpPeerToPeer::AddEvent(event);
            MpPeerToPeer::DeletePeer( event.m_peer );
            NSLog(@"MCManager. MCSessionStateNotConnected");
            break;
        }
    }
}

// Received data from remote peer.
- (void)session:(MCSession *)session didReceiveData:(NSData *)data fromPeer:(MCPeerID *)peerID
{
    MpEvent event;
    strcpy( event.m_peer.m_networkName, [[peerID displayName] UTF8String] );
    event.m_eventType = MpEventType_ReceivedData;
    event.m_length = (int)[data length];
    event.m_pData = (char*)BtMemory::Allocate(event.m_length);
    event.m_dataForDelloc = event.m_pData;
    memcpy( event.m_pData, data.bytes, event.m_length );
    MpPeerToPeer::AddEvent(event);
}

// Received a byte stream from remote peer.
- (void)    session:(MCSession *)session
   didReceiveStream:(NSInputStream *)stream
           withName:(NSString *)streamName
           fromPeer:(MCPeerID *)peerID
{
    
}

// Start receiving a resource from remote peer.
- (void)                    session:(MCSession *)session
  didStartReceivingResourceWithName:(NSString *)resourceName
                           fromPeer:(MCPeerID *)peerID
                       withProgress:(NSProgress *)progress
{
    
}

// Finished receiving a resource from remote peer and saved the content in a temporary location
- (void)                    session:(MCSession *)session
 didFinishReceivingResourceWithName:(NSString *)resourceName
                           fromPeer:(MCPeerID *)peerID
                              atURL:(nullable NSURL *)localURL
                          withError:(nullable NSError *)error;
{
    
}

// Made first contact with peer and have identity information about the
// remote peer (certificate may be nil).
- (void)        session:(MCSession *)session
  didReceiveCertificate:(nullable NSArray *)certificate
               fromPeer:(MCPeerID *)peerID
     certificateHandler:(void (^)(BOOL accept))certificateHandler;
{
    certificateHandler(YES);
}

@end
