//
//  CKNotification.h

//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CloudKit/CKRecord.h>
#import <CloudKit/CKDatabase.h>

@class CKRecordID, CKRecordZoneID;

NS_ASSUME_NONNULL_BEGIN
API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKNotificationID : NSObject <NSCopying, NSSecureCoding>
@end

typedef NS_ENUM(NSInteger, CKNotificationType) {
    CKNotificationTypeQuery            = 1, /* Generated by CKQuerySubscriptions */
    CKNotificationTypeRecordZone       = 2, /* Generated by CKRecordZoneSubscriptions */
    CKNotificationTypeReadNotification = 3, /* Indicates a notification that a client had previously marked as read. */
    CKNotificationTypeDatabase         API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0)) = 4, /* Generated by CKDatabaseSubscriptions */
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKNotification : NSObject

- (instancetype)init NS_UNAVAILABLE;

+ (instancetype)notificationFromRemoteNotificationDictionary:(NSDictionary *)notificationDictionary;

/* When you instantiate a CKNotification from a remote notification dictionary, you will get back a concrete
 subclass defined below.  Use the type of notification to avoid -isKindOfClass: checks */
@property (nonatomic, readonly, assign) CKNotificationType notificationType;

@property (nonatomic, readonly, copy, nullable) CKNotificationID *notificationID;

@property (nonatomic, readonly, copy, nullable) NSString *containerIdentifier;

/* push notifications have a limited size.  In some cases, CloudKit servers may not be able to send you a full
 CKNotification's worth of info in one push.  In those cases, isPruned returns YES.  The order in which we'll
 drop properties is defined in each CKNotification subclass below.
 The CKNotification can be obtained in full via a CKFetchNotificationChangesOperation */
@property (nonatomic, readonly, assign) BOOL isPruned;


/* These keys are parsed out of the 'aps' payload from a remote notification dictionary.
 On tvOS, alerts, badges, sounds, and categories are not handled in push notifications. */

/* Optional alert string to display in a push notification. */
@property (nonatomic, readonly, copy, nullable) NSString *alertBody __TVOS_PROHIBITED;
/* Instead of a raw alert string, you may optionally specify a key for a localized string in your app's Localizable.strings file. */
@property (nonatomic, readonly, copy, nullable) NSString *alertLocalizationKey __TVOS_PROHIBITED;
/* A list of field names to take from the matching record that is used as substitution variables in a formatted alert string. */
@property (nonatomic, readonly, copy, nullable) NSArray<NSString *> *alertLocalizationArgs __TVOS_PROHIBITED;

/* Optional title of the alert to display in a push notification. */
@property (nonatomic, readonly, copy, nullable) NSString *title API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) __TVOS_PROHIBITED;
/* Instead of a raw title string, you may optionally specify a key for a localized string in your app's Localizable.strings file. */
@property (nonatomic, readonly, copy, nullable) NSString *titleLocalizationKey API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) __TVOS_PROHIBITED;
/* A list of field names to take from the matching record that is used as substitution variables in a formatted title string. */
@property (nonatomic, readonly, copy, nullable) NSArray<NSString *> *titleLocalizationArgs API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) __TVOS_PROHIBITED;

/* Optional subtitle of the alert to display in a push notification. */
@property (nonatomic, readonly, copy, nullable) NSString *subtitle API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) __TVOS_PROHIBITED;
/* Instead of a raw subtitle string, you may optionally specify a key for a localized string in your app's Localizable.strings file. */
@property (nonatomic, readonly, copy, nullable) NSString *subtitleLocalizationKey API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) __TVOS_PROHIBITED;
/* A list of field names to take from the matching record that is used as substitution variables in a formatted subtitle string. */
@property (nonatomic, readonly, copy, nullable) NSArray<NSString *> *subtitleLocalizationArgs API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0)) __TVOS_PROHIBITED;


/* A key for a localized string to be used as the alert action in a modal style notification. */
@property (nonatomic, readonly, copy, nullable) NSString *alertActionLocalizationKey __TVOS_PROHIBITED;
/* The name of an image in your app bundle to be used as the launch image when launching in response to the notification. */
@property (nonatomic, readonly, copy, nullable) NSString *alertLaunchImage __TVOS_PROHIBITED;

/* The number to display as the badge of the application icon */
@property (nonatomic, readonly, copy, nullable) NSNumber *badge API_AVAILABLE(tvos(10.0));
/* The name of a sound file in your app bundle to play upon receiving the notification. */
@property (nonatomic, readonly, copy, nullable) NSString *soundName __TVOS_PROHIBITED;

/* The ID of the subscription that caused this notification to fire */
@property (nonatomic, readonly, copy, nullable) NSString *subscriptionID API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0));

/* The category for user-initiated actions in the notification */
@property (nonatomic, readonly, copy, nullable) NSString *category API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0)) __TVOS_PROHIBITED;

@end

/* notificationType == CKNotificationTypeQuery
 When properties must be dropped (see isPruned), here's the order of importance.  The most important properties are first,
 they'll be the last ones to be dropped.
 - notificationID
 - badge
 - alertLocalizationKey
 - alertLocalizationArgs
 - alertBody
 - alertActionLocalizationKey
 - alertLaunchImage
 - soundName
 - content-available
 - desiredKeys
 - queryNotificationReason
 - recordID
 - containerIdentifier
 - titleLocalizationKey
 - titleLocalizationArgs
 - title
 - subtitleLocalizationKey
 - subtitleLocalizationArgs
 - subtitle
 */

typedef NS_ENUM(NSInteger, CKQueryNotificationReason) {
    CKQueryNotificationReasonRecordCreated = 1,
    CKQueryNotificationReasonRecordUpdated,
    CKQueryNotificationReasonRecordDeleted,
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKQueryNotification : CKNotification

@property (nonatomic, readonly, assign) CKQueryNotificationReason queryNotificationReason;

/* A set of key->value pairs for creates and updates.  You request the server fill out this property via the
 "desiredKeys" property of CKNotificationInfo */
@property (nonatomic, readonly, copy, nullable) NSDictionary<NSString *, id> *recordFields;

@property (nonatomic, readonly, copy, nullable) CKRecordID *recordID;

/* If YES, this record is in the public database.  Else, it's in the private database */
@property (nonatomic, readonly, assign) BOOL isPublicDatabase API_DEPRECATED("Use databaseScope instead", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0), watchos(3.0, 3.0));

@property (nonatomic, readonly, assign) CKDatabaseScope databaseScope API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

@end


/* notificationType == CKNotificationTypeRecordZone
 When properties must be dropped (see isPruned), here's the order of importance.  The most important properties are first,
 they'll be the last ones to be dropped.
 - notificationID
 - badge
 - alertLocalizationKey
 - alertLocalizationArgs
 - alertBody
 - alertActionLocalizationKey
 - alertLaunchImage
 - soundName
 - content-available
 - recordZoneID
 - containerIdentifier
 - titleLocalizationKey
 - titleLocalizationArgs
 - title
 - subtitleLocalizationKey
 - subtitleLocalizationArgs
 - subtitle
 */

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKRecordZoneNotification : CKNotification

@property (nonatomic, readonly, copy, nullable) CKRecordZoneID *recordZoneID;

@property (nonatomic, readonly, assign) CKDatabaseScope databaseScope API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

@end


/* notificationType == CKNotificationTypeDatabase
 When properties must be dropped (see isPruned), here's the order of importance.  The most important properties are first,
 they'll be the last ones to be dropped.
 - notificationID
 - badge
 - alertLocalizationKey
 - alertLocalizationArgs
 - alertBody
 - alertActionLocalizationKey
 - alertLaunchImage
 - soundName
 - content-available
 - containerIdentifier
 - titleLocalizationKey
 - titleLocalizationArgs
 - title
 - subtitleLocalizationKey
 - subtitleLocalizationArgs
 - subtitle
 */

API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKDatabaseNotification : CKNotification

@property (nonatomic, readonly, assign) CKDatabaseScope databaseScope;

@end

NS_ASSUME_NONNULL_END
