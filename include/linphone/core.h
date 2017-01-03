/*
linphone
Copyright (C) 2000 - 2010 Simon MORLAT (simon.morlat@linphone.org)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef LINPHONECORE_H
#define LINPHONECORE_H

#include "ortp/ortp.h"
#include "ortp/payloadtype.h"
#include "mediastreamer2/mscommon.h"
#include "mediastreamer2/msvideo.h"
#include "mediastreamer2/mediastream.h"
#include "mediastreamer2/bitratecontrol.h"
#include "linphone/sipsetup.h"
#include "linphone/lpconfig.h"

#define LINPHONE_IPADDR_SIZE 64
#define LINPHONE_HOSTNAME_SIZE 128

#ifndef LINPHONE_PUBLIC
#define LINPHONE_PUBLIC MS2_PUBLIC
#endif

#ifndef LINPHONE_DEPRECATED
#define LINPHONE_DEPRECATED MS2_DEPRECATED
#endif


#ifdef __cplusplus
extern "C" {
#endif

struct _LinphoneCore;
struct _LinphoneChatRoom;
struct _LinphoneAuthInfo;
struct _SipSetupContext;
struct _LinphoneInfoMessage;

/**
 * Linphone core main object created by function linphone_core_new() .
 * @ingroup initializing
 */
typedef struct _LinphoneCore LinphoneCore;

/**
 * Safely down-cast a belle_sip_object_t into LinphoneCore
 * @ingroup initializing
 */
#define LINPHONE_CORE(object) BELLE_SIP_CAST(object, LinphoneCore)

/**
 * Disable a sip transport
 * Use with #LCSipTransports
 * @ingroup initializing
 */
#define LC_SIP_TRANSPORT_DISABLED 0
/**
 * Randomly chose a sip port for this transport
 * Use with #LCSipTransports
 * @ingroup initializing
 */
#define LC_SIP_TRANSPORT_RANDOM (-1)

/**
 * Don't create any server socket for this transport, ie don't bind on any port.
 * Use with #LCSipTransports
 * @ingroup initializing
**/
#define LC_SIP_TRANSPORT_DONTBIND (-2)

/**
 * Linphone core SIP transport ports.
 * Special values LC_SIP_TRANSPORT_RANDOM, LC_SIP_TRANSPORT_RANDOM, #define LC_SIP_TRANSPORT_DONTBIND can be used.
 * Use with #linphone_core_set_sip_transports
 * @ingroup initializing
 */
typedef struct _LinphoneSipTransports{
	/**
	 * SIP/UDP port.
	 **/
	int udp_port;
	/**
	 * SIP/TCP port
	 * */
	int tcp_port;
	/**
	 * SIP/DTLS port
	 * */
	int dtls_port;
	/**
	 * SIP/TLS port
	 * */
	int tls_port;
} LinphoneSipTransports;

/* set LCSipTransports to ensure backward compatibility */
typedef struct _LinphoneSipTransports LCSipTransports;


/**
 * Enum describing transport type for LinphoneAddress.
 * @ingroup linphone_address
**/
enum _LinphoneTransportType{
	LinphoneTransportUdp,
	LinphoneTransportTcp,
	LinphoneTransportTls,
	LinphoneTransportDtls
};
/*this enum MUST be kept in sync with the SalTransport from sal.h*/

/**
 * Typedef for transport type enum.
 * @ingroup linphone_address
**/
typedef enum _LinphoneTransportType LinphoneTransportType;

/**
 * Enum describing the stream types.
 * @ingroup initializing
**/
enum _LinphoneStreamType {
	LinphoneStreamTypeAudio,
	LinphoneStreamTypeVideo,
	LinphoneStreamTypeText,
	LinphoneStreamTypeUnknown /* WARNING: Make sure this value remains the last one in the list */
};

/**
 * Enum describing the stream types.
 * @ingroup initializing
**/
typedef enum _LinphoneStreamType LinphoneStreamType;

/**
 * Function returning a human readable value for LinphoneStreamType.
 * @ingroup initializing
 **/

LINPHONE_PUBLIC const char *linphone_stream_type_to_string(const LinphoneStreamType);

typedef struct belle_sip_dict LinphoneDictionary;

/**
 * The LinphoneCall object represents a call issued or received by the LinphoneCore
 * @ingroup call_control
**/
struct _LinphoneCall;
/**
 * The LinphoneCall object represents a call issued or received by the LinphoneCore
 * @ingroup call_control
**/
typedef struct _LinphoneCall LinphoneCall;

/**
 * Enum describing various failure reasons or contextual information for some events.
 * @see linphone_call_get_reason()
 * @see linphone_proxy_config_get_error()
 * @see linphone_error_info_get_reason()
 * @ingroup misc
**/
enum _LinphoneReason{
	LinphoneReasonNone,
	LinphoneReasonNoResponse, /**<No response received from remote*/
	LinphoneReasonForbidden, /**<Authentication failed due to bad credentials or resource forbidden*/
	LinphoneReasonDeclined, /**<The call has been declined*/
	LinphoneReasonNotFound, /**<Destination of the call was not found.*/
	LinphoneReasonNotAnswered, /**<The call was not answered in time (request timeout)*/
	LinphoneReasonBusy, /**<Phone line was busy */
	LinphoneReasonUnsupportedContent, /**<Unsupported content */
	LinphoneReasonIOError, /**<Transport error: connection failures, disconnections etc...*/
	LinphoneReasonDoNotDisturb, /**<Do not disturb reason*/
	LinphoneReasonUnauthorized, /**<Operation is unauthorized because missing credential*/
	LinphoneReasonNotAcceptable, /**<Operation is rejected due to incompatible or unsupported media parameters*/
	LinphoneReasonNoMatch, /**<Operation could not be executed by server or remote client because it didn't have any context for it*/
	LinphoneReasonMovedPermanently, /**<Resource moved permanently*/
	LinphoneReasonGone, /**<Resource no longer exists*/
	LinphoneReasonTemporarilyUnavailable, /**<Temporarily unavailable*/
	LinphoneReasonAddressIncomplete, /**<Address incomplete*/
	LinphoneReasonNotImplemented, /**<Not implemented*/
	LinphoneReasonBadGateway, /**<Bad gateway*/
	LinphoneReasonServerTimeout, /**<Server timeout*/
	LinphoneReasonUnknown /**Unknown reason*/
};

#define LinphoneReasonBadCredentials LinphoneReasonForbidden

/*for compatibility*/
#define LinphoneReasonMedia LinphoneReasonUnsupportedContent

/**
 * Enum describing failure reasons.
 * @ingroup misc
**/
typedef enum _LinphoneReason LinphoneReason;

/**
 * Converts a LinphoneReason enum to a string.
 * @ingroup misc
**/
LINPHONE_PUBLIC const char *linphone_reason_to_string(LinphoneReason err);

/**
 * Object representing full details about a signaling error or status.
 * All LinphoneErrorInfo object returned by the liblinphone API are readonly and transcients. For safety they must be used immediately
 * after obtaining them. Any other function call to the liblinphone may change their content or invalidate the pointer.
 * @ingroup misc
**/
typedef struct _LinphoneErrorInfo LinphoneErrorInfo;

/**
 * Enum describing the authentication methods
 * @ingroup network_parameters
**/
enum _LinphoneAuthMethod {
	LinphoneAuthHttpDigest, /* Digest authentication requested */
	LinphoneAuthTls, /* Client certificate requested */
};

/**
 * Typedef for authentication methods enum.
 * @ingroup network_parameters
**/
typedef enum _LinphoneAuthMethod LinphoneAuthMethod;

/**
 * Get reason code from the error info.
 * @param ei the error info.
 * @return a #LinphoneReason
 * @ingroup misc
**/
LINPHONE_PUBLIC LinphoneReason linphone_error_info_get_reason(const LinphoneErrorInfo *ei);

/**
 * Get textual phrase from the error info.
 * This is the text that is provided by the peer in the protocol (SIP).
 * @param ei the error info.
 * @return the error phrase
 * @ingroup misc
**/
LINPHONE_PUBLIC const char *linphone_error_info_get_phrase(const LinphoneErrorInfo *ei);

/**
 * Provides additional information regarding the failure.
 * With SIP protocol, the "Reason" and "Warning" headers are returned.
 * @param ei the error info.
 * @return more details about the failure.
 * @ingroup misc
**/
LINPHONE_PUBLIC const char *linphone_error_info_get_details(const LinphoneErrorInfo *ei);

/**
 * Get the status code from the low level protocol (ex a SIP status code).
 * @param ei the error info.
 * @return the status code.
 * @ingroup misc
**/
LINPHONE_PUBLIC int linphone_error_info_get_protocol_code(const LinphoneErrorInfo *ei);

/* linphone dictionary */
LINPHONE_PUBLIC	LinphoneDictionary* linphone_dictionary_new(void);
LINPHONE_PUBLIC LinphoneDictionary * linphone_dictionary_clone(const LinphoneDictionary* src);
LINPHONE_PUBLIC LinphoneDictionary * linphone_dictionary_ref(LinphoneDictionary* obj);
LINPHONE_PUBLIC void linphone_dictionary_unref(LinphoneDictionary* obj);
LINPHONE_PUBLIC void linphone_dictionary_set_int(LinphoneDictionary* obj, const char* key, int value);
LINPHONE_PUBLIC int linphone_dictionary_get_int(LinphoneDictionary* obj, const char* key, int default_value);
LINPHONE_PUBLIC void linphone_dictionary_set_string(LinphoneDictionary* obj, const char* key, const char*value);
LINPHONE_PUBLIC const char* linphone_dictionary_get_string(LinphoneDictionary* obj, const char* key, const char* default_value);
LINPHONE_PUBLIC void linphone_dictionary_set_int64(LinphoneDictionary* obj, const char* key, int64_t value);
LINPHONE_PUBLIC int64_t linphone_dictionary_get_int64(LinphoneDictionary* obj, const char* key, int64_t default_value);
LINPHONE_PUBLIC int linphone_dictionary_remove(LinphoneDictionary* obj, const char* key);
LINPHONE_PUBLIC void linphone_dictionary_clear(LinphoneDictionary* obj);
LINPHONE_PUBLIC int linphone_dictionary_haskey(const LinphoneDictionary* obj, const char* key);
LINPHONE_PUBLIC void linphone_dictionary_foreach( const LinphoneDictionary* obj, void (*apply_func)(const char*key, void* value, void* userdata), void* userdata);
/**
 * Converts a config section into a dictionary.
 * @return a #LinphoneDictionary with all the keys from a section, or NULL if the section doesn't exist
 * @ingroup misc
 */
LINPHONE_PUBLIC LinphoneDictionary* lp_config_section_to_dict( const LpConfig* lpconfig, const char* section );

/**
 * Loads a dictionary into a section of the lpconfig. If the section doesn't exist it is created.
 * Overwrites existing keys, creates non-existing keys.
 * @ingroup misc
 */
LINPHONE_PUBLIC void lp_config_load_dict_to_section( LpConfig* lpconfig, const char* section, const LinphoneDictionary* dict);


/**
 * @addtogroup media_parameters
 * @{
**/

/**
 * Object representing an RTP payload type.
 */
typedef PayloadType LinphonePayloadType;

/**
 * Get the type of payload.
 * @param[in] pt LinphonePayloadType object
 * @return The type of payload.
 */
LINPHONE_PUBLIC int linphone_payload_type_get_type(const LinphonePayloadType *pt);

/**
 * Get the normal bitrate in bits/s.
 * @param[in] pt LinphonePayloadType object
 * @return The normal bitrate in bits/s.
 */
LINPHONE_PUBLIC int linphone_payload_type_get_normal_bitrate(const LinphonePayloadType *pt);

/**
 * Get the mime type.
 * @param[in] pt LinphonePayloadType object
 * @return The mime type.
 */
LINPHONE_PUBLIC const char * linphone_payload_type_get_mime_type(const LinphonePayloadType *pt);

/**
 * Get the number of channels.
 * @param[in] pt LinphonePayloadType object
 * @return The number of channels.
 */
LINPHONE_PUBLIC int linphone_payload_type_get_channels(const LinphonePayloadType *pt);


/**
 * Enum describing RTP AVPF activation modes.
**/
enum _LinphoneAVPFMode{
	LinphoneAVPFDefault=-1, /**<Use default value defined at upper level*/
	LinphoneAVPFDisabled, /**<AVPF is disabled*/
	LinphoneAVPFEnabled /**<AVPF is enabled */
};

/**
 * Enum describing RTP AVPF activation modes.
**/
typedef enum _LinphoneAVPFMode  LinphoneAVPFMode;

/**
 * Enum describing type of media encryption types.
**/
enum _LinphoneMediaEncryption {
	LinphoneMediaEncryptionNone, /**< No media encryption is used */
	LinphoneMediaEncryptionSRTP, /**< Use SRTP media encryption */
	LinphoneMediaEncryptionZRTP, /**< Use ZRTP media encryption */
	LinphoneMediaEncryptionDTLS /**< Use DTLS media encryption */
};

/**
 * Enum describing type of media encryption types.
**/
typedef enum _LinphoneMediaEncryption LinphoneMediaEncryption;

/**
 * Convert enum member to string.
**/
LINPHONE_PUBLIC const char *linphone_media_encryption_to_string(LinphoneMediaEncryption menc);

/**
 * @}
**/


/*
 * Note for developers: this enum must be kept synchronized with the SalPrivacy enum declared in sal.h
 */
/**
 * @ingroup call_control
 * Defines privacy policy to apply as described by rfc3323
**/
typedef enum _LinphonePrivacy {
	/**
	 * Privacy services must not perform any privacy function
	 */
	LinphonePrivacyNone=0x0,
	/**
	 * Request that privacy services provide a user-level privacy
	 * function.
	 * With this mode, "from" header is hidden, usually replaced by  From: "Anonymous" <sip:anonymous@anonymous.invalid>
	 */
	LinphonePrivacyUser=0x1,
	/**
	 * Request that privacy services modify headers that cannot
	 * be set arbitrarily by the user (Contact/Via).
	 */
	LinphonePrivacyHeader=0x2,
	/**
	 *  Request that privacy services provide privacy for session
	 * media
	 */
	LinphonePrivacySession=0x4,
	/**
	 * rfc3325
	 * The presence of this privacy type in
	 * a Privacy header field indicates that the user would like the Network
	 * Asserted Identity to be kept private with respect to SIP entities
	 * outside the Trust Domain with which the user authenticated.  Note
	 * that a user requesting multiple types of privacy MUST include all of
	 * the requested privacy types in its Privacy header field value
	 *
	 */
	LinphonePrivacyId=0x8,
	/**
	 * Privacy service must perform the specified services or
	 * fail the request
	 *
	 **/
	LinphonePrivacyCritical=0x10,

	/**
	 * Special keyword to use privacy as defined either globally or by proxy using linphone_proxy_config_set_privacy()
	 */
	LinphonePrivacyDefault=0x8000,
} LinphonePrivacy;
/*
 * a mask  of #LinphonePrivacy values
 * */
typedef unsigned int LinphonePrivacyMask;


LINPHONE_PUBLIC const char* linphone_privacy_to_string(LinphonePrivacy privacy);


#include "linphone/address.h"
#include "linphone/buffer.h"
#include "linphone/call_log.h"
#include "linphone/call_params.h"
#include "linphone/content.h"
#include "linphone/event.h"
#include "linphone/linphonefriend.h"
#include "linphone/nat_policy.h"
#include "linphone/xmlrpc.h"
#include "linphone/conference.h"

/**
 * Create a #LinphoneAddress object by parsing the user supplied address, given as a string.
 * @param[in] lc #LinphoneCore object
 * @param[in] address String containing the user supplied address
 * @return The create #LinphoneAddress object
 * @ingroup linphone_address
 */
LINPHONE_PUBLIC LinphoneAddress * linphone_core_create_address(LinphoneCore *lc, const char *address);

/**
 * The LinphoneInfoMessage is an object representing an informational message sent or received by the core.
**/
typedef struct _LinphoneInfoMessage LinphoneInfoMessage;

/**
 * Creates an empty info message.
 * @param lc the LinphoneCore
 * @return a new LinphoneInfoMessage.
 *
 * The info message can later be filled with information using linphone_info_message_add_header() or linphone_info_message_set_content(),
 * and finally sent with linphone_core_send_info_message().
**/
LINPHONE_PUBLIC LinphoneInfoMessage *linphone_core_create_info_message(LinphoneCore*lc);

/**
 * Send a LinphoneInfoMessage through an established call
 * @param call the call
 * @param info the info message
**/
LINPHONE_PUBLIC int linphone_call_send_info_message(struct _LinphoneCall *call, const LinphoneInfoMessage *info);

/**
 * Add a header to an info message to be sent.
 * @param im the info message
 * @param name the header'name
 * @param value the header's value
**/
LINPHONE_PUBLIC void linphone_info_message_add_header(LinphoneInfoMessage *im, const char *name, const char *value);

/**
 * Obtain a header value from a received info message.
 * @param im the info message
 * @param name the header'name
 * @return the corresponding header's value, or NULL if not exists.
**/
LINPHONE_PUBLIC const char *linphone_info_message_get_header(const LinphoneInfoMessage *im, const char *name);

/**
 * Assign a content to the info message.
 * @param im the linphone info message
 * @param content the content described as a #LinphoneContent structure.
 * All fields of the LinphoneContent are copied, thus the application can destroy/modify/recycloe the content object freely ater the function returns.
**/
LINPHONE_PUBLIC void linphone_info_message_set_content(LinphoneInfoMessage *im,  const LinphoneContent *content);

/**
 * Returns the info message's content as a #LinphoneContent structure.
**/
LINPHONE_PUBLIC const LinphoneContent * linphone_info_message_get_content(const LinphoneInfoMessage *im);

/**
 * Destroy a LinphoneInfoMessage
**/
LINPHONE_PUBLIC void linphone_info_message_destroy(LinphoneInfoMessage *im);
LINPHONE_PUBLIC LinphoneInfoMessage *linphone_info_message_copy(const LinphoneInfoMessage *orig);



/**
 * Structure describing policy regarding video streams establishments.
 * @ingroup media_parameters
**/
struct _LinphoneVideoPolicy{
	bool_t automatically_initiate; /**<Whether video shall be automatically proposed for outgoing calls.*/
	bool_t automatically_accept; /**<Whether video shall be automatically accepted for incoming calls*/
	bool_t unused[2];
};

/**
 * Structure describing policy regarding video streams establishments.
 * @ingroup media_parameters
**/
typedef struct _LinphoneVideoPolicy LinphoneVideoPolicy;




/**
 * @addtogroup call_misc
 * @{
**/

#define LINPHONE_CALL_STATS_AUDIO ((int)LinphoneStreamTypeAudio)
#define LINPHONE_CALL_STATS_VIDEO ((int)LinphoneStreamTypeVideo)
#define LINPHONE_CALL_STATS_TEXT  ((int)LinphoneStreamTypeText)

/**
 * Enum describing ICE states.
 * @ingroup initializing
**/
enum _LinphoneIceState{
	LinphoneIceStateNotActivated, /**< ICE has not been activated for this call or stream*/
	LinphoneIceStateFailed, /**< ICE processing has failed */
	LinphoneIceStateInProgress, /**< ICE process is in progress */
	LinphoneIceStateHostConnection, /**< ICE has established a direct connection to the remote host */
	LinphoneIceStateReflexiveConnection, /**< ICE has established a connection to the remote host through one or several NATs */
	LinphoneIceStateRelayConnection /**< ICE has established a connection through a relay */
};

/**
 * Enum describing Ice states.
 * @ingroup initializing
**/
typedef enum _LinphoneIceState LinphoneIceState;

LINPHONE_PUBLIC const char *linphone_ice_state_to_string(LinphoneIceState state);

/**
 * Enum describing uPnP states.
 * @ingroup initializing
**/
enum _LinphoneUpnpState{
	LinphoneUpnpStateIdle, /**< uPnP is not activate */
	LinphoneUpnpStatePending, /**< uPnP process is in progress */
	LinphoneUpnpStateAdding,   /**< Internal use: Only used by port binding */
	LinphoneUpnpStateRemoving, /**< Internal use: Only used by port binding */
	LinphoneUpnpStateNotAvailable,  /**< uPnP is not available */
	LinphoneUpnpStateOk, /**< uPnP is enabled */
	LinphoneUpnpStateKo, /**< uPnP processing has failed */
	LinphoneUpnpStateBlacklisted, /**< IGD router is blacklisted */
};

/**
 * Enum describing uPnP states.
 * @ingroup initializing
**/
typedef enum _LinphoneUpnpState LinphoneUpnpState;


/**
 * Enum describing Ip family.
 * @ingroup initializing
**/
enum _LinphoneAddressFamily {
	LinphoneAddressFamilyInet, /* IpV4 */
	LinphoneAddressFamilyInet6, /* IpV6 */
	LinphoneAddressFamilyUnspec, /* Unknown */
};

/**
 * Enum describing Ip family.
 * @ingroup initializing
**/
typedef enum _LinphoneAddressFamily LinphoneAddressFamily;

/**
 * @}
**/

#include "linphone/call_stats.h"

/**
 * Access last known statistics for audio stream, for a given call.
**/
LINPHONE_PUBLIC const LinphoneCallStats *linphone_call_get_audio_stats(LinphoneCall *call);

/**
 * Access last known statistics for video stream, for a given call.
**/
LINPHONE_PUBLIC const LinphoneCallStats *linphone_call_get_video_stats(LinphoneCall *call);

/**
 * Access last known statistics for video stream, for a given call.
**/
LINPHONE_PUBLIC const LinphoneCallStats *linphone_call_get_text_stats(LinphoneCall *call);

/**
 * Callback prototype
 */
typedef void (*LinphoneCallCbFunc)(LinphoneCall *call,void * user_data);

/**
 * @addtogroup call_control
 * @{
 */

/**
 * Player interface.
**/
typedef struct _LinphonePlayer LinphonePlayer;

/**
 * Callback for notifying end of play (file).
 * @param obj the LinphonePlayer
 * @param user_data the user_data provided when calling linphone_player_open().
**/
typedef void (*LinphonePlayerEofCallback)(struct _LinphonePlayer *obj, void *user_data);

/**
 * Open a new source on this player.
 * @param obj the player
 * @param filename file to open.
 * @param cb a callback used to notify end of play.
 * @param user_data a user-data provided in the callback to help the application to retrieve its context.
 * @return 0 if successful, -1 otherwise
**/
LINPHONE_PUBLIC int linphone_player_open(LinphonePlayer *obj, const char *filename, LinphonePlayerEofCallback, void *user_data);

/**
 * Start a play operation. The player must have been open previously with linphone_player_open().
 * @param obj the player.
 * @return 0 if successful, -1 otherwise
**/
LINPHONE_PUBLIC int linphone_player_start(LinphonePlayer *obj);

/**
 * Suspend a play operation. The player must have been started previously with linphone_player_start().
 * @param obj the player.
 * @return 0 if successful, -1 otherwise
**/
LINPHONE_PUBLIC int linphone_player_pause(LinphonePlayer *obj);

/**
 * Seek at a given position given in milliseconds. The player must be in the paused state.
 * @param obj the player.
 * @param time_ms the position to seek to.
 * @return 0 if successful, -1 otherwise
**/
LINPHONE_PUBLIC int linphone_player_seek(LinphonePlayer *obj, int time_ms);

/**
 * Get the state of play operation.
 * @param obj the player.
 * @return the state of the player within MSPlayerClosed, MSPlayerStarted, MSPlayerPaused.
**/
LINPHONE_PUBLIC MSPlayerState linphone_player_get_state(LinphonePlayer *obj);

/**
 * Get the duration of the media
 * @param obj the player
 * @return  The duration in milliseconds
 */
LINPHONE_PUBLIC int linphone_player_get_duration(LinphonePlayer *obj);

/**
 * Get the position of the playback
 * @param obj the player
 * @return Position of the playback in milliseconds
 */
LINPHONE_PUBLIC int linphone_player_get_current_position(LinphonePlayer *obj);

/**
 * Close the player.
 * @param obj the player.
**/
LINPHONE_PUBLIC void linphone_player_close(LinphonePlayer *obj);

/**
 * @brief Destroy a player
 * @param obj The player
 */
LINPHONE_PUBLIC void linphone_player_destroy(LinphonePlayer *obj);

/**
 * @}
 */

/**
 * Create an independent media file player.
 * This player support WAVE and MATROSKA formats.
 * @param lc A LinphoneCore object
 * @param snd_card Playback sound card. If NULL, the sound card set in LinphoneCore will be used
 * @param video_out Video display. If NULL, the video display set in LinphoneCore will be used
 * @param window_id Id of the drawing window. Depend of video out
 * @return A pointer on the new instance. NULL if faild.
 */
LINPHONE_PUBLIC LinphonePlayer *linphone_core_create_local_player(LinphoneCore *lc, MSSndCard *snd_card, const char *video_out, void *window_id);

/**
 * Check whether Matroksa format is supported by the player
 * @return TRUE if it is supported
 */
LINPHONE_PUBLIC bool_t linphone_local_player_matroska_supported(void);

/**
 * LinphoneCallState enum represents the different state a call can reach into.
 * The application is notified of state changes through the LinphoneCoreVTable::call_state_changed callback.
 * @ingroup call_control
**/
typedef enum _LinphoneCallState{
	LinphoneCallIdle,					/**<Initial call state */
	LinphoneCallIncomingReceived, /**<This is a new incoming call */
	LinphoneCallOutgoingInit, /**<An outgoing call is started */
	LinphoneCallOutgoingProgress, /**<An outgoing call is in progress */
	LinphoneCallOutgoingRinging, /**<An outgoing call is ringing at remote end */
	LinphoneCallOutgoingEarlyMedia, /**<An outgoing call is proposed early media */
	LinphoneCallConnected, /**<Connected, the call is answered */
	LinphoneCallStreamsRunning, /**<The media streams are established and running*/
	LinphoneCallPausing, /**<The call is pausing at the initiative of local end */
	LinphoneCallPaused, /**< The call is paused, remote end has accepted the pause */
	LinphoneCallResuming, /**<The call is being resumed by local end*/
	LinphoneCallRefered, /**<The call is being transfered to another party, resulting in a new outgoing call to follow immediately*/
	LinphoneCallError, /**<The call encountered an error*/
	LinphoneCallEnd, /**<The call ended normally*/
	LinphoneCallPausedByRemote, /**<The call is paused by remote end*/
	LinphoneCallUpdatedByRemote, /**<The call's parameters change is requested by remote end, used for example when video is added by remote */
	LinphoneCallIncomingEarlyMedia, /**<We are proposing early media to an incoming call */
	LinphoneCallUpdating, /**<A call update has been initiated by us */
	LinphoneCallReleased, /**< The call object is no more retained by the core */
	LinphoneCallEarlyUpdatedByRemote, /*<The call is updated by remote while not yet answered (early dialog SIP UPDATE received).*/
	LinphoneCallEarlyUpdating /*<We are updating the call while not yet answered (early dialog SIP UPDATE sent)*/
} LinphoneCallState;

LINPHONE_PUBLIC	const char *linphone_call_state_to_string(LinphoneCallState cs);

/**
 * Acquire a reference to the call.
 * An application that wishes to retain a pointer to call object
 * must use this function to unsure the pointer remains
 * valid. Once the application no more needs this pointer,
 * it must call linphone_call_unref().
 * @param[in] call The call.
 * @return The same call.
 * @ingroup call_control
**/
LINPHONE_PUBLIC LinphoneCall *linphone_call_ref(LinphoneCall *call);

/**
 * Release reference to the call.
 * @param[in] call The call.
 * @ingroup call_control
**/
LINPHONE_PUBLIC void linphone_call_unref(LinphoneCall *call);

/**
 * Retrieve the user pointer associated with the call.
 * @param[in] call The call.
 * @return The user pointer associated with the call.
 * @ingroup call_control
**/
LINPHONE_PUBLIC void *linphone_call_get_user_data(const LinphoneCall *call);

/**
 * Assign a user pointer to the call.
 * @param[in] call The call.
 * @param[in] ud The user pointer to associate with the call.
 * @ingroup call_control
**/
LINPHONE_PUBLIC void linphone_call_set_user_data(LinphoneCall *call, void *ud);

/**
 * Get the core that has created the specified call.
 * @param[in] call LinphoneCall object
 * @return The LinphoneCore object that has created the specified call.
 * @ingroup call_control
 */
LINPHONE_PUBLIC LinphoneCore *linphone_call_get_core(const LinphoneCall *call);

LINPHONE_PUBLIC	LinphoneCallState linphone_call_get_state(const LinphoneCall *call);

/**
 * Check whether autoanswering is enabled.k
 * @param[in] call The #LinphoneCall.
 * @return TRUE if the #LinphoneCall asked for autoanswering.ss
 */
LINPHONE_PUBLIC bool_t linphone_call_asked_to_autoanswer(LinphoneCall *call);

/**
 * Get the remote address of the current call.
 * @param[in] lc LinphoneCore object.
 * @return The remote address of the current call or NULL if there is no current call.
 * @ingroup call_control
 */
LINPHONE_PUBLIC	const LinphoneAddress * linphone_core_get_current_call_remote_address(LinphoneCore *lc);

/**
 * Returns the remote address associated to this call
**/
LINPHONE_PUBLIC	const LinphoneAddress * linphone_call_get_remote_address(const LinphoneCall *call);

/**
 * Returns the remote address associated to this call as a string.
 *
 * The result string must be freed by user using ms_free().
**/
LINPHONE_PUBLIC	char *linphone_call_get_remote_address_as_string(const LinphoneCall *call);

/**
 * Returns the diversion address associated to this call
 *
**/
LINPHONE_PUBLIC	const LinphoneAddress * linphone_call_get_diversion_address(const LinphoneCall *call);

/**
 * Returns direction of the call (incoming or outgoing).
**/
LINPHONE_PUBLIC	LinphoneCallDir linphone_call_get_dir(const LinphoneCall *call);

/**
 * Returns the call log associated to this call.
**/
LINPHONE_PUBLIC	LinphoneCallLog *linphone_call_get_call_log(const LinphoneCall *call);

/**
 * Returns the refer-to uri (if the call was transfered).
**/
LINPHONE_PUBLIC const char *linphone_call_get_refer_to(const LinphoneCall *call);

/**
 * Returns true if this calls has received a transfer that has not been
 * executed yet.
 * Pending transfers are executed when this call is being paused or closed,
 * locally or by remote endpoint.
 * If the call is already paused while receiving the transfer request, the
 * transfer immediately occurs.
**/
LINPHONE_PUBLIC bool_t linphone_call_has_transfer_pending(const LinphoneCall *call);

/**
 * Returns the transferer if this call was started automatically as a result of an incoming transfer request.
 * The call in which the transfer request was received is returned in this case.
**/
LINPHONE_PUBLIC LinphoneCall *linphone_call_get_transferer_call(const LinphoneCall *call);

/**
 * When this call has received a transfer request, returns the new call that was automatically created as a result of the transfer.
**/
LINPHONE_PUBLIC LinphoneCall *linphone_call_get_transfer_target_call(const LinphoneCall *call);

/**
 * Returns the call object this call is replacing, if any.
 * Call replacement can occur during call transfers.
 * By default, the core automatically terminates the replaced call and accept the new one.
 * This function allows the application to know whether a new incoming call is a one that replaces another one.
**/
LINPHONE_PUBLIC	LinphoneCall *linphone_call_get_replaced_call(LinphoneCall *call);

/**
 * Returns call's duration in seconds.
**/
LINPHONE_PUBLIC	int linphone_call_get_duration(const LinphoneCall *call);

/**
 * Returns current parameters associated to the call.
**/
LINPHONE_PUBLIC	const LinphoneCallParams * linphone_call_get_current_params(LinphoneCall *call);

/**
 * Returns call parameters proposed by remote.
 *
 * This is useful when receiving an incoming call, to know whether the remote party
 * supports video, encryption or whatever.
**/
LINPHONE_PUBLIC	const LinphoneCallParams * linphone_call_get_remote_params(LinphoneCall *call);

/**
 * Indicate whether camera input should be sent to remote end.
**/
LINPHONE_PUBLIC void linphone_call_enable_camera(LinphoneCall *lc, bool_t enabled);

/**
 * Returns TRUE if camera pictures are allowed to be sent to the remote party.
**/
LINPHONE_PUBLIC bool_t linphone_call_camera_enabled(const LinphoneCall *lc);

/**
 * Take a photo of currently received video and write it into a jpeg file.
 * Note that the snapshot is asynchronous, an application shall not assume that the file is created when the function returns.
 * @param call a LinphoneCall
 * @param file a path where to write the jpeg content.
 * @return 0 if successfull, -1 otherwise (typically if jpeg format is not supported).
**/
LINPHONE_PUBLIC int linphone_call_take_video_snapshot(LinphoneCall *call, const char *file);

/**
 * Take a photo of currently captured video and write it into a jpeg file.
 * Note that the snapshot is asynchronous, an application shall not assume that the file is created when the function returns.
 * @param call a LinphoneCall
 * @param file a path where to write the jpeg content.
 * @return 0 if successfull, -1 otherwise (typically if jpeg format is not supported).
**/
LINPHONE_PUBLIC int linphone_call_take_preview_snapshot(LinphoneCall *call, const char *file);

/**
 * Returns the reason for a call termination (either error or normal termination)
**/
LINPHONE_PUBLIC	LinphoneReason linphone_call_get_reason(const LinphoneCall *call);

/**
 * Returns full details about call errors or termination reasons.
**/
LINPHONE_PUBLIC const LinphoneErrorInfo *linphone_call_get_error_info(const LinphoneCall *call);

/**
 * Returns the far end's user agent description string, if available.
**/
LINPHONE_PUBLIC	const char *linphone_call_get_remote_user_agent(LinphoneCall *call);

/**
 * Returns the far end's sip contact as a string, if available.
**/
LINPHONE_PUBLIC	const char *linphone_call_get_remote_contact(LinphoneCall *call);

/**
 * Get the mesured playback volume level.
 *
 * @param call The call.
 * @return float Volume level in percentage.
 */
LINPHONE_PUBLIC	float linphone_call_get_play_volume(LinphoneCall *call);

/**
 * Get the mesured record volume level
 *
 * @param call The call.
 * @return float Volume level in percentage.
 */
LINPHONE_PUBLIC	float linphone_call_get_record_volume(LinphoneCall *call);

/**
 * Create a new chat room for messaging from a call if not already existing, else return existing one.
 * No reference is given to the caller: the chat room will be deleted when the call is ended.
 * @param call #LinphoneCall object
 * @return #LinphoneChatRoom where messaging can take place.
 */
LINPHONE_PUBLIC	struct _LinphoneChatRoom * linphone_call_get_chat_room(LinphoneCall *call);



/**
 * Get speaker volume gain.
 * If the sound backend supports it, the returned gain is equal to the gain set
 * with the system mixer.
 *
 * @param call The call.
 * @return Percenatge of the max supported volume gain. Valid values are in [ 0.0 : 1.0 ].
 * In case of failure, a negative value is returned
 */
LINPHONE_PUBLIC float linphone_call_get_speaker_volume_gain(const LinphoneCall *call);

/**
 * Set speaker volume gain.
 * If the sound backend supports it, the new gain will synchronized with the system mixer.
 *
 * @param call The call.
 * @param volume Percentage of the max supported gain. Valid values are in [ 0.0 : 1.0 ].
 */
LINPHONE_PUBLIC void linphone_call_set_speaker_volume_gain(LinphoneCall *call, float volume);

/**
 * Get microphone volume gain.
 * If the sound backend supports it, the returned gain is equal to the gain set
 * with the system mixer.
 *
 * @param call The call.
 * @return double Percenatge of the max supported volume gain. Valid values are in [ 0.0 : 1.0 ].
 * In case of failure, a negative value is returned
 */
LINPHONE_PUBLIC float linphone_call_get_microphone_volume_gain(const LinphoneCall *call);

/**
 * Set microphone volume gain.
 * If the sound backend supports it, the new gain will synchronized with the system mixer.
 *
 * @param call The call.
 * @param volume Percentage of the max supported gain. Valid values are in [ 0.0 : 1.0 ].
 */
LINPHONE_PUBLIC void linphone_call_set_microphone_volume_gain(LinphoneCall *call, float volume);

/**
 * Obtain real-time quality rating of the call
 *
 * Based on local RTP statistics and RTCP feedback, a quality rating is computed and updated
 * during all the duration of the call. This function returns its value at the time of the function call.
 * It is expected that the rating is updated at least every 5 seconds or so.
 * The rating is a floating point number comprised between 0 and 5.
 *
 * 4-5 = good quality <br>
 * 3-4 = average quality <br>
 * 2-3 = poor quality <br>
 * 1-2 = very poor quality <br>
 * 0-1 = can't be worse, mostly unusable <br>
 *
 * @return The function returns -1 if no quality measurement is available, for example if no
 * active audio stream exist. Otherwise it returns the quality rating.
**/
LINPHONE_PUBLIC	float linphone_call_get_current_quality(LinphoneCall *call);

/**
 * Returns call quality averaged over all the duration of the call.
 *
 * See linphone_call_get_current_quality() for more details about quality measurement.
**/
LINPHONE_PUBLIC	float linphone_call_get_average_quality(LinphoneCall *call);
LINPHONE_PUBLIC	const char* linphone_call_get_authentication_token(LinphoneCall *call);

/**
 * Returns whether ZRTP authentication token is verified.
 * If not, it must be verified by users as described in ZRTP procedure.
 * Once done, the application must inform of the results with linphone_call_set_authentication_token_verified().
 * @param call the LinphoneCall
 * @return TRUE if authentication token is verifed, false otherwise.
 * @ingroup call_control
**/
LINPHONE_PUBLIC	bool_t linphone_call_get_authentication_token_verified(LinphoneCall *call);

/**
 * Set the result of ZRTP short code verification by user.
 * If remote party also does the same, it will update the ZRTP cache so that user's verification will not be required for the two users.
 * @param call the LinphoneCall
 * @param verified whether the ZRTP SAS is verified.
 * @ingroup call_control
**/
LINPHONE_PUBLIC	void linphone_call_set_authentication_token_verified(LinphoneCall *call, bool_t verified);

/**
 * Request remote side to send us a Video Fast Update.
**/
LINPHONE_PUBLIC void linphone_call_send_vfu_request(LinphoneCall *call);

/**
 * @deprecated Use linphone_call_get_user_data() instead.
 */
#define linphone_call_get_user_pointer(call) linphone_call_get_user_data(call)

/**
 * @deprecated Use linphone_call_set_user_data() instead.
 */
#define linphone_call_set_user_pointer(call, ud) linphone_call_set_user_data(call, ud)

LINPHONE_PUBLIC	void linphone_call_set_next_video_frame_decoded_callback(LinphoneCall *call, LinphoneCallCbFunc cb, void* user_data);

/**
 * Returns the current transfer state, if a transfer has been initiated from this call.
 * @see linphone_core_transfer_call() , linphone_core_transfer_call_to_another()
**/
LINPHONE_PUBLIC LinphoneCallState linphone_call_get_transfer_state(LinphoneCall *call);

/**
 * Perform a zoom of the video displayed during a call.
 * @param call the call.
 * @param zoom_factor a floating point number describing the zoom factor. A value 1.0 corresponds to no zoom applied.
 * @param cx a floating point number pointing the horizontal center of the zoom to be applied. This value should be between 0.0 and 1.0.
 * @param cy a floating point number pointing the vertical center of the zoom to be applied. This value should be between 0.0 and 1.0.
 *
 * cx and cy are updated in return in case their coordinates were too excentrated for the requested zoom factor. The zoom ensures that all the screen is fullfilled with the video.
**/
LINPHONE_PUBLIC void linphone_call_zoom_video(LinphoneCall* call, float zoom_factor, float* cx, float* cy);

/**
 * Start call recording.
 * The output file where audio is recorded must be previously specified with linphone_call_params_set_record_file().
**/
LINPHONE_PUBLIC	void linphone_call_start_recording(LinphoneCall *call);

/**
 * Stop call recording.
**/
LINPHONE_PUBLIC	void linphone_call_stop_recording(LinphoneCall *call);

LINPHONE_PUBLIC LinphonePlayer * linphone_call_get_player(LinphoneCall *call);

/**
 * Indicates whether an operation is in progress at the media side.
 * It can be a bad idea to initiate signaling operations (adding video, pausing the call, removing video, changing video parameters) while
 * the media is busy in establishing the connection (typically ICE connectivity checks). It can result in failures generating loss of time
 * in future operations in the call.
 * Applications are invited to check this function after each call state change to decide whether certain operations are permitted or not.
 * @param call the call
 * @return TRUE if media is busy in establishing the connection, FALSE otherwise.
**/
LINPHONE_PUBLIC bool_t linphone_call_media_in_progress(LinphoneCall *call);

/**
 * Send the specified dtmf.
 *
 * The dtmf is automatically played to the user.
 * @param call The LinphoneCall object
 * @param dtmf The dtmf name specified as a char, such as '0', '#' etc...
 * @return 0 if successful, -1 on error.
 * @ingroup call_control
**/
LINPHONE_PUBLIC	int linphone_call_send_dtmf(LinphoneCall *call,char dtmf);

/**
 * Send a list of dtmf.
 *
 * The dtmfs are automatically sent to remote, separated by some needed customizable delay.
 * Sending is canceled if the call state changes to something not LinphoneCallStreamsRunning.
 * @param call The LinphoneCall object
 * @param dtmfs A dtmf sequence such as '123#123123'
 * @return -2 if there is already a DTMF sequence, -1 if call is not ready, 0 otherwise.
 * @ingroup call_control
**/
LINPHONE_PUBLIC	int linphone_call_send_dtmfs(LinphoneCall *call,const char *dtmfs);

/**
 * Stop current DTMF sequence sending.
 *
 * Please note that some DTMF could be already sent,
 * depending on when this function call is delayed from #linphone_call_send_dtmfs. This
 * function will be automatically called if call state change to anything but LinphoneCallStreamsRunning.
 *
 * @param call The LinphoneCall object
 * @ingroup call_control
**/
LINPHONE_PUBLIC	void linphone_call_cancel_dtmfs(LinphoneCall *call);

/**
 * Get the native window handle of the video window, casted as an unsigned long.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void * linphone_call_get_native_video_window_id(const LinphoneCall *call);

/**
 * Set the native video window id where the video is to be displayed.
 * For MacOS, Linux, Windows: if not set or 0 a window will be automatically created, unless the special id -1 is given.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_call_set_native_video_window_id(LinphoneCall *call, void * id);

/**
 * Return TRUE if this call is currently part of a conference
 * @param call #LinphoneCall
 * @return TRUE if part of a conference.
 *
 * @deprecated Use linphone_call_get_conference() instead.
 * @ingroup call_control
 */
LINPHONE_PUBLIC	LINPHONE_DEPRECATED bool_t linphone_call_is_in_conference(const LinphoneCall *call);

/**
 * Return the associated conference object
 * @param call #LinphoneCall
 * @return A pointer on #LinphoneConference or NULL if the call is not part
 * of any conference.
 * @ingroup call_control
 */
LINPHONE_PUBLIC LinphoneConference *linphone_call_get_conference(const LinphoneCall *call);
/**
 * Enables or disable echo cancellation for this call
 * @param call
 * @param val
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_call_enable_echo_cancellation(LinphoneCall *call, bool_t val) ;
/**
 * Returns TRUE if echo cancellation is enabled.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	bool_t linphone_call_echo_cancellation_enabled(LinphoneCall *lc);
/**
 * Enables or disable echo limiter for this call
 * @param call
 * @param val
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_call_enable_echo_limiter(LinphoneCall *call, bool_t val);
/**
 * Returns TRUE if echo limiter is enabled.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	bool_t linphone_call_echo_limiter_enabled(const LinphoneCall *call);

/**
 * Enum describing type of audio route.
 * @ingroup call_control
**/
enum _LinphoneAudioRoute {
	LinphoneAudioRouteEarpiece = MSAudioRouteEarpiece,
	LinphoneAudioRouteSpeaker = MSAudioRouteSpeaker
};

/**
 * Enum describing type of audio route.
 * @ingroup call_control
**/
typedef enum _LinphoneAudioRoute LinphoneAudioRoute;

/**
 * Change the playback output device (currently only used for blackberry)
 * @param call
 * @param route the wanted audio route (earpiece, speaker, ...)
 *
 * @ingroup call_control
**/
LINPHONE_PUBLIC void linphone_call_set_audio_route(LinphoneCall *call, LinphoneAudioRoute route);

/**
 * Returns the number of stream for the given call.
 * Currently there is only two (Audio, Video), but later there will be more.
 * @param call
 *
 * @return 2
**/
LINPHONE_PUBLIC int linphone_call_get_stream_count(LinphoneCall *call);

/**
 * Returns the type of stream for the given stream index.
 * @param call
 * @param stream_index
 *
 * @return the type (MSAudio, MSVideo, MSText) of the stream of given index.
**/
LINPHONE_PUBLIC MSFormatType linphone_call_get_stream_type(LinphoneCall *call, int stream_index);

/**
 * Returns the meta rtp transport for the given stream index.
 * @param call
 * @param stream_index
 *
 * @return a pointer to the meta rtp transport if it exists, NULL otherwise
**/
LINPHONE_PUBLIC RtpTransport* linphone_call_get_meta_rtp_transport(LinphoneCall *call, int stream_index);

/**
 * Returns the meta rtcp transport for the given stream index.
 * @param call
 * @param stream_index
 *
 * @return a pointer to the meta rtcp transport if it exists, NULL otherwise
**/
LINPHONE_PUBLIC RtpTransport* linphone_call_get_meta_rtcp_transport(LinphoneCall *call, int stream_index);

/*keep this in sync with mediastreamer2/msvolume.h*/

/**
 * Lowest volume measurement that can be returned by linphone_call_get_play_volume() or linphone_call_get_record_volume(), corresponding to pure silence.
 * @ingroup call_misc
**/
#define LINPHONE_VOLUME_DB_LOWEST (-120)

/**
 * @addtogroup proxies
 * @{
**/

/**
 * The LinphoneProxyConfig object represents a proxy configuration to be used
 * by the LinphoneCore object.
 * Its fields must not be used directly in favour of the accessors methods.
 * Once created and filled properly the LinphoneProxyConfig can be given to
 * LinphoneCore with linphone_core_add_proxy_config().
 * This will automatically triggers the registration, if enabled.
 *
 * The proxy configuration are persistent to restarts because they are saved
 * in the configuration file. As a consequence, after linphone_core_new() there
 * might already be a list of configured proxy that can be examined with
 * linphone_core_get_proxy_config_list().
 *
 * The default proxy (see linphone_core_set_default_proxy() ) is the one of the list
 * that is used by default for calls.
**/
typedef struct _LinphoneProxyConfig LinphoneProxyConfig;

/**
 * LinphoneRegistrationState describes proxy registration states.
**/
typedef enum _LinphoneRegistrationState{
	LinphoneRegistrationNone, /**<Initial state for registrations */
	LinphoneRegistrationProgress, /**<Registration is in progress */
	LinphoneRegistrationOk,	/**< Registration is successful */
	LinphoneRegistrationCleared, /**< Unregistration succeeded */
	LinphoneRegistrationFailed	/**<Registration failed */
}LinphoneRegistrationState;

/**
 * Human readable version of the #LinphoneRegistrationState
 * @param cs sate
 */
LINPHONE_PUBLIC	const char *linphone_registration_state_to_string(LinphoneRegistrationState cs);

/**
 * @}
 */

/**
 * @addtogroup authentication
 * @{
 */

/**
 * Object holding authentication information.
 *
 * @note The object's fields should not be accessed directly. Prefer using
 * the accessor methods.
 *
 * In most case, authentication information consists of a username and password.
 * Sometimes, a userid is required by proxy, and realm can be useful to discriminate
 * different SIP domains.
 *
 * Once created and filled, a LinphoneAuthInfo must be added to the LinphoneCore in
 * order to become known and used automatically when needed.
 * Use linphone_core_add_auth_info() for that purpose.
 *
 * The LinphoneCore object can take the initiative to request authentication information
 * when needed to the application through the auth_info_requested callback of the
 * LinphoneCoreVTable structure.
 *
 * The application can respond to this information request later using
 * linphone_core_add_auth_info(). This will unblock all pending authentication
 * transactions and retry them with authentication headers.
 *
**/
typedef struct _LinphoneAuthInfo LinphoneAuthInfo;

/**
 * Cast a #belle_sip_object_t into #LinphoneAuthInfo
 */
#define LINPHONE_AUTH_INFO(obj) BELLE_SIP_CAST(obj, LinphoneAuthInfo)

/**
 * Creates a #LinphoneAuthInfo object with supplied information.
 * The object can be created empty, that is with all arguments set to NULL.
 * Username, userid, password, realm and domain can be set later using specific methods.
 * At the end, username and passwd (or ha1) are required.
 * @param username The username that needs to be authenticated
 * @param userid The userid used for authenticating (use NULL if you don't know what it is)
 * @param passwd The password in clear text
 * @param ha1 The ha1-encrypted password if password is not given in clear text.
 * @param realm The authentication domain (which can be larger than the sip domain. Unfortunately many SIP servers don't use this parameter.
 * @param domain The SIP domain for which this authentication information is valid, if it has to be restricted for a single SIP domain.
 * @return A #LinphoneAuthInfo object. linphone_auth_info_destroy() must be used to destroy it when no longer needed. The LinphoneCore makes a copy of LinphoneAuthInfo
 * passed through linphone_core_add_auth_info().
**/
LINPHONE_PUBLIC	LinphoneAuthInfo *linphone_auth_info_new(const char *username, const char *userid,
	const char *passwd, const char *ha1,const char *realm, const char *domain);

/**
 * Instantiates a new auth info with values from source.
 * @param[in] source The #LinphoneAuthInfo object to be cloned
 * @return The newly created #LinphoneAuthInfo object.
 */
LINPHONE_PUBLIC	LinphoneAuthInfo *linphone_auth_info_clone(const LinphoneAuthInfo* source);

/**
 * Take ownership
 */
LINPHONE_PUBLIC LinphoneAuthInfo *linphone_auth_info_ref(LinphoneAuthInfo *obj);

/**
 * Release ownership
 */
LINPHONE_PUBLIC void linphone_auth_info_unref(LinphoneAuthInfo *obj);

/**
 * Sets the password.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] passwd The password.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_passwd(LinphoneAuthInfo *info, const char *passwd);

/**
 * Sets the username.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] username The username.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_username(LinphoneAuthInfo *info, const char *username);

/**
 * Sets the userid.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] userid The userid.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_userid(LinphoneAuthInfo *info, const char *userid);

/**
 * Sets the realm.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] realm The realm.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_realm(LinphoneAuthInfo *info, const char *realm);

/**
 * Sets the domain for which this authentication is valid.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] domain The domain.
 * This should not be necessary because realm is supposed to be unique and sufficient.
 * However, many SIP servers don't set realm correctly, then domain has to be used to distinguish between several SIP account bearing the same username.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_domain(LinphoneAuthInfo *info, const char *domain);

/**
 * Sets the ha1.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] ha1 The ha1.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_ha1(LinphoneAuthInfo *info, const char *ha1);

/**
 * Sets the TLS certificate.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] ha1 The TLS certificate.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_tls_cert(LinphoneAuthInfo *info, const char *tls_cert);

/**
 * Sets the TLS key.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] ha1 The TLS key.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_tls_key(LinphoneAuthInfo *info, const char *tls_key);

/**
 * Sets the TLS certificate path.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] ha1 The TLS certificate path.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_tls_cert_path(LinphoneAuthInfo *info, const char *tls_cert_path);

/**
 * Sets the TLS key path.
 * @param[in] info The #LinphoneAuthInfo object
 * @param[in] ha1 The TLS key path.
**/
LINPHONE_PUBLIC void linphone_auth_info_set_tls_key_path(LinphoneAuthInfo *info, const char *tls_key_path);

/**
 * Gets the username.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The username.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_username(const LinphoneAuthInfo *info);

/**
 * Gets the password.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The password.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_passwd(const LinphoneAuthInfo *info);

/**
 * Gets the userid.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The userid.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_userid(const LinphoneAuthInfo *info);

/**
 * Gets the realm.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The realm.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_realm(const LinphoneAuthInfo *info);

/**
 * Gets the domain.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The domain.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_domain(const LinphoneAuthInfo *info);

/**
 * Gets the ha1.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The ha1.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_ha1(const LinphoneAuthInfo *info);

/**
 * Gets the TLS certificate.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The TLS certificate.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_tls_cert(const LinphoneAuthInfo *info);

/**
 * Gets the TLS key.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The TLS key.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_tls_key(const LinphoneAuthInfo *info);

/**
 * Gets the TLS certificate path.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The TLS certificate path.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_tls_cert_path(const LinphoneAuthInfo *info);

/**
 * Gets the TLS key path.
 *
 * @param[in] info The #LinphoneAuthInfo object
 * @return The TLS key path.
 */
LINPHONE_PUBLIC const char *linphone_auth_info_get_tls_key_path(const LinphoneAuthInfo *info);

/**
 * Destroys a LinphoneAuthInfo object.
 * @note you don't need those function
 * @deprecated Use linphone_auth_info_unref() instead.
**/
LINPHONE_DEPRECATED LINPHONE_PUBLIC void linphone_auth_info_destroy(LinphoneAuthInfo *info);

LINPHONE_PUBLIC LinphoneAuthInfo * linphone_auth_info_new_from_config_file(LpConfig *config, int pos);

/**
 * @}
 */


#include "linphone/account_creator.h"
#include "linphone/friendlist.h"
#include "linphone/linphone_proxy_config.h"


/**
 * @addtogroup chatroom
 * @{
 */

/**
 * An object to handle the callbacks for the handling a LinphoneChatMessage objects.
 */
typedef struct _LinphoneChatMessageCbs LinphoneChatMessageCbs;

/**
 * A chat room message to hold content to be sent.
 * <br> Can be created by linphone_chat_room_create_message().
 */
typedef struct _LinphoneChatMessage LinphoneChatMessage;

/**
 * A chat room is the place where text messages are exchanged.
 * <br> Can be created by linphone_core_create_chat_room().
 */
typedef struct _LinphoneChatRoom LinphoneChatRoom;

/**
 * LinphoneChatMessageState is used to notify if messages have been succesfully delivered or not.
 */
typedef enum _LinphoneChatMessageState {
	LinphoneChatMessageStateIdle, /**< Initial state */
	LinphoneChatMessageStateInProgress, /**< Delivery in progress */
	LinphoneChatMessageStateDelivered, /**< Message successfully delivered and acknowledged by remote end point */
	LinphoneChatMessageStateNotDelivered, /**< Message was not delivered */
	LinphoneChatMessageStateFileTransferError, /**< Message was received(and acknowledged) but cannot get file from server */
	LinphoneChatMessageStateFileTransferDone /**< File transfer has been completed successfully. */
} LinphoneChatMessageState;

typedef enum _LinphoneLimeState {
	LinphoneLimeDisabled, /**< Lime is not used at all */
	LinphoneLimeMandatory, /**< Lime is always used */
	LinphoneLimePreferred, /**< Lime is used only if we already shared a secret with remote */
} LinphoneLimeState;

/**
 * Call back used to notify message delivery status
 * @param msg #LinphoneChatMessage object
 * @param status LinphoneChatMessageState
 * @param ud application user data
 * @deprecated
 */
typedef void (*LinphoneChatMessageStateChangedCb)(LinphoneChatMessage* msg,LinphoneChatMessageState state,void* ud);

/**
 * Call back used to notify message delivery status
 * @param msg #LinphoneChatMessage object
 * @param status LinphoneChatMessageState
 */
typedef void (*LinphoneChatMessageCbsMsgStateChangedCb)(LinphoneChatMessage* msg, LinphoneChatMessageState state);

/**
 * File transfer receive callback prototype. This function is called by the core upon an incoming File transfer is started. This function may be call several time for the same file in case of large file.
 * @param message #LinphoneChatMessage message from which the body is received.
 * @param content #LinphoneContent incoming content information
 * @param buffer #LinphoneBuffer holding the received data. Empty buffer means end of file.
 */
typedef void (*LinphoneChatMessageCbsFileTransferRecvCb)(LinphoneChatMessage *message, const LinphoneContent* content, const LinphoneBuffer *buffer);

/**
 * File transfer send callback prototype. This function is called by the core when an outgoing file transfer is started. This function is called until size is set to 0.
 * @param message #LinphoneChatMessage message from which the body is received.
 * @param content #LinphoneContent outgoing content
 * @param offset the offset in the file from where to get the data to be sent
 * @param size the number of bytes expected by the framework
 * @return A LinphoneBuffer object holding the data written by the application. An empty buffer means end of file.
 */
typedef LinphoneBuffer * (*LinphoneChatMessageCbsFileTransferSendCb)(LinphoneChatMessage *message,  const LinphoneContent* content, size_t offset, size_t size);

/**
 * File transfer progress indication callback prototype.
 * @param message #LinphoneChatMessage message from which the body is received.
 * @param content #LinphoneContent incoming content information
 * @param offset The number of bytes sent/received since the beginning of the transfer.
 * @param total The total number of bytes to be sent/received.
 */
typedef void (*LinphoneChatMessageCbsFileTransferProgressIndicationCb)(LinphoneChatMessage *message, const LinphoneContent* content, size_t offset, size_t total);

/**
 * Sets the database filename where chat messages will be stored.
 * If the file does not exist, it will be created.
 * @ingroup initializing
 * @param lc the linphone core
 * @param path filesystem path
**/
LINPHONE_PUBLIC void linphone_core_set_chat_database_path(LinphoneCore *lc, const char *path);

/**
 * Get path to the database file used for storing chat messages.
 * @param lc the linphone core
 * @return file path or NULL if not exist
 **/

LINPHONE_PUBLIC const char *linphone_core_get_chat_database_path(const LinphoneCore *lc);
/**
 * Get a chat room whose peer is the supplied address. If it does not exist yet, it will be created.
 * No reference is transfered to the application. The LinphoneCore keeps a reference on the chat room.
 * @param lc the linphone core
 * @param addr a linphone address.
 * @return #LinphoneChatRoom where messaging can take place.
**/
LINPHONE_PUBLIC LinphoneChatRoom *linphone_core_get_chat_room(LinphoneCore *lc, const LinphoneAddress *addr);
/**
 * Get a chat room for messaging from a sip uri like sip:joe@sip.linphone.org. If it does not exist yet, it will be created.
 * No reference is transfered to the application. The LinphoneCore keeps a reference on the chat room.
 * @param lc A #LinphoneCore object
 * @param to The destination address for messages.
 * @return #LinphoneChatRoom where messaging can take place.
**/
LINPHONE_PUBLIC LinphoneChatRoom *linphone_core_get_chat_room_from_uri(LinphoneCore *lc, const char *to);

/**
 * Removes a chatroom including all message history from the LinphoneCore.
 * @param lc A #LinphoneCore object
 * @param cr A #LinphoneChatRoom object
**/
LINPHONE_PUBLIC void linphone_core_delete_chat_room(LinphoneCore *lc, LinphoneChatRoom *cr);

/**
 * Inconditionnaly disable incoming chat messages.
 * @param lc A #LinphoneCore object
 * @param deny_reason the deny reason (#LinphoneReasonNone has no effect).
**/
LINPHONE_PUBLIC void linphone_core_disable_chat(LinphoneCore *lc, LinphoneReason deny_reason);
/**
 * Enable reception of incoming chat messages.
 * By default it is enabled but it can be disabled with linphone_core_disable_chat().
 * @param lc A #LinphoneCore object
**/
LINPHONE_PUBLIC void linphone_core_enable_chat(LinphoneCore *lc);
/**
 * Returns whether chat is enabled.
 * @param lc A #LinphoneCore object
**/
LINPHONE_PUBLIC bool_t linphone_core_chat_enabled(const LinphoneCore *lc);
/**
 * Destroy a LinphoneChatRoom.
 * @param cr #LinphoneChatRoom object
 * @deprecated Use linphone_chat_room_unref() instead.
 */
LINPHONE_PUBLIC LINPHONE_DEPRECATED void linphone_chat_room_destroy(LinphoneChatRoom *cr);
/**
 * Create a message attached to a dedicated chat room;
 * @param cr the chat room.
 * @param message text message, NULL if absent.
 * @return a new #LinphoneChatMessage
 */
LINPHONE_PUBLIC	LinphoneChatMessage* linphone_chat_room_create_message(LinphoneChatRoom *cr,const char* message);
/**
 * Create a message attached to a dedicated chat room;
 * @param cr the chat room.
 * @param message text message, NULL if absent.
 * @param external_body_url the URL given in external body or NULL.
 * @param state the LinphoneChatMessage.State of the message.
 * @param time the time_t at which the message has been received/sent.
 * @param is_read TRUE if the message should be flagged as read, FALSE otherwise.
 * @param is_incoming TRUE if the message has been received, FALSE otherwise.
 * @return a new #LinphoneChatMessage
 */
LINPHONE_PUBLIC	LinphoneChatMessage* linphone_chat_room_create_message_2(LinphoneChatRoom *cr, const char* message, const char* external_body_url, LinphoneChatMessageState state, time_t time, bool_t is_read, bool_t is_incoming);

/**
 * Acquire a reference to the chat room.
 * @param[in] cr The chat room.
 * @return The same chat room.
**/
LINPHONE_PUBLIC LinphoneChatRoom *linphone_chat_room_ref(LinphoneChatRoom *cr);

/**
 * Release reference to the chat room.
 * @param[in] cr The chat room.
**/
LINPHONE_PUBLIC void linphone_chat_room_unref(LinphoneChatRoom *cr);

/**
 * Retrieve the user pointer associated with the chat room.
 * @param[in] cr The chat room.
 * @return The user pointer associated with the chat room.
**/
LINPHONE_PUBLIC void *linphone_chat_room_get_user_data(const LinphoneChatRoom *cr);

/**
 * Assign a user pointer to the chat room.
 * @param[in] cr The chat room.
 * @param[in] ud The user pointer to associate with the chat room.
**/
LINPHONE_PUBLIC void linphone_chat_room_set_user_data(LinphoneChatRoom *cr, void *ud);

 /**
 * Create a message attached to a dedicated chat room with a particular content.
 * Use #linphone_chat_room_send_message to initiate the transfer
 * @param cr the chat room.
 * @param initial_content #LinphoneContent initial content. #LinphoneCoreVTable.file_transfer_send is invoked later to notify file transfer progress and collect next chunk of the message if #LinphoneContent.data is NULL.
 * @return a new #LinphoneChatMessage
 */
LINPHONE_PUBLIC	LinphoneChatMessage* linphone_chat_room_create_file_transfer_message(LinphoneChatRoom *cr, const LinphoneContent* initial_content);

/**
 * get peer address \link linphone_core_get_chat_room() associated to \endlink this #LinphoneChatRoom
 * @param cr #LinphoneChatRoom object
 * @return #LinphoneAddress peer address
 */
LINPHONE_PUBLIC	const LinphoneAddress* linphone_chat_room_get_peer_address(LinphoneChatRoom *cr);

/**
 * Send a message to peer member of this chat room.
 * @deprecated Use linphone_chat_room_send_chat_message() instead.
 * @param cr #LinphoneChatRoom object
 * @param msg message to be sent
 */
LINPHONE_PUBLIC	LINPHONE_DEPRECATED void linphone_chat_room_send_message(LinphoneChatRoom *cr, const char *msg);
/**
 * Send a message to peer member of this chat room.
 * @param cr #LinphoneChatRoom object
 * @param msg #LinphoneChatMessage message to be sent
 * @param status_cb LinphoneChatMessageStateChangeCb status callback invoked when message is delivered or could not be delivered. May be NULL
 * @param ud user data for the status cb.
 * @deprecated Use linphone_chat_room_send_chat_message() instead.
 * @note The LinphoneChatMessage must not be destroyed until the the callback is called.
 * The LinphoneChatMessage reference is transfered to the function and thus doesn't need to be unref'd by the application.
 */
LINPHONE_PUBLIC	LINPHONE_DEPRECATED void linphone_chat_room_send_message2(LinphoneChatRoom *cr, LinphoneChatMessage* msg,LinphoneChatMessageStateChangedCb status_cb,void* ud);
/**
 * Send a message to peer member of this chat room.
 * @param[in] cr LinphoneChatRoom object
 * @param[in] msg LinphoneChatMessage object
 * The state of the message sending will be notified via the callbacks defined in the LinphoneChatMessageCbs object that can be obtained
 * by calling linphone_chat_message_get_callbacks().
 * The LinphoneChatMessage reference is transfered to the function and thus doesn't need to be unref'd by the application.
 */
LINPHONE_PUBLIC void linphone_chat_room_send_chat_message(LinphoneChatRoom *cr, LinphoneChatMessage *msg);

/**
 * Mark all messages of the conversation as read
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation.
 */
LINPHONE_PUBLIC void linphone_chat_room_mark_as_read(LinphoneChatRoom *cr);
/**
 * Delete a message from the chat room history.
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation.
 * @param[in] msg The #LinphoneChatMessage object to remove.
 */

LINPHONE_PUBLIC void linphone_chat_room_delete_message(LinphoneChatRoom *cr, LinphoneChatMessage *msg);
/**
 * Delete all messages from the history
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation.
 */
LINPHONE_PUBLIC void linphone_chat_room_delete_history(LinphoneChatRoom *cr);
/**
 * Gets the number of messages in a chat room.
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation for which size has to be computed
 * @return the number of messages.
 */
LINPHONE_PUBLIC int linphone_chat_room_get_history_size(LinphoneChatRoom *cr);

/**
 * Gets nb_message most recent messages from cr chat room, sorted from oldest to most recent.
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation for which messages should be retrieved
 * @param[in] nb_message Number of message to retrieve. 0 means everything.
 * @return \bctbx_list{LinphoneChatMessage}
 */
LINPHONE_PUBLIC bctbx_list_t *linphone_chat_room_get_history(LinphoneChatRoom *cr,int nb_message);

/**
 * Gets the partial list of messages in the given range, sorted from oldest to most recent.
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation for which messages should be retrieved
 * @param[in] begin The first message of the range to be retrieved. History most recent message has index 0.
 * @param[in] end The last message of the range to be retrieved. History oldest message has index of history size - 1 (use #linphone_chat_room_get_history_size to retrieve history size)
 * @return \bctbx_list{LinphoneChatMessage}
 */
LINPHONE_PUBLIC bctbx_list_t *linphone_chat_room_get_history_range(LinphoneChatRoom *cr, int begin, int end);

/**
 * Notifies the destination of the chat message being composed that the user is typing a new message.
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation for which a new message is being typed.
 */
LINPHONE_PUBLIC void linphone_chat_room_compose(LinphoneChatRoom *cr);

/**
 * Tells whether the remote is currently composing a message.
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation.
 * @return TRUE if the remote is currently composing a message, FALSE otherwise.
 */
LINPHONE_PUBLIC bool_t linphone_chat_room_is_remote_composing(const LinphoneChatRoom *cr);

/**
 * Gets the number of unread messages in the chatroom.
 * @param[in] cr The #LinphoneChatRoom object corresponding to the conversation.
 * @return the number of unread messages.
 */
LINPHONE_PUBLIC int linphone_chat_room_get_unread_messages_count(LinphoneChatRoom *cr);
/**
 * Returns back pointer to #LinphoneCore object.
 * @deprecated use linphone_chat_room_get_core()
**/
LINPHONE_PUBLIC LINPHONE_DEPRECATED LinphoneCore* linphone_chat_room_get_lc(LinphoneChatRoom *cr);
/**
 * Returns back pointer to #LinphoneCore object.
**/
LINPHONE_PUBLIC LinphoneCore* linphone_chat_room_get_core(LinphoneChatRoom *cr);

/**
 * When realtime text is enabled #linphone_call_params_realtime_text_enabled, #LinphoneCoreIsComposingReceivedCb is call everytime a char is received from peer.
 * At the end of remote typing a regular #LinphoneChatMessage is received with committed data from #LinphoneCoreMessageReceivedCb.
 * @param[in] cr #LinphoneChatRoom object
 * @returns  RFC 4103/T.140 char
 */
LINPHONE_PUBLIC uint32_t linphone_chat_room_get_char(const LinphoneChatRoom *cr);

/**
 * Returns true if lime is available for given peer
 *
 * @return true if zrtp secrets have already been shared and ready to use
 */
 LINPHONE_PUBLIC bool_t linphone_chat_room_lime_available(LinphoneChatRoom *cr);

/**
 * Returns an list of chat rooms
 * @param[in] lc #LinphoneCore object
 * @return \bctbx_list{LinphoneChatRoom}
**/
LINPHONE_PUBLIC const bctbx_list_t* linphone_core_get_chat_rooms(LinphoneCore *lc);
LINPHONE_PUBLIC unsigned int linphone_chat_message_store(LinphoneChatMessage *msg);

/**
 * Returns a #LinphoneChatMessageState as a string.
 */
LINPHONE_PUBLIC	const char* linphone_chat_message_state_to_string(const LinphoneChatMessageState state);
/**
 * Get the state of the message
 *@param message #LinphoneChatMessage obj
 *@return #LinphoneChatMessageState
 */
LINPHONE_PUBLIC	LinphoneChatMessageState linphone_chat_message_get_state(const LinphoneChatMessage* message);
/**
 * Duplicate a LinphoneChatMessage
**/
LINPHONE_PUBLIC LinphoneChatMessage* linphone_chat_message_clone(const LinphoneChatMessage* message);
/**
 * Acquire a reference to the chat message.
 * @param msg the chat message
 * @return the same chat message
**/
LINPHONE_PUBLIC LinphoneChatMessage * linphone_chat_message_ref(LinphoneChatMessage *msg);
/**
 * Release reference to the chat message.
 * @param msg the chat message.
**/
LINPHONE_PUBLIC void linphone_chat_message_unref(LinphoneChatMessage *msg);
/**
 * Destroys a LinphoneChatMessage.
**/
LINPHONE_PUBLIC void linphone_chat_message_destroy(LinphoneChatMessage* msg);
/** @deprecated Use linphone_chat_message_set_from_address() instead. */
#define linphone_chat_message_set_from(msg, addr) linphone_chat_message_set_from_address(msg, addr)
/**
 * Set origin of the message
 * @param[in] message #LinphoneChatMessage obj
 * @param[in] from #LinphoneAddress origin of this message (copied)
 */
LINPHONE_PUBLIC void linphone_chat_message_set_from_address(LinphoneChatMessage* message, const LinphoneAddress* from);
/** @deprecated Use linphone_chat_message_get_from_address() instead. */
#define linphone_chat_message_get_from(msg) linphone_chat_message_get_from_address(msg)
/**
 * Get origin of the message
 * @param[in] message #LinphoneChatMessage obj
 * @return #LinphoneAddress
 */
LINPHONE_PUBLIC	const LinphoneAddress* linphone_chat_message_get_from_address(const LinphoneChatMessage* message);
#define linphone_chat_message_set_to(msg, addr) linphone_chat_message_set_to_address(msg, addr)
/**
 * Set destination of the message
 * @param[in] message #LinphoneChatMessage obj
 * @param[in] addr #LinphoneAddress destination of this message (copied)
 */
LINPHONE_PUBLIC void linphone_chat_message_set_to_address(LinphoneChatMessage* message, const LinphoneAddress* addr);
/** @deprecated Use linphone_chat_message_get_to_address() instead. */
#define linphone_chat_message_get_to(msg) linphone_chat_message_get_to_address(msg)
/**
 * Get destination of the message
 * @param[in] message #LinphoneChatMessage obj
 * @return #LinphoneAddress
 */
LINPHONE_PUBLIC	const LinphoneAddress* linphone_chat_message_get_to_address(const LinphoneChatMessage* message);
/**
 * Linphone message can carry external body as defined by rfc2017
 * @param message #LinphoneChatMessage
 * @return external body url or NULL if not present.
 */
LINPHONE_PUBLIC	const char* linphone_chat_message_get_external_body_url(const LinphoneChatMessage* message);
/**
 * Linphone message can carry external body as defined by rfc2017
 *
 * @param message a LinphoneChatMessage
 * @param url ex: access-type=URL; URL="http://www.foo.com/file"
 */
LINPHONE_PUBLIC	void linphone_chat_message_set_external_body_url(LinphoneChatMessage* message,const char* url);
/**
 * Get the file_transfer_information (used by call backs to recover informations during a rcs file transfer)
 *
 * @param message #LinphoneChatMessage
 * @return a pointer to the LinphoneContent structure or NULL if not present.
 */
LINPHONE_PUBLIC	const LinphoneContent* linphone_chat_message_get_file_transfer_information(const LinphoneChatMessage* message);
/**
 * Start the download of the file from remote server
 *
 * @param message #LinphoneChatMessage
 * @param status_cb LinphoneChatMessageStateChangeCb status callback invoked when file is downloaded or could not be downloaded
 * @param ud user data
 * @deprecated Use linphone_chat_message_download_file() instead.
 */
LINPHONE_PUBLIC LINPHONE_DEPRECATED void linphone_chat_message_start_file_download(LinphoneChatMessage* message, LinphoneChatMessageStateChangedCb status_cb, void* ud);
/**
 * Start the download of the file referenced in a LinphoneChatMessage from remote server.
 * @param[in] message LinphoneChatMessage object.
 */
LINPHONE_PUBLIC int linphone_chat_message_download_file(LinphoneChatMessage *message);
/**
 * Cancel an ongoing file transfer attached to this message.(upload or download)
 * @param msg	#LinphoneChatMessage
 */
LINPHONE_PUBLIC void linphone_chat_message_cancel_file_transfer(LinphoneChatMessage* msg);
/**
 * Linphone message has an app-specific field that can store a text. The application might want
 * to use it for keeping data over restarts, like thumbnail image path.
 * @param message #LinphoneChatMessage
 * @return the application-specific data or NULL if none has been stored.
 */
LINPHONE_PUBLIC	const char* linphone_chat_message_get_appdata(const LinphoneChatMessage* message);
/**
 * Linphone message has an app-specific field that can store a text. The application might want
 * to use it for keeping data over restarts, like thumbnail image path.
 *
 * Invoking this function will attempt to update the message storage to reflect the changeif it is
 * enabled.
 *
 * @param message #LinphoneChatMessage
 * @param data the data to store into the message
 */
LINPHONE_PUBLIC	void linphone_chat_message_set_appdata(LinphoneChatMessage* message, const char* data);
/**
 * Get text part of this message
 * @return text or NULL if no text.
 */
LINPHONE_PUBLIC	const char* linphone_chat_message_get_text(const LinphoneChatMessage* message);
/**
 * Get the time the message was sent.
 */
LINPHONE_PUBLIC	time_t linphone_chat_message_get_time(const LinphoneChatMessage* message);
/**
 * User pointer get function
 */
LINPHONE_PUBLIC	void* linphone_chat_message_get_user_data(const LinphoneChatMessage* message);
/**
 *User pointer set function
 */
LINPHONE_PUBLIC	void linphone_chat_message_set_user_data(LinphoneChatMessage* message, void *user_data);
/**
 * Returns the chatroom this message belongs to.
**/
LINPHONE_PUBLIC LinphoneChatRoom* linphone_chat_message_get_chat_room(LinphoneChatMessage *msg);

LINPHONE_PUBLIC	const LinphoneAddress* linphone_chat_message_get_peer_address(LinphoneChatMessage *msg);
/**
 * Returns the origin address of a message if it was a outgoing message, or the destination address if it was an incoming message.
 *@param message #LinphoneChatMessage obj
 *@return #LinphoneAddress
 */
LINPHONE_PUBLIC	LinphoneAddress *linphone_chat_message_get_local_address(const LinphoneChatMessage* message);
/**
 * Add custom headers to the message.
 * @param message the message
 * @param header_name name of the header_name
 * @param header_value header value
**/
LINPHONE_PUBLIC	void linphone_chat_message_add_custom_header(LinphoneChatMessage* message, const char *header_name, const char *header_value);
/**
 * Retrieve a custom header value given its name.
 * @param message the message
 * @param header_name header name searched
**/
LINPHONE_PUBLIC	const char * linphone_chat_message_get_custom_header(LinphoneChatMessage* message, const char *header_name);
/**
 * Returns TRUE if the message has been read, otherwise returns FALSE.
 * @param message the message
**/
LINPHONE_PUBLIC bool_t linphone_chat_message_is_read(LinphoneChatMessage* message);
/**
 * Returns TRUE if the message has been sent, returns FALSE if the message has been received.
 * @param message the message
**/
LINPHONE_PUBLIC bool_t linphone_chat_message_is_outgoing(LinphoneChatMessage* message);
/**
 * Returns the id used to identify this message in the storage database
 * @param message the message
 * @return the id
 */
LINPHONE_PUBLIC unsigned int linphone_chat_message_get_storage_id(LinphoneChatMessage* message);
LINPHONE_PUBLIC LinphoneReason linphone_chat_message_get_reason(LinphoneChatMessage* msg);
/**
 * Get full details about delivery error of a chat message.
 * @param msg a LinphoneChatMessage
 * @return a LinphoneErrorInfo describing the details.
**/
LINPHONE_PUBLIC const LinphoneErrorInfo *linphone_chat_message_get_error_info(const LinphoneChatMessage *msg);
/**
 * Set the path to the file to read from or write to during the file transfer.
 * @param[in] msg LinphoneChatMessage object
 * @param[in] filepath The path to the file to use for the file transfer.
 */
LINPHONE_PUBLIC void linphone_chat_message_set_file_transfer_filepath(LinphoneChatMessage *msg, const char *filepath);
/**
 * Get the path to the file to read from or write to during the file transfer.
 * @param[in] msg LinphoneChatMessage object
 * @return The path to the file to use for the file transfer.
 */
LINPHONE_PUBLIC const char * linphone_chat_message_get_file_transfer_filepath(LinphoneChatMessage *msg);

/**
 * Fulfill a chat message char by char. Message linked to a Real Time Text Call send char in realtime following RFC 4103/T.140
 * To commit a message, use #linphone_chat_room_send_message
 * @param[in] msg LinphoneChatMessage
 * @param[in] character T.140 char
 * @returns 0 if succeed.
 */
LINPHONE_PUBLIC int linphone_chat_message_put_char(LinphoneChatMessage *msg,uint32_t character);

/**
 * get Curent Call associated to this chatroom if any
 * To commit a message, use #linphone_chat_room_send_message
 * @param[in] room LinphoneChatRomm
 * @returns LinphoneCall or NULL.
 */
LINPHONE_PUBLIC LinphoneCall *linphone_chat_room_get_call(const LinphoneChatRoom *room);


/**
 * Get the LinphoneChatMessageCbs object associated with the LinphoneChatMessage.
 * @param[in] msg LinphoneChatMessage object
 * @return The LinphoneChatMessageCbs object associated with the LinphoneChatMessage.
 */
LINPHONE_PUBLIC LinphoneChatMessageCbs * linphone_chat_message_get_callbacks(const LinphoneChatMessage *msg);

/**
 * Acquire a reference to the LinphoneChatMessageCbs object.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @return The same LinphoneChatMessageCbs object.
 */
LINPHONE_PUBLIC LinphoneChatMessageCbs * linphone_chat_message_cbs_ref(LinphoneChatMessageCbs *cbs);
/**
 * Release reference to the LinphoneChatMessageCbs object.
 * @param[in] cbs LinphoneChatMessageCbs object.
 */
LINPHONE_PUBLIC void linphone_chat_message_cbs_unref(LinphoneChatMessageCbs *cbs);
/**
 * Retrieve the user pointer associated with the LinphoneChatMessageCbs object.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @return The user pointer associated with the LinphoneChatMessageCbs object.
 */
LINPHONE_PUBLIC void *linphone_chat_message_cbs_get_user_data(const LinphoneChatMessageCbs *cbs);
/**
 * Assign a user pointer to the LinphoneChatMessageCbs object.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @param[in] ud The user pointer to associate with the LinphoneChatMessageCbs object.
 */
LINPHONE_PUBLIC void linphone_chat_message_cbs_set_user_data(LinphoneChatMessageCbs *cbs, void *ud);
/**
 * Get the message state changed callback.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @return The current message state changed callback.
 */
LINPHONE_PUBLIC LinphoneChatMessageCbsMsgStateChangedCb linphone_chat_message_cbs_get_msg_state_changed(const LinphoneChatMessageCbs *cbs);
/**
 * Set the message state changed callback.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @param[in] cb The message state changed callback to be used.
 */
LINPHONE_PUBLIC void linphone_chat_message_cbs_set_msg_state_changed(LinphoneChatMessageCbs *cbs, LinphoneChatMessageCbsMsgStateChangedCb cb);
/**
 * Get the file transfer receive callback.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @return The current file transfer receive callback.
 */
LINPHONE_PUBLIC LinphoneChatMessageCbsFileTransferRecvCb linphone_chat_message_cbs_get_file_transfer_recv(const LinphoneChatMessageCbs *cbs);
/**
 * Set the file transfer receive callback.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @param[in] cb The file transfer receive callback to be used.
 */
LINPHONE_PUBLIC void linphone_chat_message_cbs_set_file_transfer_recv(LinphoneChatMessageCbs *cbs, LinphoneChatMessageCbsFileTransferRecvCb cb);
/**
 * Get the file transfer send callback.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @return The current file transfer send callback.
 */
LINPHONE_PUBLIC LinphoneChatMessageCbsFileTransferSendCb linphone_chat_message_cbs_get_file_transfer_send(const LinphoneChatMessageCbs *cbs);
/**
 * Set the file transfer send callback.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @param[in] cb The file transfer send callback to be used.
 */
LINPHONE_PUBLIC void linphone_chat_message_cbs_set_file_transfer_send(LinphoneChatMessageCbs *cbs, LinphoneChatMessageCbsFileTransferSendCb cb);
/**
 * Get the file transfer progress indication callback.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @return The current file transfer progress indication callback.
 */
LINPHONE_PUBLIC LinphoneChatMessageCbsFileTransferProgressIndicationCb linphone_chat_message_cbs_get_file_transfer_progress_indication(const LinphoneChatMessageCbs *cbs);
/**
 * Set the file transfer progress indication callback.
 * @param[in] cbs LinphoneChatMessageCbs object.
 * @param[in] cb The file transfer progress indication callback to be used.
 */
 LINPHONE_PUBLIC void linphone_chat_message_cbs_set_file_transfer_progress_indication(LinphoneChatMessageCbs *cbs, LinphoneChatMessageCbsFileTransferProgressIndicationCb cb);

/**
 * @}
 */


/**
 * @addtogroup initializing
 * @{
**/

/**
 * LinphoneGlobalState describes the global state of the LinphoneCore object.
 * It is notified via the LinphoneCoreVTable::global_state_changed
**/
typedef enum _LinphoneGlobalState{
	LinphoneGlobalOff,
	LinphoneGlobalStartup,
	LinphoneGlobalOn,
	LinphoneGlobalShutdown,
	LinphoneGlobalConfiguring
}LinphoneGlobalState;

LINPHONE_PUBLIC const char *linphone_global_state_to_string(LinphoneGlobalState gs);

/**
 * LinphoneCoreLogCollectionUploadState is used to notify if log collection upload have been succesfully delivered or not.
 */
typedef enum _LinphoneCoreLogCollectionUploadState {
	LinphoneCoreLogCollectionUploadStateInProgress, /**< Delivery in progress */
	LinphoneCoreLogCollectionUploadStateDelivered, /**< Log collection upload successfully delivered and acknowledged by remote end point */
	LinphoneCoreLogCollectionUploadStateNotDelivered, /**< Log collection upload was not delivered */
} LinphoneCoreLogCollectionUploadState;

LINPHONE_PUBLIC const char *linphone_core_log_collection_upload_state_to_string(const LinphoneCoreLogCollectionUploadState lcus);

/**
 * Global state notification callback.
 * @param lc the #LinphoneCore.
 * @param gstate the global state
 * @param message informational message.
 */
typedef void (*LinphoneCoreCbsGlobalStateChangedCb)(LinphoneCore *lc, LinphoneGlobalState gstate, const char *message);
/**
 * Old name of #LinphoneCoreCbsGlobalStateChangedCb.
 */
typedef LinphoneCoreCbsGlobalStateChangedCb LinphoneCoreGlobalStateChangedCb;
/**
 * Call state notification callback.
 * @param lc the LinphoneCore
 * @param call the call object whose state is changed.
 * @param cstate the new state of the call
 * @param message a non NULL informational message about the state.
 */
typedef void (*LinphoneCoreCbsCallStateChangedCb)(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *message);
/**
 * Old name of #LinphoneCoreCbsCallStateChangedCb.
 */
typedef LinphoneCoreCbsCallStateChangedCb LinphoneCoreCallStateChangedCb;

/**
 * Call encryption changed callback.
 * @param lc the LinphoneCore
 * @param call the call on which encryption is changed.
 * @param on whether encryption is activated.
 * @param authentication_token an authentication_token, currently set for ZRTP kind of encryption only.
 */
typedef void (*LinphoneCoreCbsCallEncryptionChangedCb)(LinphoneCore *lc, LinphoneCall *call, bool_t on, const char *authentication_token);
/**
 * Old name of #LinphoneCoreCbsCallEncryptionChangedCb.
 */
typedef LinphoneCoreCbsCallEncryptionChangedCb LinphoneCoreCallEncryptionChangedCb;

/** @ingroup Proxies
 * Registration state notification callback prototype
 * */
typedef void (*LinphoneCoreCbsRegistrationStateChangedCb)(LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message) ;
/**
 * Old name of #LinphoneCoreCbsRegistrationStateChangedCb.
 */
typedef LinphoneCoreCbsRegistrationStateChangedCb LinphoneCoreRegistrationStateChangedCb;

/** Callback prototype
 * @deprecated
 */
typedef void (*ShowInterfaceCb)(LinphoneCore *lc) ;
/** Callback prototype
 * @deprecated
 */
typedef void (*DisplayStatusCb)(LinphoneCore *lc, const char *message) ;
/** Callback prototype
 * @deprecated
 */
typedef void (*DisplayMessageCb)(LinphoneCore *lc, const char *message) ;
/** Callback prototype
 * @deprecated
 */
typedef void (*DisplayUrlCb)(LinphoneCore *lc, const char *message, const char *url) ;
/** Callback prototype
 */
typedef void (*LinphoneCoreCbFunc)(LinphoneCore *lc,void * user_data);
/**
 * Report status change for a friend previously \link linphone_core_add_friend() added \endlink to #LinphoneCore.
 * @param lc #LinphoneCore object .
 * @param lf Updated #LinphoneFriend .
 */
typedef void (*LinphoneCoreCbsNotifyPresenceReceivedCb)(LinphoneCore *lc, LinphoneFriend * lf);
/**
 * Old name of #LinphoneCoreCbsNotifyPresenceReceivedCb.
 */
typedef LinphoneCoreCbsNotifyPresenceReceivedCb LinphoneCoreNotifyPresenceReceivedCb;
/**
 * Reports presence model change for a specific URI or phone number of a friend
 * @param lc #LinphoneCore object
 * @param lf #LinphoneFriend object
 * @param uri_or_tel The URI or phone number for which teh presence model has changed
 * @param presence_model The new presence model
 */
typedef void (*LinphoneCoreCbsNotifyPresenceReceivedForUriOrTelCb)(LinphoneCore *lc, LinphoneFriend *lf, const char *uri_or_tel, const LinphonePresenceModel *presence_model);
/**
 * Old name of #LinphoneCoreCbsNotifyPresenceReceivedForUriOrTelCb.
 */
typedef LinphoneCoreCbsNotifyPresenceReceivedForUriOrTelCb LinphoneCoreNotifyPresenceReceivedForUriOrTelCb;
/**
 *  Reports that a new subscription request has been received and wait for a decision.
 *  Status on this subscription request is notified by \link linphone_friend_set_inc_subscribe_policy() changing policy \endlink for this friend
 * @param lc #LinphoneCore object
 * @param lf #LinphoneFriend corresponding to the subscriber
 * @param url of the subscriber
 *  Callback prototype
 */
typedef void (*LinphoneCoreCbsNewSubscriptionRequestedCb)(LinphoneCore *lc, LinphoneFriend *lf, const char *url);
/**
 * Old name of #LinphoneCoreCbsNewSubscriptionRequestedCb.
 */
typedef LinphoneCoreCbsNewSubscriptionRequestedCb LinphoneCoreNewSubscriptionRequestedCb;
/**
 * Callback for requesting authentication information to application or user.
 * @param lc the LinphoneCore
 * @param realm the realm (domain) on which authentication is required.
 * @param username the username that needs to be authenticated.
 * @param domain the domain on which authentication is required.
 * Application shall reply to this callback using linphone_core_add_auth_info().
 */
typedef void (*LinphoneCoreAuthInfoRequestedCb)(LinphoneCore *lc, const char *realm, const char *username, const char *domain);

/**
 * Callback for requesting authentication information to application or user.
 * @param lc the LinphoneCore
 * @param auth_info a LinphoneAuthInfo pre-filled with username, realm and domain values as much as possible
 * @param method the type of authentication requested
 * Application shall reply to this callback using linphone_core_add_auth_info().
 */
typedef void (*LinphoneCoreCbsAuthenticationRequestedCb)(LinphoneCore *lc, LinphoneAuthInfo *auth_info, LinphoneAuthMethod method);
/**
 * Old name of #LinphoneCoreCbsAuthenticationRequestedCb.
 */
typedef LinphoneCoreCbsAuthenticationRequestedCb LinphoneCoreAuthenticationRequestedCb;

/**
 * Callback to notify a new call-log entry has been added.
 * This is done typically when a call terminates.
 * @param lc the LinphoneCore
 * @param newcl the new call log entry added.
 */
typedef void (*LinphoneCoreCbsCallLogUpdatedCb)(LinphoneCore *lc, LinphoneCallLog *newcl);
/**
 * Old name of #LinphoneCoreCbsCallLogUpdatedCb.
 */
typedef LinphoneCoreCbsCallLogUpdatedCb LinphoneCoreCallLogUpdatedCb;

/**
 * Callback prototype
 * @deprecated use #LinphoneCoreMessageReceivedCb instead.
 *
 * @param lc #LinphoneCore object
 * @param room #LinphoneChatRoom involved in this conversation. Can be be created by the framework in case \link #LinphoneAddress the from \endlink is not present in any chat room.
 * @param from #LinphoneAddress from
 * @param message incoming message
 */
typedef void (*LinphoneCoreTextMessageReceivedCb)(LinphoneCore *lc, LinphoneChatRoom *room, const LinphoneAddress *from, const char *message);

/**
 * Chat message callback prototype
 *
 * @param lc #LinphoneCore object
 * @param room #LinphoneChatRoom involved in this conversation. Can be be created by the framework in case \link #LinphoneAddress the from \endlink is not present in any chat room.
 * @param LinphoneChatMessage incoming message
 */
typedef void (*LinphoneCoreCbsMessageReceivedCb)(LinphoneCore *lc, LinphoneChatRoom *room, LinphoneChatMessage *message);
/**
 * Old name of #LinphoneCoreCbsMessageReceivedCb.
 */
typedef LinphoneCoreCbsMessageReceivedCb LinphoneCoreMessageReceivedCb;

/**
 * File transfer receive callback prototype. This function is called by the core upon an incoming File transfer is started. This function may be call several time for the same file in case of large file.
 *
 *
 * @param lc #LinphoneCore object
 * @param message #LinphoneChatMessage message from which the body is received.
 * @param content #LinphoneContent incoming content information
 * @param buff pointer to the received data
 * @param size number of bytes to be read from buff. 0 means end of file.
 *
 */
typedef void (*LinphoneCoreFileTransferRecvCb)(LinphoneCore *lc, LinphoneChatMessage *message, const LinphoneContent* content, const char* buff, size_t size);

/**
 * File transfer send callback prototype. This function is called by the core upon an outgoing File transfer is started. This function is called until size is set to 0.
 * <br> a #LinphoneContent with a size equal zero
 *
 * @param lc #LinphoneCore object
 * @param message #LinphoneChatMessage message from which the body is received.
 * @param content #LinphoneContent outgoing content
 * @param buff pointer to the buffer where data chunk shall be written by the app
 * @param size as input value, it represents the number of bytes expected by the framework. As output value, it means the number of bytes wrote by the application in the buffer. 0 means end of file.
 *
 */
typedef void (*LinphoneCoreFileTransferSendCb)(LinphoneCore *lc, LinphoneChatMessage *message,  const LinphoneContent* content, char* buff, size_t* size);

/**
 * File transfer progress indication callback prototype.
 *
 * @param lc #LinphoneCore object
 * @param message #LinphoneChatMessage message from which the body is received.
 * @param content #LinphoneContent incoming content information
 * @param offset The number of bytes sent/received since the beginning of the transfer.
 * @param total The total number of bytes to be sent/received.
 */
typedef void (*LinphoneCoreFileTransferProgressIndicationCb)(LinphoneCore *lc, LinphoneChatMessage *message, const LinphoneContent* content, size_t offset, size_t total);

/**
 * Is composing notification callback prototype.
 *
 * @param[in] lc #LinphoneCore object
 * @param[in] room #LinphoneChatRoom involved in the conversation.
 */
typedef void (*LinphoneCoreCbsIsComposingReceivedCb)(LinphoneCore *lc, LinphoneChatRoom *room);
/**
 * Old name of #LinphoneCoreCbsIsComposingReceivedCb.
 */
typedef LinphoneCoreCbsIsComposingReceivedCb LinphoneCoreIsComposingReceivedCb;

/**
 * Callback for being notified of DTMFs received.
 * @param lc the linphone core
 * @param call the call that received the dtmf
 * @param dtmf the ascii code of the dtmf
 */
typedef void (*LinphoneCoreCbsDtmfReceivedCb)(LinphoneCore* lc, LinphoneCall *call, int dtmf);
/**
 * Old name of #LinphoneCoreCbsDtmfReceivedCb.
 */
typedef LinphoneCoreCbsDtmfReceivedCb LinphoneCoreDtmfReceivedCb;

/** Callback prototype */
typedef void (*LinphoneCoreCbsReferReceivedCb)(LinphoneCore *lc, const char *refer_to);
/**
 * Old name of #LinphoneCoreCbsReferReceivedCb.
 */
typedef LinphoneCoreCbsReferReceivedCb LinphoneCoreReferReceivedCb;
/** Callback prototype */
typedef void (*LinphoneCoreCbsBuddyInfoUpdatedCb)(LinphoneCore *lc, LinphoneFriend *lf);
/**
 * Old name of #LinphoneCoreCbsBuddyInfoUpdatedCb.
 */
typedef LinphoneCoreCbsBuddyInfoUpdatedCb LinphoneCoreBuddyInfoUpdatedCb;
/**
 * Callback for notifying progresses of transfers.
 * @param lc the LinphoneCore
 * @param transfered the call that was transfered
 * @param new_call_state the state of the call to transfer target at the far end.
 */
typedef void (*LinphoneCoreCbsTransferStateChangedCb)(LinphoneCore *lc, LinphoneCall *transfered, LinphoneCallState new_call_state);
/**
 * Old name of LinphoneCoreCbsTransferStateChangedCb.
 */
typedef LinphoneCoreCbsTransferStateChangedCb LinphoneCoreTransferStateChangedCb;

/**
 * Callback for receiving quality statistics for calls.
 * @param lc the LinphoneCore
 * @param call the call
 * @param stats the call statistics.
 */
typedef void (*LinphoneCoreCbsCallStatsUpdatedCb)(LinphoneCore *lc, LinphoneCall *call, const LinphoneCallStats *stats);
/**
 * Old name of #LinphoneCoreCbsCallStatsUpdatedCb.
 */
typedef LinphoneCoreCbsCallStatsUpdatedCb LinphoneCoreCallStatsUpdatedCb;

/**
 * Callback prototype for receiving info messages.
 * @param lc the LinphoneCore
 * @param call the call whose info message belongs to.
 * @param msg the info message.
 */
typedef void (*LinphoneCoreCbsInfoReceivedCb)(LinphoneCore *lc, LinphoneCall *call, const LinphoneInfoMessage *msg);
/**
 * Old name of #LinphoneCoreCbsInfoReceivedCb.
 */
typedef LinphoneCoreCbsInfoReceivedCb LinphoneCoreInfoReceivedCb;

/**
 * LinphoneGlobalState describes the global state of the LinphoneCore object.
 * It is notified via the LinphoneCoreVTable::global_state_changed
**/
typedef enum _LinphoneConfiguringState {
	LinphoneConfiguringSuccessful,
	LinphoneConfiguringFailed,
	LinphoneConfiguringSkipped
} LinphoneConfiguringState;

/**
 * Converts a _LinphoneConfiguringState enum to a string.
 * @ingroup misc
**/
LINPHONE_PUBLIC const char *linphone_configuring_state_to_string(LinphoneConfiguringState cs);

/**
 * Callback prototype for configuring status changes notification
 * @param lc the LinphoneCore
 * @param message informational message.
 */
typedef void (*LinphoneCoreCbsConfiguringStatusCb)(LinphoneCore *lc, LinphoneConfiguringState status, const char *message);
/**
 * Old name of #LinphoneCoreCbsConfiguringStatusCb.
 */
typedef LinphoneCoreCbsConfiguringStatusCb LinphoneCoreConfiguringStatusCb;

/**
 * Callback prototype for reporting network change either automatically detected or notified by #linphone_core_set_network_reachable.
 * @param lc the LinphoneCore
 * @param reachable true if network is reachable.
 */
typedef void (*LinphoneCoreCbsNetworkReachableCb)(LinphoneCore *lc, bool_t reachable);
/**
 * Old name of #LinphoneCoreCbsNetworkReachableCb.
 */
typedef LinphoneCoreCbsNetworkReachableCb LinphoneCoreNetworkReachableCb;

/**
 * Callback prototype for reporting log collection upload state change.
 * @param[in] lc LinphoneCore object
 * @param[in] state The state of the log collection upload
 * @param[in] info Additional information: error message in case of error state, URL of uploaded file in case of success.
 */
typedef void (*LinphoneCoreCbsLogCollectionUploadStateChangedCb)(LinphoneCore *lc, LinphoneCoreLogCollectionUploadState state, const char *info);
/**
 * Old name of #LinphoneCoreCbsLogCollectionUploadStateChangedCb.
 */
typedef LinphoneCoreCbsLogCollectionUploadStateChangedCb LinphoneCoreLogCollectionUploadStateChangedCb;

/**
 * Callback prototype for reporting log collection upload progress indication.
 * @param[in] lc LinphoneCore object
 */
typedef void (*LinphoneCoreCbsLogCollectionUploadProgressIndicationCb)(LinphoneCore *lc, size_t offset, size_t total);
/**
 * Old name of #LinphoneCoreCbsLogCollectionUploadProgressIndicationCb.
 */
typedef LinphoneCoreCbsLogCollectionUploadProgressIndicationCb LinphoneCoreLogCollectionUploadProgressIndicationCb;

/**
 * Callback prototype for reporting when a friend list has been added to the core friends list.
 * @param[in] lc LinphoneCore object
 * @param[in] list LinphoneFriendList object
 */
typedef void (*LinphoneCoreCbsFriendListCreatedCb) (LinphoneCore *lc, LinphoneFriendList *list);
/**
 * Old name of #LinphoneCoreCbsFriendListCreatedCb.
 */
typedef LinphoneCoreCbsFriendListCreatedCb LinphoneCoreFriendListCreatedCb;

/**
 * Callback prototype for reporting when a friend list has been removed from the core friends list.
 * @param[in] lc LinphoneCore object
 * @param[in] list LinphoneFriendList object
 */
typedef void (*LinphoneCoreCbsFriendListRemovedCb) (LinphoneCore *lc, LinphoneFriendList *list);
/**
 * Old name of #LinphoneCoreCbsFriendListRemovedCb.
 */
typedef LinphoneCoreCbsFriendListRemovedCb LinphoneCoreFriendListRemovedCb;

/**
 * This structure holds all callbacks that the application should implement.
 *  None is mandatory.
**/
typedef struct _LinphoneCoreVTable{
	LinphoneCoreGlobalStateChangedCb global_state_changed; /**<Notifies global state changes*/
	LinphoneCoreRegistrationStateChangedCb registration_state_changed;/**<Notifies registration state changes*/
	LinphoneCoreCallStateChangedCb call_state_changed;/**<Notifies call state changes*/
	LinphoneCoreNotifyPresenceReceivedCb notify_presence_received; /**< Notify received presence events*/
	LinphoneCoreNotifyPresenceReceivedForUriOrTelCb notify_presence_received_for_uri_or_tel; /**< Notify received presence events*/
	LinphoneCoreNewSubscriptionRequestedCb new_subscription_requested; /**< Notify about pending presence subscription request */
	LINPHONE_DEPRECATED LinphoneCoreAuthInfoRequestedCb auth_info_requested; /**< @deprecated Use authentication_requested instead. Ask the application some authentication information */
	LinphoneCoreAuthenticationRequestedCb authentication_requested; /**< Ask the application some authentication information */
	LinphoneCoreCallLogUpdatedCb call_log_updated; /**< Notifies that call log list has been updated */
	LinphoneCoreMessageReceivedCb message_received; /**< a message is received, can be text or external body*/
	LinphoneCoreIsComposingReceivedCb is_composing_received; /**< An is-composing notification has been received */
	LinphoneCoreDtmfReceivedCb dtmf_received; /**< A dtmf has been received received */
	LinphoneCoreReferReceivedCb refer_received; /**< An out of call refer was received */
	LinphoneCoreCallEncryptionChangedCb call_encryption_changed; /**<Notifies on change in the encryption of call streams */
	LinphoneCoreTransferStateChangedCb transfer_state_changed; /**<Notifies when a transfer is in progress */
	LinphoneCoreBuddyInfoUpdatedCb buddy_info_updated; /**< a LinphoneFriend's BuddyInfo has changed*/
	LinphoneCoreCallStatsUpdatedCb call_stats_updated; /**<Notifies on refreshing of call's statistics. */
	LinphoneCoreInfoReceivedCb info_received; /**<Notifies an incoming informational message received.*/
	LinphoneCoreSubscriptionStateChangedCb subscription_state_changed; /**<Notifies subscription state change */
	LinphoneCoreNotifyReceivedCb notify_received; /**< Notifies a an event notification, see linphone_core_subscribe() */
	LinphoneCorePublishStateChangedCb publish_state_changed;/**Notifies publish state change (only from #LinphoneEvent api)*/
	LinphoneCoreConfiguringStatusCb configuring_status; /** Notifies configuring status changes */
	LINPHONE_DEPRECATED DisplayStatusCb display_status; /**< @deprecated Callback that notifies various events with human readable text.*/
	LINPHONE_DEPRECATED DisplayMessageCb display_message;/**< @deprecated Callback to display a message to the user */
	LINPHONE_DEPRECATED DisplayMessageCb display_warning;/**< @deprecated Callback to display a warning to the user */
	LINPHONE_DEPRECATED DisplayUrlCb display_url; /**< @deprecated */
	LINPHONE_DEPRECATED ShowInterfaceCb show; /**< vNotifies the application that it should show up*/
	LINPHONE_DEPRECATED LinphoneCoreTextMessageReceivedCb text_received; /**< @deprecated, use #message_received instead <br> A text message has been received */
	LINPHONE_DEPRECATED LinphoneCoreFileTransferRecvCb file_transfer_recv; /**< @deprecated Callback to store file received attached to a #LinphoneChatMessage */
	LINPHONE_DEPRECATED LinphoneCoreFileTransferSendCb file_transfer_send; /**< @deprecated Callback to collect file chunk to be sent for a #LinphoneChatMessage */
	LINPHONE_DEPRECATED LinphoneCoreFileTransferProgressIndicationCb file_transfer_progress_indication; /**< @deprecated Callback to indicate file transfer progress */
	LinphoneCoreNetworkReachableCb network_reachable; /**< Callback to report IP network status (I.E up/down )*/
	LinphoneCoreLogCollectionUploadStateChangedCb log_collection_upload_state_changed; /**< Callback to upload collected logs */
	LinphoneCoreLogCollectionUploadProgressIndicationCb log_collection_upload_progress_indication; /**< Callback to indicate log collection upload progress */
	LinphoneCoreFriendListCreatedCb friend_list_created;
	LinphoneCoreFriendListRemovedCb friend_list_removed;
	void *user_data; /**<User data associated with the above callbacks */
} LinphoneCoreVTable;

/**
 * Instantiate a vtable with all arguments set to NULL
 * @return newly allocated vtable
 */
LINPHONE_PUBLIC LinphoneCoreVTable *linphone_core_v_table_new(void);

/**
 * Sets a user data pointer in the vtable.
 * @param table the vtable
 * @param data the user data to attach
 */
LINPHONE_PUBLIC void linphone_core_v_table_set_user_data(LinphoneCoreVTable *table, void *data);

/**
 * Gets a user data pointer in the vtable.
 * @param table the vtable
 * @return the data attached to the vtable
 */
LINPHONE_PUBLIC void* linphone_core_v_table_get_user_data(const LinphoneCoreVTable *table);

/**
 * Gets the current VTable.
 * This is meant only to be called from a callback to be able to get the user_data associated with the vtable that called the callback.
 * @param lc the linphonecore
 * @return the vtable that called the last callback
 */
LINPHONE_PUBLIC LinphoneCoreVTable *linphone_core_get_current_vtable(LinphoneCore *lc);

/**
 * Destroy a vtable.
 * @param table to be destroyed
 */
LINPHONE_PUBLIC void linphone_core_v_table_destroy(LinphoneCoreVTable* table);


/**
 * That class holds all the callbacks which are called by #LinphoneCore.
 *
 * Use linphone_factory_create_core_cbs() to create an instance. Then, call the
 * callback setters on the events you need to monitor and pass the object to
 * a #LinphoneCore instance through linphone_core_add_listener().
 *
 * That class is inherited from #belle_sip_object_t.
 */
typedef struct _LinphoneCoreCbs LinphoneCoreCbs;

/**
 * Increment the reference counter.
 */
LINPHONE_PUBLIC LinphoneCoreCbs *linphone_core_cbs_ref(LinphoneCoreCbs *cbs);

/**
 * Decrement the reference counter.
 */
LINPHONE_PUBLIC void linphone_core_cbs_unref(LinphoneCoreCbs *cbs);

/**
 * Set private data to be get from each callbacks.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_user_data(LinphoneCoreCbs *cbs, void *user_data);

/**
 * Get the user pointer.
 */
LINPHONE_PUBLIC void *linphone_core_cbs_get_user_data(LinphoneCoreCbs *cbs);

/**
 * Gets the current #LinphoneCoreCbs.
 * This is meant only to be called from a callback to be able to get the user_data associated with the #LinphoneCoreCbs that is calling the callback.
 * @param lc the linphonecore
 * @return the #LinphoneCoreCbs that has called the last callback
 */
LINPHONE_PUBLIC LinphoneCoreCbs *linphone_core_get_current_callbacks(const LinphoneCore *lc);

/**
 * Set the #LinphoneCoreCbsRegistrationStateChangedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_registration_state_changed(LinphoneCoreCbs *cbs, LinphoneCoreCbsRegistrationStateChangedCb cb);

/**
 * Set the #LinphoneCoreCbsCallStateChangedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_call_state_changed(LinphoneCoreCbs *cbs, LinphoneCoreCbsCallStateChangedCb cb);

/**
 * Set the #LinphoneCoreCbsNotifyPresenceReceivedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_notify_presence_received(LinphoneCoreCbs *cbs, LinphoneCoreCbsNotifyPresenceReceivedCb cb);

/**
 * Set the #LinphoneCoreCbsNotifyPresenceReceivedForUriOrTelCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_notify_presence_received_for_uri_or_tel(LinphoneCoreCbs *cbs, LinphoneCoreCbsNotifyPresenceReceivedForUriOrTelCb cb);

/**
 * Set the #LinphoneCoreCbsNewSubscriptionRequestedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_new_subscription_requested(LinphoneCoreCbs *cbs, LinphoneCoreCbsNewSubscriptionRequestedCb cb);

/**
 * Set the #LinphoneCoreCbsAuthenticationRequestedCb callback.'
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_authentication_requested(LinphoneCoreCbs *cbs, LinphoneCoreCbsAuthenticationRequestedCb cb);

/**
 * Set the #LinphoneCoreCbsCallLogUpdatedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_call_log_updated(LinphoneCoreCbs *cbs, LinphoneCoreCbsCallLogUpdatedCb cb);

/**
 * Set the #LinphoneCoreCbsMessageReceivedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_message_received(LinphoneCoreCbs *cbs, LinphoneCoreCbsMessageReceivedCb cb);

/**
 * Set the #LinphoneCoreCbsIsComposingReceivedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_is_composing_received(LinphoneCoreCbs *cbs, LinphoneCoreCbsIsComposingReceivedCb cb);

/**
 * Set the #LinphoneCoreCbsDtmfReceivedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_dtmf_received(LinphoneCoreCbs *cbs, LinphoneCoreCbsDtmfReceivedCb cb);

/**
 * Set the #LinphoneCoreCbsReferReceivedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_refer_received(LinphoneCoreCbs *cbs, LinphoneCoreCbsReferReceivedCb cb);

/**
 * Set the #LinphoneCoreCbsCallEncryptionChangedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_call_encryption_changed(LinphoneCoreCbs *cbs, LinphoneCoreCbsCallEncryptionChangedCb cb);

/**
 * Set the #LinphoneCoreCbsTransferStateChangedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_transfer_state_changed(LinphoneCoreCbs *cbs, LinphoneCoreCbsTransferStateChangedCb cb);

/**
 * Set the #LinphoneCoreCbsBuddyInfoUpdatedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_buddy_info_updated(LinphoneCoreCbs *cbs, LinphoneCoreCbsBuddyInfoUpdatedCb cb);

/**
 * Set the #LinphoneCoreCbsCallStatsUpdatedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_call_stats_updated(LinphoneCoreCbs *cbs, LinphoneCoreCbsCallStatsUpdatedCb cb);

/**
 * Set the #LinphoneCoreCbsInfoReceivedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_info_received(LinphoneCoreCbs *cbs, LinphoneCoreCbsInfoReceivedCb cb);

/**
 * Set the #LinphoneCoreCbsSubscriptionStateChangedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_subscription_state_changed(LinphoneCoreCbs *cbs, LinphoneCoreCbsSubscriptionStateChangedCb cb);

/**
 * Set the #LinphoneCoreCbsNotifyReceivedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_notify_received(LinphoneCoreCbs *cbs, LinphoneCoreCbsNotifyReceivedCb cb);

/**
 * Set the #LinphoneCoreCbsPublishStateChangedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_publish_state_changed(LinphoneCoreCbs *cbs, LinphoneCoreCbsPublishStateChangedCb cb);

/**
 * Set the #LinphoneCoreCbsConfiguringStatusCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_configuring_status(LinphoneCoreCbs *cbs, LinphoneCoreCbsConfiguringStatusCb cb);

/**
 * Set the #LinphoneCoreCbsNetworkReachableCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_network_reachable(LinphoneCoreCbs *cbs, LinphoneCoreCbsNetworkReachableCb cb);

/**
 * Set the #LinphoneCoreCbsLogCollectionUploadStateChangedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_log_collection_upload_state_changed(LinphoneCoreCbs *cbs, LinphoneCoreCbsLogCollectionUploadStateChangedCb cb);

/**
 * Set the #LinphoneCoreCbsLogCollectionUploadProgressIndicationCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_log_collection_upload_progress_indication(LinphoneCoreCbs *cbs, LinphoneCoreCbsLogCollectionUploadProgressIndicationCb cb);

/**
 * Set the #LinphoneCoreCbsFriendListCreatedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_friend_list_created(LinphoneCoreCbs *cbs, LinphoneCoreCbsFriendListCreatedCb cb);

/**
 * Set the #LinphoneCoreCbsFriendListRemovedCb callback.
 * @param[in] cbs A #LinphoneCoreCbs.
 * @param[in] cb The callback.
 */
LINPHONE_PUBLIC void linphone_core_cbs_set_friend_list_removed(LinphoneCoreCbs *cbs, LinphoneCoreCbsFriendListRemovedCb cb);

/**
 * @}
**/

typedef struct _LCCallbackObj
{
	LinphoneCoreCbFunc _func;
	void * _user_data;
}LCCallbackObj;


/**
 * Policy to use to pass through firewalls.
 * @ingroup network_parameters
 * @deprecated Use #LinphoneNatPolicy instead.
**/
typedef enum _LinphoneFirewallPolicy {
	LinphonePolicyNoFirewall, /**< Do not use any mechanism to pass through firewalls */
	LinphonePolicyUseNatAddress, /**< Use the specified public adress */
	LinphonePolicyUseStun, /**< Use a STUN server to get the public address */
	LinphonePolicyUseIce, /**< Use the ICE protocol */
	LinphonePolicyUseUpnp, /**< Use the uPnP protocol */
} LinphoneFirewallPolicy;

typedef enum _LinphoneWaitingState{
	LinphoneWaitingStart,
	LinphoneWaitingProgress,
	LinphoneWaitingFinished
} LinphoneWaitingState;
typedef void * (*LinphoneCoreWaitingCallback)(LinphoneCore *lc, void *context, LinphoneWaitingState ws, const char *purpose, float progress);


/* THE main API */

/**
 * @addtogroup initializing
 * @{
**/

typedef enum _LinphoneLogCollectionState {
	LinphoneLogCollectionDisabled,
	LinphoneLogCollectionEnabled,
	LinphoneLogCollectionEnabledWithoutPreviousLogHandler
} LinphoneLogCollectionState;

/**
 * Tells whether the linphone core log collection is enabled.
 * @return The state of the linphone core log collection.
 */
LINPHONE_PUBLIC LinphoneLogCollectionState linphone_core_log_collection_enabled(void);

/**
 * Enable the linphone core log collection to upload logs on a server.
 * @param[in] state LinphoneLogCollectionState value telling whether to enable log collection or not.
 */
LINPHONE_PUBLIC void linphone_core_enable_log_collection(LinphoneLogCollectionState state);

/**
 * Get the path where the log files will be written for log collection.
 * @return The path where the log files will be written.
 */
LINPHONE_PUBLIC const char * linphone_core_get_log_collection_path(void);

/**
 * Set the path where the log files will be written for log collection.
 * @param[in] path The path where the log files will be written.
 */
LINPHONE_PUBLIC void linphone_core_set_log_collection_path(const char *path);

/**
 * Get the prefix of the filenames that will be used for log collection.
 * @return The prefix of the filenames used for log collection.
 */
LINPHONE_PUBLIC const char * linphone_core_get_log_collection_prefix(void);

/**
 * Set the prefix of the filenames that will be used for log collection.
 * @param[in] prefix The prefix to use for the filenames for log collection.
 */
LINPHONE_PUBLIC void linphone_core_set_log_collection_prefix(const char *prefix);

/**
 * Get the max file size in bytes of the files used for log collection.
 * @return The max file size in bytes of the files used for log collection.
 */
LINPHONE_PUBLIC size_t linphone_core_get_log_collection_max_file_size(void);

/**
 * Set the max file size in bytes of the files used for log collection.
 * Warning: this function should only not be used to change size
 * dynamically but instead only before calling @see
 * linphone_core_enable_log_collection. If you increase max size
  * on runtime, logs chronological order COULD be broken.
 * @param[in] size The max file size in bytes of the files used for log collection.
 */
LINPHONE_PUBLIC void linphone_core_set_log_collection_max_file_size(size_t size);

/**
 * Set the url of the server where to upload the collected log files.
 * @param[in] core LinphoneCore object
 * @param[in] server_url The url of the server where to upload the collected log files.
 */
LINPHONE_PUBLIC void linphone_core_set_log_collection_upload_server_url(LinphoneCore *core, const char *server_url);

/**
 * Upload the log collection to the configured server url.
 * @param[in] core LinphoneCore object
 */
LINPHONE_PUBLIC void linphone_core_upload_log_collection(LinphoneCore *core);

/**
 * Compress the log collection in a single file.
 * @return The path of the compressed log collection file (to be freed calling ms_free()).
 */
LINPHONE_PUBLIC char * linphone_core_compress_log_collection(void);

/**
 * Reset the log collection by removing the log files.
 */
LINPHONE_PUBLIC void linphone_core_reset_log_collection(void);

/**
 * Define a log handler.
 *
 * @param logfunc The function pointer of the log handler.
 */
LINPHONE_PUBLIC void linphone_core_set_log_handler(OrtpLogFunc logfunc);

/**
 * Define a log file.
 *
 * If the file pointer passed as an argument is NULL, stdout is used instead.
 *
 * @param file A pointer to the FILE structure of the file to write to.
 */
LINPHONE_PUBLIC void linphone_core_set_log_file(FILE *file);

/**
 * Define the minimum level for logging.
 *
 * @param loglevel Minimum level for logging messages.
**/
LINPHONE_PUBLIC void linphone_core_set_log_level(OrtpLogLevel loglevel);

/**
 * Define the log level using mask.
 *
 * The loglevel parameter is a bitmask parameter. Therefore to enable only warning and error
 * messages, use ORTP_WARNING | ORTP_ERROR. To disable logs, simply set loglevel to 0.
 *
 * @param loglevel A bitmask of the log levels to set.
 */
LINPHONE_PUBLIC void linphone_core_set_log_level_mask(OrtpLogLevel loglevel);

/**
 * Enable logs in supplied FILE*.
 *
 * @deprecated Use #linphone_core_set_log_file and #linphone_core_set_log_level instead.
 *
 * @param file a C FILE* where to fprintf logs. If null stdout is used.
 *
**/
LINPHONE_DEPRECATED LINPHONE_PUBLIC void linphone_core_enable_logs(FILE *file);

/**
 * Enable logs through the user's supplied log callback.
 *
 * @deprecated Use #linphone_core_set_log_handler and #linphone_core_set_log_level instead.
 *
 * @param logfunc The address of a OrtpLogFunc callback whose protoype is
 *            	  typedef void (*OrtpLogFunc)(OrtpLogLevel lev, const char *fmt, va_list args);
 *
**/
LINPHONE_DEPRECATED LINPHONE_PUBLIC void linphone_core_enable_logs_with_cb(OrtpLogFunc logfunc);

/**
 * Entirely disable logging.
 *
 * @deprecated Use #linphone_core_set_log_level instead.
**/
LINPHONE_DEPRECATED LINPHONE_PUBLIC void linphone_core_disable_logs(void);

/**
 * Enable logs serialization (output logs from either the thread that creates the linphone core or the thread that calls linphone_core_iterate()).
 * Must be called before creating the linphone core.
 */
LINPHONE_PUBLIC void linphone_core_serialize_logs(void);

/**
 * Returns liblinphone's version as a string.
**/
LINPHONE_PUBLIC	const char *linphone_core_get_version(void);

/**
 * @return liblinphone's user agent as a string.
**/
LINPHONE_PUBLIC	const char *linphone_core_get_user_agent(LinphoneCore *lc);
/**
 * @deprecated Use #linphone_core_get_user_agent instead.
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED const char *linphone_core_get_user_agent_name(void);
/**
 * @deprecated Use #linphone_core_get_user_agent instead.
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED const char *linphone_core_get_user_agent_version(void);

/**
 * @}
**/

/**
 * Instanciates a LinphoneCore object.
 * @ingroup initializing
 *
 * The LinphoneCore object is the primary handle for doing all phone actions.
 * It should be unique within your application.
 * @param vtable a LinphoneCoreVTable structure holding your application callbacks
 * @param config_path a path to a config file. If it does not exists it will be created.
 *        The config file is used to store all settings, call logs, friends, proxies... so that all these settings
 *	       become persistent over the life of the LinphoneCore object.
 *	       It is allowed to set a NULL config file. In that case LinphoneCore will not store any settings.
 * @param factory_config_path a path to a read-only config file that can be used to
 *        to store hard-coded preference such as proxy settings or internal preferences.
 *        The settings in this factory file always override the one in the normal config file.
 *        It is OPTIONAL, use NULL if unneeded.
 * @param userdata an opaque user pointer that can be retrieved at any time (for example in
 *        callbacks) using linphone_core_get_user_data().
 * @see linphone_core_new_with_config
 * @deprecated Use linphone_factory_create_core() instead.
**/
LINPHONE_DEPRECATED LINPHONE_PUBLIC LinphoneCore *linphone_core_new(const LinphoneCoreVTable *vtable,
						const char *config_path, const char *factory_config_path, void* userdata);

/**
 * Instantiates a LinphoneCore object with a given LpConfig.
 * @ingroup initializing
 *
 * The LinphoneCore object is the primary handle for doing all phone actions.
 * It should be unique within your application.
 * @param vtable a LinphoneCoreVTable structure holding your application callbacks
 * @param config a pointer to an LpConfig object holding the configuration of the LinphoneCore to be instantiated.
 * @param userdata an opaque user pointer that can be retrieved at any time (for example in
 *        callbacks) using linphone_core_get_user_data().
 * @see linphone_core_new
 * @deprecated Use linphone_factory_create_core_with_config() instead.
**/
LINPHONE_DEPRECATED LINPHONE_PUBLIC LinphoneCore *linphone_core_new_with_config(const LinphoneCoreVTable *vtable, LpConfig *config, void *userdata);

/**
 * Increment the reference counter of a #LinphoneCore object.
 * @param lc The #LinphoneCore which the ref counter is to be incremented.
 * @return A pointer on the #LinphoneCore passed as parameter.
 * @ingroup initializing
 */
LINPHONE_PUBLIC LinphoneCore *linphone_core_ref(LinphoneCore *lc);

/**
 * Decrement the ref counter of a #LinphoneCore object and destroy it
 * if the counter reach 0.
 * @param lc The #LinphoneCore which the reference counter is to be decreased.
 * @ingroup initializing
 */
LINPHONE_PUBLIC void linphone_core_unref(LinphoneCore *lc);

/**
 * Main loop function. It is crucial that your application call it periodically.
 *  - It performs various backgrounds tasks:
 *  - receiving of SIP messages
 *  - handles timers and timeout
 *  - performs registration to proxies
 *  - authentication retries
 * The application MUST call this function periodically, in its main loop.
 *
 * Be careful that this function must be called from the same thread as
 * other liblinphone methods. If it is not the case make sure all liblinphone calls are
 * serialized with a mutex.
 *
 * For ICE to work properly it should be called every 20ms.
 * @ingroup initializing
 */
LINPHONE_PUBLIC	void linphone_core_iterate(LinphoneCore *lc);

/**
 * @ingroup initializing
 * add a listener to be notified of linphone core events. Once events are received, registered vtable are invoked in order.
 * @param vtable a LinphoneCoreVTable structure holding your application callbacks. Object is owned by linphone core until linphone_core_remove_listener.
 * @param lc object
 * @deprecated Use linphone_core_add_callbacks() instead.
 */
LINPHONE_DEPRECATED LINPHONE_PUBLIC void linphone_core_add_listener(LinphoneCore *lc, LinphoneCoreVTable *vtable);
/**
 * @ingroup initializing
 * Add a listener in order to be notified of #LinphoneCore events. Once an event is received, registred #LinphoneCoreCbs are sequencially
 * invoked in order.
 * @param lc The #LinphoneCore object to monitor.
 * @param cbs A #LinphoneCoreCbs object holding the callbakcs you need. A reference is take by #LinphoneCore until you invoke linphone_core_remove_cbs().
 */
LINPHONE_PUBLIC void linphone_core_add_callbacks(LinphoneCore *lc, LinphoneCoreCbs *cbs);
/**
 * @ingroup initializing
 * remove a listener registred by linphone_core_add_listener.
 * @param lc object
 * @param vtable a LinphoneCoreVTable structure holding your application callbacks.
 * @deprecated Use linphone_core_remove_callbacks() instead.
 */
LINPHONE_DEPRECATED LINPHONE_PUBLIC void linphone_core_remove_listener(LinphoneCore *lc, const LinphoneCoreVTable *vtable);
/**
 * @ingroup initializing
 * Remove a listener from a #LinphoneCore
 * @param lc The #LinphoneCore
 * @param cbs The pointer on the #LinphoneCoreCbs to remove.
 */
LINPHONE_PUBLIC void linphone_core_remove_callbacks(LinphoneCore *lc, const LinphoneCoreCbs *cbs);

/**
 * Sets the user agent string used in SIP messages.
 * You may ideally call that just after linphone_core_new() or linphone_core_init().
 * @ingroup misc
 */
LINPHONE_PUBLIC	void linphone_core_set_user_agent(LinphoneCore *lc, const char *ua_name, const char *version);

/**
 * See linphone_proxy_config_normalize_sip_uri for documentation. Default proxy config is used to parse
 * the address.
 * @ingroup misc
 */
LINPHONE_PUBLIC	LinphoneAddress * linphone_core_interpret_url(LinphoneCore *lc, const char *url);

/**
 * Initiates an outgoing call.
 * The application doesn't own a reference to the returned LinphoneCall object.
 * Use linphone_call_ref() to safely keep the #LinphoneCall pointer valid within your application.
 * @param lc the LinphoneCore object
 * @param url the destination of the call (sip address, or phone number).
 * @return a LinphoneCall object or NULL in case of failure
 * @ingroup call_control
 */
LINPHONE_PUBLIC	LinphoneCall * linphone_core_invite(LinphoneCore *lc, const char *url);

/**
 * Initiates an outgoing call given a destination #LinphoneAddress.
 *
 * The #LinphoneAddress can be constructed directly using linphone_address_new(), or
 * created by linphone_core_interpret_url().
 * The application doesn't own a reference to the returned LinphoneCall object.
 * Use linphone_call_ref() to safely keep the LinphoneCall pointer valid within your application.
 *
 * @param lc the LinphoneCore object
 * @param addr the destination of the call (sip address).
 * @return a #LinphoneCall object or NULL in case of failure
 * @ingroup call_control
 */
LINPHONE_PUBLIC	LinphoneCall * linphone_core_invite_address(LinphoneCore *lc, const LinphoneAddress *addr);

/**
 * Initiates an outgoing call according to supplied call parameters.
 *
 * The application doesn't own a reference to the returned LinphoneCall object.
 * Use linphone_call_ref() to safely keep the LinphoneCall pointer valid within your application.
 *
 * @ingroup call_control
 * @param lc the LinphoneCore object
 * @param url the destination of the call (sip address, or phone number).
 * @param params call parameters
 * @return a LinphoneCall object or NULL in case of failure
 */
LINPHONE_PUBLIC	LinphoneCall * linphone_core_invite_with_params(LinphoneCore *lc, const char *url, const LinphoneCallParams *params);

/**
 * Initiates an outgoing call given a destination LinphoneAddress
 *
 * @ingroup call_control
 * @param lc the LinphoneCore object
 * @param addr the destination of the call (sip address).
 * @param params call parameters
 *
 * The LinphoneAddress can be constructed directly using linphone_address_new(), or
 * created by linphone_core_interpret_url().
 * The application doesn't own a reference to the returned LinphoneCall object.
 * Use linphone_call_ref() to safely keep the LinphoneCall pointer valid within your application.
 *
 * @return a LinphoneCall object or NULL in case of failure
**/
LINPHONE_PUBLIC	LinphoneCall * linphone_core_invite_address_with_params(LinphoneCore *lc, const LinphoneAddress *addr, const LinphoneCallParams *params);

/**
 * Performs a simple call transfer to the specified destination.
 *
 * @ingroup call_control
 * The remote endpoint is expected to issue a new call to the specified destination.
 * The current call remains active and thus can be later paused or terminated.
 *
 * It is possible to follow the progress of the transfer provided that transferee sends notification about it.
 * In this case, the transfer_state_changed callback of the #LinphoneCoreVTable is invoked to notify of the state of the new call at the other party.
 * The notified states are #LinphoneCallOutgoingInit , #LinphoneCallOutgoingProgress, #LinphoneCallOutgoingRinging and #LinphoneCallConnected.
**/
LINPHONE_PUBLIC	int linphone_core_transfer_call(LinphoneCore *lc, LinphoneCall *call, const char *refer_to);

/**
 * Transfer a call to destination of another running call. This is used for "attended transfer" scenarios.
 * @param lc linphone core object
 * @param call a running call you want to transfer
 * @param dest a running call whose remote person will receive the transfer
 *
 * @ingroup call_control
 *
 * The transfered call is supposed to be in paused state, so that it is able to accept the transfer immediately.
 * The destination call is a call previously established to introduce the transfered person.
 * This method will send a transfer request to the transfered person. The phone of the transfered is then
 * expected to automatically call to the destination of the transfer. The receiver of the transfer will then automatically
 * close the call with us (the 'dest' call).
 *
 * It is possible to follow the progress of the transfer provided that transferee sends notification about it.
 * In this case, the transfer_state_changed callback of the #LinphoneCoreVTable is invoked to notify of the state of the new call at the other party.
 * The notified states are #LinphoneCallOutgoingInit , #LinphoneCallOutgoingProgress, #LinphoneCallOutgoingRinging and #LinphoneCallConnected.
 */
LINPHONE_PUBLIC	int linphone_core_transfer_call_to_another(LinphoneCore *lc, LinphoneCall *call, LinphoneCall *dest);

/**
 * Start a new call as a consequence of a transfer request received from a call.
 * This function is for advanced usage: the execution of transfers is automatically managed by the LinphoneCore. However if an application
 * wants to have control over the call parameters for the new call, it should call this function immediately during the LinphoneCallRefered notification.
 * @param lc the #LinphoneCore
 * @param call a call that has just been notified about LinphoneCallRefered state event.
 * @param params the call parameters to be applied to the new call.
 * @return a #LinphoneCall corresponding to the new call that is attempted to the transfer destination.
 * @see LinphoneCoreVTable::call_state_changed
 */
LINPHONE_PUBLIC LinphoneCall * linphone_core_start_refered_call(LinphoneCore *lc, LinphoneCall *call, const LinphoneCallParams *params);

/** @deprecated Use linphone_core_is_incoming_invite_pending() instead. */
#define linphone_core_inc_invite_pending(lc) linphone_core_is_incoming_invite_pending(lc)

/**
 * Tells whether there is an incoming invite pending.
 * @ingroup call_control
 * @param[in] lc LinphoneCore object
 * @return A boolean telling whether an incoming invite is pending or not.
 */
LINPHONE_PUBLIC	bool_t linphone_core_is_incoming_invite_pending(LinphoneCore*lc);

/**
 * Returns TRUE if there is a call running.
 *
 * @ingroup call_control
**/
LINPHONE_PUBLIC bool_t linphone_core_in_call(const LinphoneCore *lc);

/**
 * Returns The _LinphoneCall struct of the current call if one is in call
 *
 * @ingroup call_control
**/
LINPHONE_PUBLIC	LinphoneCall *linphone_core_get_current_call(const LinphoneCore *lc);

/**
 * Accept an incoming call.
 *
 * @ingroup call_control
 * Basically the application is notified of incoming calls within the
 * call_state_changed callback of the #LinphoneCoreVTable structure, where it will receive
 * a LinphoneCallIncoming event with the associated LinphoneCall object.
 * The application can later accept the call using this method.
 * @param lc the LinphoneCore object
 * @param call the LinphoneCall object representing the call to be answered.
 *
 */
LINPHONE_PUBLIC	int linphone_core_accept_call(LinphoneCore *lc, LinphoneCall *call);

/**
 * Accept an incoming call, with parameters.
 *
 * @ingroup call_control
 * Basically the application is notified of incoming calls within the
 * call_state_changed callback of the #LinphoneCoreVTable structure, where it will receive
 * a LinphoneCallIncoming event with the associated LinphoneCall object.
 * The application can later accept the call using
 * this method.
 * @param lc the LinphoneCore object
 * @param call the LinphoneCall object representing the call to be answered.
 * @param params the specific parameters for this call, for example whether video is accepted or not. Use NULL to use default parameters.
 *
 */
LINPHONE_PUBLIC	int linphone_core_accept_call_with_params(LinphoneCore *lc, LinphoneCall *call, const LinphoneCallParams *params);

/**
 * When receiving an incoming, accept to start a media session as early-media.
 * This means the call is not accepted but audio & video streams can be established if the remote party supports early media.
 * However, unlike after call acceptance, mic and camera input are not sent during early-media, though received audio & video are played normally.
 * The call can then later be fully accepted using linphone_core_accept_call() or linphone_core_accept_call_with_params().
 * @param lc the linphonecore
 * @param call the call
 * @param params the call params, can be NULL.
 * @return 0 if successful, -1 otherwise.
 * @ingroup call_control
 */
LINPHONE_PUBLIC int linphone_core_accept_early_media_with_params(LinphoneCore* lc, LinphoneCall* call, const LinphoneCallParams* params);

/**
 * Accept an early media session for an incoming call.
 * This is identical as calling linphone_core_accept_early_media_with_params() with NULL call parameters.
 * @see linphone_core_accept_early_media_with_params()
 * @param lc the core
 * @param call the incoming call
 * @return 0 if successful, -1 otherwise.
 * @ingroup call_control
 */
LINPHONE_PUBLIC int linphone_core_accept_early_media(LinphoneCore* lc, LinphoneCall* call);

/**
 * Terminates a call.
 *
 * @ingroup call_control
 * @param lc the LinphoneCore
 * @param the_call the LinphoneCall object representing the call to be terminated.
 */
LINPHONE_PUBLIC	int linphone_core_terminate_call(LinphoneCore *lc, LinphoneCall *call);

/**
 * Redirect the specified call to the given redirect URI.
 * @param[in] lc #LinphoneCore object.
 * @param[in] call The #LinphoneCall to redirect.
 * @param[in] redirect_uri The URI to redirect the call to.
 * @return 0 if successful, -1 on error.
 * @ingroup call_control
 */
LINPHONE_PUBLIC int linphone_core_redirect_call(LinphoneCore *lc, LinphoneCall *call, const char *redirect_uri);

/**
 * Decline a pending incoming call, with a reason.
 *
 * @ingroup call_control
 *
 * @param lc the linphone core
 * @param call the LinphoneCall, must be in the IncomingReceived state.
 * @param reason the reason for rejecting the call: LinphoneReasonDeclined or LinphoneReasonBusy
 */
LINPHONE_PUBLIC	int linphone_core_decline_call(LinphoneCore *lc, LinphoneCall * call, LinphoneReason reason);

/**
 * Terminates all the calls.
 *
 * @ingroup call_control
 * @param lc The LinphoneCore
 */
LINPHONE_PUBLIC	int linphone_core_terminate_all_calls(LinphoneCore *lc);

/**
 * Pauses the call. If a music file has been setup using linphone_core_set_play_file(),
 * this file will be played to the remote user.
 *
 * @ingroup call_control
**/
LINPHONE_PUBLIC	int linphone_core_pause_call(LinphoneCore *lc, LinphoneCall *call);

/**
 * Pause all currently running calls.
 * @ingroup call_control
**/
LINPHONE_PUBLIC	int linphone_core_pause_all_calls(LinphoneCore *lc);

/**
 * Resumes the call.
 *
 * @ingroup call_control
**/
LINPHONE_PUBLIC	int linphone_core_resume_call(LinphoneCore *lc, LinphoneCall *call);

/**
 * @ingroup call_control
 * Updates a running call according to supplied call parameters or parameters changed in the LinphoneCore.
 *
 * In this version this is limited to the following use cases:
 * - setting up/down the video stream according to the video parameter of the LinphoneCallParams (see linphone_call_params_enable_video() ).
 * - changing the size of the transmitted video after calling linphone_core_set_preferred_video_size()
 *
 * In case no changes are requested through the LinphoneCallParams argument, then this argument can be omitted and set to NULL.
 * @param lc the core
 * @param call the call to be updated
 * @param params the new call parameters to use. (may be NULL)
 * @return 0 if successful, -1 otherwise.
 */
LINPHONE_PUBLIC	int linphone_core_update_call(LinphoneCore *lc, LinphoneCall *call, const LinphoneCallParams *params);

/**
 * @ingroup call_control
 * When receiving a #LinphoneCallUpdatedByRemote state notification, prevent LinphoneCore from performing an automatic answer.
 *
 * When receiving a #LinphoneCallUpdatedByRemote state notification (ie an incoming reINVITE), the default behaviour of
 * LinphoneCore is defined by the "defer_update_default" option of the "sip" section of the config. If this option is 0 (the default)
 * then the LinphoneCore automatically answers the reINIVTE with call parameters unchanged.
 * However when for example when the remote party updated the call to propose a video stream, it can be useful
 * to prompt the user before answering. This can be achieved by calling linphone_core_defer_call_update() during
 * the call state notification, to deactivate the automatic answer that would just confirm the audio but reject the video.
 * Then, when the user responds to dialog prompt, it becomes possible to call linphone_core_accept_call_update() to answer
 * the reINVITE, with eventually video enabled in the LinphoneCallParams argument.
 *
 * The #LinphoneCallUpdatedByRemote notification can also arrive when receiving an INVITE without SDP. In such case, an unchanged offer is made
 * in the 200Ok, and when the ACK containing the SDP answer is received, #LinphoneCallUpdatedByRemote is triggered to notify the application of possible
 * changes in the media session. However in such case defering the update has no meaning since we just generating an offer.
 *
 * @return 0 if successful, -1 if the linphone_core_defer_call_update() was done outside a valid #LinphoneCallUpdatedByRemote notification.
 */
LINPHONE_PUBLIC	int linphone_core_defer_call_update(LinphoneCore *lc, LinphoneCall *call);

/**
 * @ingroup call_control
 * Accept call modifications initiated by other end.
 *
 * This call may be performed in response to a #LinphoneCallUpdatedByRemote state notification.
 * When such notification arrives, the application can decide to call linphone_core_defer_update_call() so that it can
 * have the time to prompt the user. linphone_call_get_remote_params() can be used to get information about the call parameters
 * requested by the other party, such as whether a video stream is requested.
 *
 * When the user accepts or refuse the change, linphone_core_accept_call_update() can be done to answer to the other party.
 * If params is NULL, then the same call parameters established before the update request will continue to be used (no change).
 * If params is not NULL, then the update will be accepted according to the parameters passed.
 * Typical example is when a user accepts to start video, then params should indicate that video stream should be used
 * (see linphone_call_params_enable_video()).
 * @param lc the linphone core object.
 * @param call the LinphoneCall object
 * @param params a LinphoneCallParams object describing the call parameters to accept.
 * @return 0 if successful, -1 otherwise (actually when this function call is performed outside ot #LinphoneCallUpdatedByRemote state).
 */
LINPHONE_PUBLIC	int linphone_core_accept_call_update(LinphoneCore *lc, LinphoneCall *call, const LinphoneCallParams *params);

/**
 * Create a LinphoneCallParams suitable for linphone_core_invite_with_params(), linphone_core_accept_call_with_params(), linphone_core_accept_early_media_with_params(),
 * linphone_core_accept_call_update().
 * The parameters are initialized according to the current LinphoneCore configuration and the current state of the LinphoneCall.
 * @param lc the LinphoneCore
 * @param call the call for which the parameters are to be build, or NULL in the case where the parameters are to be used for a new outgoing call.
 * @return a new LinphoneCallParams
 * @ingroup call_control
 */
LINPHONE_PUBLIC LinphoneCallParams *linphone_core_create_call_params(LinphoneCore *lc, LinphoneCall *call);

/**
 * Get the call with the remote_address specified
 * @return the LinphoneCall of the call if found
 *
 * @ingroup call_control
 */
LINPHONE_PUBLIC	LinphoneCall *linphone_core_get_call_by_remote_address(LinphoneCore *lc, const char *remote_address);

/**
 * Get the call with the remote_address specified
 * @return the #LinphoneCall of the call if found
 *
 * @ingroup call_control
 */
LINPHONE_PUBLIC	LinphoneCall *linphone_core_get_call_by_remote_address2(LinphoneCore *lc, const LinphoneAddress *remote_address);


/**
 * Send the specified dtmf.
 *
 * @ingroup media_parameters
 * @deprecated Use #linphone_call_send_dtmf instead.
 * This function only works during calls. The dtmf is automatically played to the user.
 * @param lc The LinphoneCore object
 * @param dtmf The dtmf name specified as a char, such as '0', '#' etc...
 *
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED void linphone_core_send_dtmf(LinphoneCore *lc,char dtmf);

/**
 * Sets the local "from" identity.
 *
 * @ingroup proxies
 * This data is used in absence of any proxy configuration or when no
 * default proxy configuration is set. See LinphoneProxyConfig
**/
LINPHONE_PUBLIC	int linphone_core_set_primary_contact(LinphoneCore *lc, const char *contact);

/**
 * Returns the default identity when no proxy configuration is used.
 *
 * @ingroup proxies
**/
LINPHONE_PUBLIC	const char *linphone_core_get_primary_contact(LinphoneCore *lc);

/**
 * Returns the default identity SIP address as a string.
 *
 * If no default proxy is set, this will return the primary contact (
 * see linphone_core_get_primary_contact() ). If a default proxy is set
 * it returns the registered identity on the proxy.
 * @ingroup proxies
 */
LINPHONE_PUBLIC	const char * linphone_core_get_identity(LinphoneCore *lc);

/**
 * Tells LinphoneCore to guess local hostname automatically in primary contact.
 *
 * @ingroup proxies
**/
LINPHONE_PUBLIC void linphone_core_set_guess_hostname(LinphoneCore *lc, bool_t val);
/**
 * Returns TRUE if hostname part of primary contact is guessed automatically.
 *
 * @ingroup proxies
**/
LINPHONE_PUBLIC bool_t linphone_core_get_guess_hostname(LinphoneCore *lc);

/**
 * Tells to LinphoneCore to use Linphone Instant Messaging encryption
 *
 */
LINPHONE_PUBLIC void linphone_core_enable_lime(LinphoneCore *lc, LinphoneLimeState val);
LINPHONE_PUBLIC LinphoneLimeState linphone_core_lime_enabled(const LinphoneCore *lc);
LINPHONE_PUBLIC bool_t linphone_core_lime_available(const LinphoneCore *lc);

/**
 * Returns TRUE if IPv6 is enabled.
 * See linphone_core_enable_ipv6() for more details on how IPv6 is supported in liblinphone.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	bool_t linphone_core_ipv6_enabled(LinphoneCore *lc);

/**
 * Turns IPv6 support on or off.
 * @ingroup network_parameter.
 */
LINPHONE_PUBLIC	void linphone_core_enable_ipv6(LinphoneCore *lc, bool_t val);

/**
 * Same as linphone_core_get_primary_contact() but the result is a LinphoneAddress object
 * instead of const char*
 *
 * @ingroup proxies
**/
LINPHONE_PUBLIC	LinphoneAddress *linphone_core_get_primary_contact_parsed(LinphoneCore *lc);
LINPHONE_PUBLIC	const char * linphone_core_get_identity(LinphoneCore *lc);
/**
 * Sets maximum available download bandwidth
 * This is IP bandwidth, in kbit/s.
 * This information is used signaled to other parties during
 * calls (within SDP messages) so that the remote end can have
 * sufficient knowledge to properly configure its audio & video
 * codec output bitrate to not overflow available bandwidth.
 *
 * @ingroup media_parameters
 *
 * @param lc the LinphoneCore object
 * @param bw the bandwidth in kbits/s, 0 for infinite
 */
LINPHONE_PUBLIC	void linphone_core_set_download_bandwidth(LinphoneCore *lc, int bw);
/**
 * Sets maximum available upload bandwidth
 * This is IP bandwidth, in kbit/s.
 * This information is used by liblinphone together with remote
 * side available bandwidth signaled in SDP messages to properly
 * configure audio & video codec's output bitrate.
 *
 * @param lc the LinphoneCore object
 * @param bw the bandwidth in kbits/s, 0 for infinite
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_upload_bandwidth(LinphoneCore *lc, int bw);

/**
 * Retrieve the maximum available download bandwidth.
 * This value was set by linphone_core_set_download_bandwidth().
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	int linphone_core_get_download_bandwidth(const LinphoneCore *lc);
/**
 * Retrieve the maximum available upload bandwidth.
 * This value was set by linphone_core_set_upload_bandwidth().
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	int linphone_core_get_upload_bandwidth(const LinphoneCore *lc);

/**
 * Enable adaptive rate control.
 *
 * @ingroup media_parameters
 *
 * Adaptive rate control consists in using RTCP feedback provided information to dynamically
 * control the output bitrate of the audio and video encoders, so that we can adapt to the network conditions and
 * available bandwidth. Control of the audio encoder is done in case of audio-only call, and control of the video encoder is done for audio & video calls.
 * Adaptive rate control feature is enabled by default.
**/
LINPHONE_PUBLIC void linphone_core_enable_adaptive_rate_control(LinphoneCore *lc, bool_t enabled);

/**
 * Returns whether adaptive rate control is enabled.
 *
 * @ingroup media_parameters
 *
 * See linphone_core_enable_adaptive_rate_control().
**/
LINPHONE_PUBLIC bool_t linphone_core_adaptive_rate_control_enabled(const LinphoneCore *lc);

/**
 * Sets adaptive rate algorithm. It will be used for each new calls starting from
 * now. Calls already started will not be updated.
 *
 * @ingroup media_parameters
 *
**/
LINPHONE_PUBLIC void linphone_core_set_adaptive_rate_algorithm(LinphoneCore *lc, const char *algorithm);
/**
 * Returns which adaptive rate algorithm is currently configured for future calls.
 *
 * @ingroup media_parameters
 *
 * See linphone_core_set_adaptive_rate_algorithm().
**/
LINPHONE_PUBLIC const char* linphone_core_get_adaptive_rate_algorithm(const LinphoneCore *lc);

/**
 * Set audio packetization time linphone expects to receive from peer.
 * A value of zero means that ptime is not specified.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_download_ptime(LinphoneCore *lc, int ptime);
/**
 * Get audio packetization time linphone expects to receive from peer.
 * A value of zero means that ptime is not specified.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	int  linphone_core_get_download_ptime(LinphoneCore *lc);

/**
 * Set audio packetization time linphone will send (in absence of requirement from peer)
 * A value of 0 stands for the current codec default packetization time.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_core_set_upload_ptime(LinphoneCore *lc, int ptime);

/**
 * Set audio packetization time linphone will send (in absence of requirement from peer)
 * A value of 0 stands for the current codec default packetization time.
 *
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	int linphone_core_get_upload_ptime(LinphoneCore *lc);

/**
 * Set the SIP transport timeout.
 * @param[in] lc #LinphoneCore object.
 * @param[in] timeout_ms The SIP transport timeout in milliseconds.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_sip_transport_timeout(LinphoneCore *lc, int timeout_ms);

/**
 * Get the SIP transport timeout.
 * @param[in] lc #LinphoneCore object.
 * @return The SIP transport timeout in milliseconds.
 * @ingroup media_parameters
 */
int linphone_core_get_sip_transport_timeout(LinphoneCore *lc);

/**
 * Enable or disable DNS SRV resolution.
 * @param[in] lc #LinphoneCore object.
 * @param[in] enable TRUE to enable DNS SRV resolution, FALSE to disable it.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC void linphone_core_enable_dns_srv(LinphoneCore *lc, bool_t enable);

/**
 * Tells whether DNS SRV resolution is enabled.
 * @param[in] lc #LinphoneCore object.
 * @return TRUE if DNS SRV resolution is enabled, FALSE if disabled.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_dns_srv_enabled(const LinphoneCore *lc);

/**
 * Enable or disable DNS search (use of local domain if the fully qualified name did return results).
 * @param[in] lc #LinphoneCore object.
 * @param[in] enable TRUE to enable DNS search, FALSE to disable it.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC void linphone_core_enable_dns_search(LinphoneCore *lc, bool_t enable);

/**
 * Tells whether DNS search (use of local domain if the fully qualified name did return results) is enabled.
 * @param[in] lc #LinphoneCore object.
 * @return TRUE if DNS search is enabled, FALSE if disabled.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_dns_search_enabled(const LinphoneCore *lc);

/**
 * Forces liblinphone to use the supplied list of dns servers, instead of system's ones.
 * @param[in] lc #LinphoneCore object.
 * @param[in] servers A #bctbx_list_t of strings containing the IP addresses of DNS servers to be used.
 * Setting to NULL restores default behaviour, which is to use the DNS server list provided by the system.
 * The list is copied internally.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC void linphone_core_set_dns_servers(LinphoneCore *lc, const bctbx_list_t *servers);

/**
 * Returns the list of available audio codecs.
 * @param[in] lc The LinphoneCore object
 * @return \bctbx_list{PayloadType}
 *
 * This list is unmodifiable. The ->data field of the bctbx_list_t points a PayloadType
 * structure holding the codec information.
 * It is possible to make copy of the list with bctbx_list_copy() in order to modify it
 * (such as the order of codecs).
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	const bctbx_list_t *linphone_core_get_audio_codecs(const LinphoneCore *lc);

/**
 * Sets the list of audio codecs.
 * The list is taken by the LinphoneCore thus the application should not free it.
 * This list is made of struct PayloadType describing the codec parameters.
 * @param[in] lc The #LinphoneCore object.
 * @param[in] codecs \bctbx_list{PayloadType}
 * @return 0 whatever has happened.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC int linphone_core_set_audio_codecs(LinphoneCore *lc, bctbx_list_t *codecs);

/**
 * Returns the list of available video codecs.
 * @param[in] lc The LinphoneCore object
 * @return \bctbx_list{PayloadType}
 *
 * This list is unmodifiable. The ->data field of the bctbx_list_t points a PayloadType
 * structure holding the codec information.
 * It is possible to make copy of the list with bctbx_list_copy() in order to modify it
 * (such as the order of codecs).
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const bctbx_list_t *linphone_core_get_video_codecs(const LinphoneCore *lc);

/**
 * Sets the list of video codecs.
 * The list is taken by the #LinphoneCore thus the application should not free it.
 * This list is made of struct PayloadType describing the codec parameters.
 * @param[in] lc The LinphoneCore object
 * @param[in] codecs \bctbx_list{PayloadType}
 * @return 0 whatever has happened.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC int linphone_core_set_video_codecs(LinphoneCore *lc, bctbx_list_t *codecs);

/**
 * Returns the list of available text codecs.
 * @param[in] lc The LinphoneCore object
 * @return \bctbx_list{PayloadType}
 *
 * This list is unmodifiable. The ->data field of the bctbx_list_t points a PayloadType
 * structure holding the codec information.
 * It is possible to make copy of the list with bctbx_list_copy() in order to modify it
 * (such as the order of codecs).
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const bctbx_list_t *linphone_core_get_text_codecs(const LinphoneCore *lc);


LINPHONE_PUBLIC int linphone_core_set_text_codecs(LinphoneCore *lc, bctbx_list_t *codecs);

/**
 * Enable RFC3389 generic confort noise algorithm (CN payload type).
 * It is disabled by default, because this algorithm is only relevant for legacy codecs (PCMU, PCMA, G722).
 * @param lc the LinphoneCore
 * @param enabled TRUE if enabled, FALSE otherwise.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC void linphone_core_enable_generic_confort_noise(LinphoneCore *lc, bool_t enabled);

/**
 * Returns enablement of RFC3389 generic confort noise algorithm.
 * @param lc the LinphoneCore
 * @return TRUE or FALSE.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_generic_confort_noise_enabled(const LinphoneCore *lc);

/**
 * Tells whether the specified payload type is enabled.
 * @param[in] lc #LinphoneCore object.
 * @param[in] pt The #LinphonePayloadType we want to know is enabled or not.
 * @return TRUE if the payload type is enabled, FALSE if disabled.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_payload_type_enabled(LinphoneCore *lc, const LinphonePayloadType *pt);

/**
 * Tells whether the specified payload type represents a variable bitrate codec.
 * @param[in] lc #LinphoneCore object.
 * @param[in] pt The #LinphonePayloadType we want to know
 * @return TRUE if the payload type represents a VBR codec, FALSE if disabled.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_payload_type_is_vbr(LinphoneCore *lc, const LinphonePayloadType *pt);

/**
 * Set an explicit bitrate (IP bitrate, not codec bitrate) for a given codec, in kbit/s.
 * @param[in] lc the #LinphoneCore object
 * @param[in] pt the #LinphonePayloadType to modify.
 * @param[in] bitrate the IP bitrate in kbit/s.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_payload_type_bitrate(LinphoneCore *lc, LinphonePayloadType *pt, int bitrate);

/**
 * Get the bitrate explicitely set with linphone_core_set_payload_type_bitrate().
 * @param[in] lc the #LinphoneCore object
 * @param[in] pt the #LinphonePayloadType to modify.
 * @return bitrate the IP bitrate in kbit/s, or -1 if an error occured.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_get_payload_type_bitrate(LinphoneCore *lc, const LinphonePayloadType *pt);

/**
 * Enable or disable the use of the specified payload type.
 * @param[in] lc #LinphoneCore object.
 * @param[in] pt The #LinphonePayloadType to enable or disable. It can be retrieved using #linphone_core_find_payload_type
 * @param[in] enable TRUE to enable the payload type, FALSE to disable it.
 * @return 0 if successful, any other value otherwise.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	int linphone_core_enable_payload_type(LinphoneCore *lc, LinphonePayloadType *pt, bool_t enable);

/**
 * Wildcard value used by #linphone_core_find_payload_type to ignore rate in search algorithm
 * @ingroup media_parameters
 */
#define LINPHONE_FIND_PAYLOAD_IGNORE_RATE -1
/**
 * Wildcard value used by #linphone_core_find_payload_type to ignore channel in search algorithm
 * @ingroup media_parameters
 */
#define LINPHONE_FIND_PAYLOAD_IGNORE_CHANNELS -1
/**
 * Get payload type from mime type and clock rate
 * @ingroup media_parameters
 * This function searches in audio and video codecs for the given payload type name and clockrate.
 * @param lc #LinphoneCore object
 * @param type payload mime type (I.E SPEEX, PCMU, VP8)
 * @param rate can be #LINPHONE_FIND_PAYLOAD_IGNORE_RATE
 * @param channels  number of channels, can be #LINPHONE_FIND_PAYLOAD_IGNORE_CHANNELS
 * @return Returns NULL if not found.
 */
LINPHONE_PUBLIC	LinphonePayloadType* linphone_core_find_payload_type(LinphoneCore* lc, const char* type, int rate, int channels) ;

/**
 * @ingroup media_parameters
 * Returns the payload type number assigned for this codec.
**/
LINPHONE_PUBLIC	int linphone_core_get_payload_type_number(LinphoneCore *lc, const PayloadType *pt);

/**
 * @ingroup media_parameters
 * Force a number for a payload type. The LinphoneCore does payload type number assignment automatically. THis function is to be used mainly for tests, in order
 * to override the automatic assignment mechanism.
**/
LINPHONE_PUBLIC void linphone_core_set_payload_type_number(LinphoneCore *lc, PayloadType *pt, int number);

LINPHONE_PUBLIC const char *linphone_core_get_payload_type_description(LinphoneCore *lc, PayloadType *pt);

/**
 * Return TRUE if codec can be used with bandwidth, FALSE else
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_check_payload_type_usability(LinphoneCore *lc, const PayloadType *pt);

/**
 * @addtogroup proxies
 * @{
 */

/**
 * Create a proxy config with default values from Linphone core.
 * @param[in] lc #LinphoneCore object
 * @return #LinphoneProxyConfig with default values set
 */
LINPHONE_PUBLIC LinphoneProxyConfig * linphone_core_create_proxy_config(LinphoneCore *lc);

/**
 * Add a proxy configuration.
 * This will start registration on the proxy, if registration is enabled.
**/
LINPHONE_PUBLIC int linphone_core_add_proxy_config(LinphoneCore *lc, LinphoneProxyConfig *config);

/**
 * Erase all proxies from config.
**/
LINPHONE_PUBLIC void linphone_core_clear_proxy_config(LinphoneCore *lc);

/**
 * Removes a proxy configuration.
 *
 * LinphoneCore will then automatically unregister and place the proxy configuration
 * on a deleted list. For that reason, a removed proxy does NOT need to be freed.
**/
LINPHONE_PUBLIC void linphone_core_remove_proxy_config(LinphoneCore *lc, LinphoneProxyConfig *config);

/**
 * Returns an unmodifiable list of entered proxy configurations.
 * @param[in] lc The LinphoneCore object
 * @return \bctbx_list{LinphoneProxyConfig}
**/
LINPHONE_PUBLIC const bctbx_list_t *linphone_core_get_proxy_config_list(const LinphoneCore *lc);

/** @deprecated Use linphone_core_set_default_proxy_config() instead. */
#define linphone_core_set_default_proxy(lc, config) linphone_core_set_default_proxy_config(lc, config)

LINPHONE_PUBLIC void linphone_core_set_default_proxy_index(LinphoneCore *lc, int index);

/**
 * @return the default proxy configuration, that is the one used to determine the current identity.
 * @deprecated Use linphone_core_get_default_proxy_config() instead.
**/
LINPHONE_PUBLIC LINPHONE_DEPRECATED int linphone_core_get_default_proxy(LinphoneCore *lc, LinphoneProxyConfig **config);

/**
 * @return the default proxy configuration, that is the one used to determine the current identity.
 * @param[in] lc LinphoneCore object
 * @return The default proxy configuration.
**/
LINPHONE_PUBLIC LinphoneProxyConfig * linphone_core_get_default_proxy_config(LinphoneCore *lc);

/**
 * Sets the default proxy.
 *
 * This default proxy must be part of the list of already entered LinphoneProxyConfig.
 * Toggling it as default will make LinphoneCore use the identity associated with
 * the proxy configuration in all incoming and outgoing calls.
 * @param[in] lc LinphoneCore object
 * @param[in] config The proxy configuration to use as the default one.
**/
LINPHONE_PUBLIC void linphone_core_set_default_proxy_config(LinphoneCore *lc, LinphoneProxyConfig *config);

/**
 * @}
 */

/**
 * Create an authentication information with default values from Linphone core.
 * @param[in] lc #LinphoneCore object
 * @param[in] username String containing the username part of the authentication credentials
 * @param[in] userid String containing the username to use to calculate the authentication digest (optional)
 * @param[in] passwd String containing the password of the authentication credentials (optional, either passwd or ha1 must be set)
 * @param[in] ha1 String containing a ha1 hash of the password (optional, either passwd or ha1 must be set)
 * @param[in] realm String used to discriminate different SIP authentication domains (optional)
 * @param[in] domain String containing the SIP domain for which this authentication information is valid, if it has to be restricted for a single SIP domain.
 * @return #LinphoneAuthInfo with default values set
 * @ingroup authentication
 */
LINPHONE_PUBLIC LinphoneAuthInfo * linphone_core_create_auth_info(LinphoneCore *lc, const char *username, const char *userid, const char *passwd, const char *ha1, const char *realm, const char *domain);

/**
 * Adds authentication information to the #LinphoneCore.
 * That piece of information will be used during all SIP transactions that require authentication.
 * @param[in] lc The #LinphoneCore.
 * @param[in] info The #LinphoneAuthInfo to add.
 * @ingroup authentication
 */
LINPHONE_PUBLIC	void linphone_core_add_auth_info(LinphoneCore *lc, const LinphoneAuthInfo *info);

/**
 * Removes an authentication information object.
 * @param[in] lc The #LinphoneCore from which the #LinphoneAuthInfo will be removed.
 * @param[in] info The #LinphoneAuthInfo to remove.
 * @ingroup authentication
 */
LINPHONE_PUBLIC void linphone_core_remove_auth_info(LinphoneCore *lc, const LinphoneAuthInfo *info);

/**
 * Returns an unmodifiable list of currently entered #LinphoneAuthInfo.
 * @param[in] lc The LinphoneCore object.
 * @return \bctbx_list{LinphoneAuthInfo}
 * @ingroup authentication
 */
LINPHONE_PUBLIC const bctbx_list_t *linphone_core_get_auth_info_list(const LinphoneCore *lc);

/**
 * Find authentication info matching realm, username, domain criteria.
 * First of all, (realm,username) pair are searched. If multiple results (which should not happen because realm are supposed to be unique), then domain is added to the search.
 * @param lc the LinphoneCore
 * @param realm the authentication 'realm' (optional)
 * @param username the SIP username to be authenticated (mandatory)
 * @param sip_domain the SIP domain name (optional)
 * @return a #LinphoneAuthInfo
 * @ingroup authentication
**/
LINPHONE_PUBLIC const LinphoneAuthInfo *linphone_core_find_auth_info(LinphoneCore *lc, const char *realm, const char *username, const char *sip_domain);

/**
 * This method is used to abort a user authentication request initiated by LinphoneCore
 * from the auth_info_requested callback of LinphoneCoreVTable.
 * @note That function does nothing for now.
**/
LINPHONE_PUBLIC void linphone_core_abort_authentication(LinphoneCore *lc,  LinphoneAuthInfo *info);

/**
 * Clear all authentication information.
**/
LINPHONE_PUBLIC	void linphone_core_clear_all_auth_info(LinphoneCore *lc);

/**
 * Enable or disable the audio adaptive jitter compensation.
 * @param[in] lc #LinphoneCore object
 * @param[in] enable TRUE to enable the audio adaptive jitter compensation, FALSE to disable it.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC void linphone_core_enable_audio_adaptive_jittcomp(LinphoneCore *lc, bool_t enable);

/**
 * Tells whether the audio adaptive jitter compensation is enabled.
 * @param[in] lc #LinphoneCore object
 * @return TRUE if the audio adaptive jitter compensation is enabled, FALSE otherwise.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_audio_adaptive_jittcomp_enabled(LinphoneCore *lc);

/**
 * Returns the nominal audio jitter buffer size in milliseconds.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC int linphone_core_get_audio_jittcomp(LinphoneCore *lc);

/**
 * Sets the nominal audio jitter buffer size in milliseconds.
 * The value takes effect immediately for all running and pending calls, if any.
 * A value of 0 disables the jitter buffer.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_audio_jittcomp(LinphoneCore *lc, int milliseconds);

/**
 * Enable or disable the video adaptive jitter compensation.
 * @param[in] lc #LinphoneCore object
 * @param[in] enable TRUE to enable the video adaptive jitter compensation, FALSE to disable it.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC void linphone_core_enable_video_adaptive_jittcomp(LinphoneCore *lc, bool_t enable);

/**
 * Tells whether the video adaptive jitter compensation is enabled.
 * @param[in] lc #LinphoneCore object
 * @return TRUE if the video adaptive jitter compensation is enabled, FALSE otherwise.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_video_adaptive_jittcomp_enabled(LinphoneCore *lc);

/**
 * Returns the nominal video jitter buffer size in milliseconds.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC int linphone_core_get_video_jittcomp(LinphoneCore *lc);

/**
 * Sets the nominal video jitter buffer size in milliseconds.
 * The value takes effect immediately for all running and pending calls, if any.
 * A value of 0 disables the jitter buffer.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_video_jittcomp(LinphoneCore *lc, int milliseconds);

/**
 * Returns the UDP port used for audio streaming.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	int linphone_core_get_audio_port(const LinphoneCore *lc);

/**
 * Get the audio port range from which is randomly chosen the UDP port used for audio streaming.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	void linphone_core_get_audio_port_range(const LinphoneCore *lc, int *min_port, int *max_port);

/**
 * Returns the UDP port used for video streaming.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	int linphone_core_get_video_port(const LinphoneCore *lc);

/**
 * Get the video port range from which is randomly chosen the UDP port used for video streaming.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	void linphone_core_get_video_port_range(const LinphoneCore *lc, int *min_port, int *max_port);

/**
 * Returns the UDP port used for text streaming.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	int linphone_core_get_text_port(const LinphoneCore *lc);

/**
 * Get the video port range from which is randomly chosen the UDP port used for text streaming.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	void linphone_core_get_text_port_range(const LinphoneCore *lc, int *min_port, int *max_port);

/**
 * Returns the value in seconds of the no-rtp timeout.
 * When no RTP or RTCP packets have been received for a while
 * LinphoneCore will consider the call is broken (remote end crashed or
 * disconnected from the network), and thus will terminate the call.
 * The no-rtp timeout is the duration above which the call is considered broken.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	int linphone_core_get_nortp_timeout(const LinphoneCore *lc);

/**
 * Sets the UDP port used for audio streaming.
 * A value if -1 will request the system to allocate the local port randomly.
 * This is recommended in order to avoid firewall warnings.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_audio_port(LinphoneCore *lc, int port);

/**
 * Sets the UDP port range from which to randomly select the port used for audio streaming.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_audio_port_range(LinphoneCore *lc, int min_port, int max_port);

/**
 * Sets the UDP port used for video streaming.
 * A value if -1 will request the system to allocate the local port randomly.
 * This is recommended in order to avoid firewall warnings.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_video_port(LinphoneCore *lc, int port);

/**
 * Sets the UDP port range from which to randomly select the port used for video streaming.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_video_port_range(LinphoneCore *lc, int min_port, int max_port);

/**
 * Sets the UDP port used for text streaming.
 * A value if -1 will request the system to allocate the local port randomly.
 * This is recommended in order to avoid firewall warnings.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_text_port(LinphoneCore *lc, int port);

/**
 * Sets the UDP port range from which to randomly select the port used for text streaming.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_text_port_range(LinphoneCore *lc, int min_port, int max_port);

/**
 * Sets the no-rtp timeout value in seconds.
 * See linphone_core_get_nortp_timeout() for details.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_nortp_timeout(LinphoneCore *lc, int seconds);

/**
 * Sets whether SIP INFO is to be used for sending digits.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_use_info_for_dtmf(LinphoneCore *lc, bool_t use_info);

/**
 * Indicates whether SIP INFO is used for sending digits.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	bool_t linphone_core_get_use_info_for_dtmf(LinphoneCore *lc);

/**
 * Sets whether RFC2833 is to be used for sending digits.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_use_rfc2833_for_dtmf(LinphoneCore *lc,bool_t use_rfc2833);

/**
 * Indicates whether RFC2833 is used for sending digits.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	bool_t linphone_core_get_use_rfc2833_for_dtmf(LinphoneCore *lc);

/**
 * Sets the UDP port to be used by SIP.
 * @deprecated use linphone_core_set_sip_transports() instead.
 * @ingroup network_parameters
 */
LINPHONE_DEPRECATED LINPHONE_PUBLIC	void linphone_core_set_sip_port(LinphoneCore *lc, int port);

/**
 * Returns the UDP port used by SIP.
 * @deprecated use linphone_core_get_sip_transports() instead.
 * @ingroup network_parameters
 */
LINPHONE_DEPRECATED LINPHONE_PUBLIC	int linphone_core_get_sip_port(LinphoneCore *lc);

/**
 * Sets the ports to be used for each of transport (UDP or TCP).
 * A zero value port for a given transport means the transport is not used.
 * A value of LC_SIP_TRANSPORT_RANDOM (-1) means the port is to be choosen randomly by the system.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	int linphone_core_set_sip_transports(LinphoneCore *lc, const LinphoneSipTransports *transports);

/**
 * Retrieves the port configuration used for each transport (udp, tcp, tls).
 * A zero value port for a given transport means the transport
 * is not used. A value of LC_SIP_TRANSPORT_RANDOM (-1) means the port is to be chosen randomly by the system.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	int linphone_core_get_sip_transports(LinphoneCore *lc, LinphoneSipTransports *transports);

/**
 * Retrieves the real port number assigned for each sip transport (udp, tcp, tls).
 * A zero value means that the transport is not activated.
 * If LC_SIP_TRANSPORT_RANDOM was passed to linphone_core_set_sip_transports(), the random port choosed by the system is returned.
 * @ingroup network_parameters
 * @param lc the LinphoneCore
 * @param tr a LCSipTransports structure.
 */
LINPHONE_PUBLIC void linphone_core_get_sip_transports_used(LinphoneCore *lc, LinphoneSipTransports *tr);

/**
 * Returns TRUE if given transport type is supported by the library, FALSE otherwise.
 */
LINPHONE_PUBLIC	bool_t linphone_core_sip_transport_supported(const LinphoneCore *lc, LinphoneTransportType tp);

LINPHONE_PUBLIC bool_t linphone_core_content_encoding_supported(const LinphoneCore *lc, const char *content_encoding);

/**
 *
 * Give access to the UDP sip socket. Can be useful to configure this socket as persistent I.E kCFStreamNetworkServiceType set to kCFStreamNetworkServiceTypeVoIP)
 * @param lc #LinphoneCore
 * @return socket file descriptor
 */
ortp_socket_t linphone_core_get_sip_socket(LinphoneCore *lc);

/**
 * Set the incoming call timeout in seconds.
 *
 * @ingroup call_control
 * If an incoming call isn't answered for this timeout period, it is
 * automatically declined.
**/
LINPHONE_PUBLIC	void linphone_core_set_inc_timeout(LinphoneCore *lc, int seconds);

/**
 * Returns the incoming call timeout
 *
 * @ingroup call_control
 * See linphone_core_set_inc_timeout() for details.
**/
LINPHONE_PUBLIC	int linphone_core_get_inc_timeout(LinphoneCore *lc);

/**
 * Set the in call timeout in seconds.
 *
 * @ingroup call_control
 * After this timeout period, the call is automatically hangup.
**/
LINPHONE_PUBLIC	void linphone_core_set_in_call_timeout(LinphoneCore *lc, int seconds);

/**
 * Returns the in call timeout
 *
 * @ingroup call_control
 * See linphone_core_set_in_call_timeout() for details.
**/
LINPHONE_PUBLIC	int linphone_core_get_in_call_timeout(LinphoneCore *lc);

/**
 * Set the in delayed timeout in seconds.
 *
 * @ingroup call_control
 * After this timeout period, a delayed call (internal call initialisation or resolution) is resumed.
**/
LINPHONE_PUBLIC	void linphone_core_set_delayed_timeout(LinphoneCore *lc, int seconds);

/**
 * Returns the delayed timeout
 *
 * @ingroup call_control
 * See linphone_core_set_delayed_timeout() for details.
**/
LINPHONE_PUBLIC	int linphone_core_get_delayed_timeout(LinphoneCore *lc);

/**
 * Set the STUN server address to use when the firewall policy is set to STUN.
 * @param[in] lc #LinphoneCore object
 * @param[in] server The STUN server address to use.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_stun_server(LinphoneCore *lc, const char *server);

/**
 * Get the STUN server address being used.
 * @param[in] lc #LinphoneCore object
 * @return The STUN server address being used.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	const char * linphone_core_get_stun_server(const LinphoneCore *lc);

/**
 * @ingroup network_parameters
 * Return the availability of uPnP.
 *
 * @return true if uPnP is available otherwise return false.
 */
LINPHONE_PUBLIC bool_t linphone_core_upnp_available(void);

/**
 * @ingroup network_parameters
 * Return the internal state of uPnP.
 *
 * @param lc #LinphoneCore
 * @return an LinphoneUpnpState.
 */
LINPHONE_PUBLIC LinphoneUpnpState linphone_core_get_upnp_state(const LinphoneCore *lc);

/**
 * @ingroup network_parameters
 * Return the external ip address of router.
 * In some cases the uPnP can have an external ip address but not a usable uPnP
 * (state different of Ok).
 *
 * @param lc #LinphoneCore
 * @return a null terminated string containing the external ip address. If the
 * the external ip address is not available return null.
 */
LINPHONE_PUBLIC const char * linphone_core_get_upnp_external_ipaddress(const LinphoneCore *lc);

/**
 * Set the public IP address of NAT when using the firewall policy is set to use NAT.
 * @param[in] lc #LinphoneCore object.
 * @param[in] addr The public IP address of NAT to use.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC void linphone_core_set_nat_address(LinphoneCore *lc, const char *addr);

/**
 * Get the public IP address of NAT being used.
 * @param[in] lc #LinphoneCore object.
 * @return The public IP address of NAT being used.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC const char *linphone_core_get_nat_address(const LinphoneCore *lc);

/**
 * Set the policy to use to pass through firewalls.
 * @param[in] lc #LinphoneCore object.
 * @param[in] pol The #LinphoneFirewallPolicy to use.
 * @ingroup network_parameters
 * @deprecated Use linphone_core_set_nat_policy() instead.
 */
LINPHONE_DEPRECATED LINPHONE_PUBLIC	void linphone_core_set_firewall_policy(LinphoneCore *lc, LinphoneFirewallPolicy pol);

/**
 * Get the policy that is used to pass through firewalls.
 * @param[in] lc #LinphoneCore object.
 * @return The #LinphoneFirewallPolicy that is being used.
 * @ingroup network_parameters
 * @deprecated Use linphone_core_get_nat_policy() instead.
 */
LINPHONE_DEPRECATED LINPHONE_PUBLIC	LinphoneFirewallPolicy linphone_core_get_firewall_policy(const LinphoneCore *lc);

/**
 * Set the policy to use to pass through NATs/firewalls.
 * It may be overridden by a NAT policy for a specific proxy config.
 * @param[in] lc #LinphoneCore object
 * @param[in] policy LinphoneNatPolicy object
 * @ingroup network_parameters
 * @see linphone_proxy_config_set_nat_policy()
 */
LINPHONE_PUBLIC void linphone_core_set_nat_policy(LinphoneCore *lc, LinphoneNatPolicy *policy);

/**
 * Get The policy that is used to pass through NATs/firewalls.
 * It may be overridden by a NAT policy for a specific proxy config.
 * @param[in] lc #LinphoneCore object
 * @return LinphoneNatPolicy object in use.
 * @ingroup network_parameters
 * @see linphone_proxy_config_get_nat_policy()
 */
LINPHONE_PUBLIC LinphoneNatPolicy * linphone_core_get_nat_policy(const LinphoneCore *lc);

/**
 * Returns a null terminated static array of string describing the sound devices.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
**/
LINPHONE_PUBLIC const char**  linphone_core_get_sound_devices(LinphoneCore *lc);

/**
 * Use this function when you want to set the default sound devices
 **/
LINPHONE_PUBLIC void linphone_core_set_default_sound_devices(LinphoneCore *lc);

/**
 * Update detection of sound devices.
 *
 * Use this function when the application is notified of USB plug events, so that
 * list of available hardwares for sound playback and capture is updated.
 * @param[in] lc #LinphoneCore object.
 * @ingroup media_parameters
 **/
LINPHONE_PUBLIC void linphone_core_reload_sound_devices(LinphoneCore *lc);

LINPHONE_PUBLIC bool_t linphone_core_sound_device_can_capture(LinphoneCore *lc, const char *device);

/**
 * Returns true if the specified sound device can capture sound.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
 * @param device the device name as returned by linphone_core_get_sound_devices()
**/
LINPHONE_PUBLIC bool_t linphone_core_sound_device_can_playback(LinphoneCore *lc, const char *device);

/**
 * Get ring sound level in 0-100 scale
 *
 * @ingroup media_parameters
 * @deprecated
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED int linphone_core_get_ring_level(LinphoneCore *lc);

/**
 * Get playback sound level in 0-100 scale.
 *
 * @ingroup media_parameters
 * @deprecated
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED int linphone_core_get_play_level(LinphoneCore *lc);

/**
 * Get sound capture level in 0-100 scale
 *
 * @ingroup media_parameters
 * @deprecated
**/
LINPHONE_PUBLIC LINPHONE_DEPRECATED int linphone_core_get_rec_level(LinphoneCore *lc);

/**
 * Set sound ring level in 0-100 scale
 *
 * @ingroup media_parameters
 * @deprecated
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED void linphone_core_set_ring_level(LinphoneCore *lc, int level);

/**
 * Set sound playback level in 0-100 scale
 * @deprecated
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED void linphone_core_set_play_level(LinphoneCore *lc, int level);

/**
 * Set sound capture level in 0-100 scale
 * @deprecated
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC LINPHONE_DEPRECATED void linphone_core_set_rec_level(LinphoneCore *lc, int level);
LINPHONE_DEPRECATED char linphone_core_get_sound_source(LinphoneCore *lc);
LINPHONE_DEPRECATED void linphone_core_set_sound_source(LinphoneCore *lc, char source);

/**
 * Allow to control microphone level:  gain in db
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_core_set_mic_gain_db(LinphoneCore *lc, float level);

/**
 * Get microphone gain in db.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	float linphone_core_get_mic_gain_db(LinphoneCore *lc);

/**
 * Allow to control play level before entering sound card:  gain in db
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_core_set_playback_gain_db(LinphoneCore *lc, float level);

/**
 * Get playback gain in db before entering  sound card.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	float linphone_core_get_playback_gain_db(LinphoneCore *lc);

/**
 * Returns the name of the currently assigned sound device for ringing.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
**/
LINPHONE_PUBLIC const char * linphone_core_get_ringer_device(LinphoneCore *lc);

/**
 * Returns the name of the currently assigned sound device for playback.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
**/
LINPHONE_PUBLIC const char * linphone_core_get_playback_device(LinphoneCore *lc);

/**
 * Returns the name of the currently assigned sound device for capture.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
**/
LINPHONE_PUBLIC const char * linphone_core_get_capture_device(LinphoneCore *lc);

/**
 * Sets the sound device used for ringing.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
 * @param devid the device name as returned by linphone_core_get_sound_devices()
**/
LINPHONE_PUBLIC int linphone_core_set_ringer_device(LinphoneCore *lc, const char * devid);

/**
 * Sets the sound device used for playback.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
 * @param devid the device name as returned by linphone_core_get_sound_devices()
**/
LINPHONE_PUBLIC int linphone_core_set_playback_device(LinphoneCore *lc, const char * devid);

/**
 * Sets the sound device used for capture.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
 * @param devid the device name as returned by linphone_core_get_sound_devices()
**/
LINPHONE_PUBLIC int linphone_core_set_capture_device(LinphoneCore *lc, const char * devid);

/**
 * Whenever the liblinphone is playing a ring to advertise an incoming call or ringback of an outgoing call, this function stops
 * the ringing. Typical use is to stop ringing when the user requests to ignore the call.
 *
 * @param lc The LinphoneCore object
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_stop_ringing(LinphoneCore *lc);

/**
 * Sets the path to a wav file used for ringing.
 *
 * @param path The file must be a wav 16bit linear. Local ring is disabled if null
 * @param lc The LinphoneCore object
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_core_set_ring(LinphoneCore *lc, const char *path);

/**
 * Returns the path to the wav file used for ringing.
 *
 * @param lc The LinphoneCore object
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const char *linphone_core_get_ring(const LinphoneCore *lc);

/**
 * Specify whether the tls server certificate must be verified when connecting to a SIP/TLS server.
 *
 * @ingroup initializing
**/
LINPHONE_PUBLIC void linphone_core_verify_server_certificates(LinphoneCore *lc, bool_t yesno);

/**
 * Specify whether the tls server certificate common name must be verified when connecting to a SIP/TLS server.
 * @ingroup initializing
**/
LINPHONE_PUBLIC void linphone_core_verify_server_cn(LinphoneCore *lc, bool_t yesno);

/**
 * Sets the path to a file or folder containing trusted root CAs (PEM format)
 * @param lc The LinphoneCore object
 * @ingroup initializing
**/
LINPHONE_PUBLIC void linphone_core_set_root_ca(LinphoneCore *lc, const char *path);

/**
 * Sets the trusted root CAs (PEM format)
 * @param lc The LinphoneCore object
 * @ingroup initializing
**/
LINPHONE_PUBLIC void linphone_core_set_root_ca_data(LinphoneCore *lc, const char *data);
/**
 * @internal
 * Set the pointer to an externally provided ssl configuration for the crypto library
 * @param lc #LinphoneCore object
 * @param[in] ssl_config A pointer to an opaque structure which will be provided directly to the crypto library used in bctoolbox. Use with extra care.
 * This ssl_config structure is responsability of the caller and will not be freed at the connection's end.
 * @ingroup initializing
 * @endinternal
 */
LINPHONE_PUBLIC void linphone_core_set_ssl_config(LinphoneCore *lc, void *ssl_config);

/**
 * Gets the path to a file or folder containing the trusted root CAs (PEM format)
 *
 * @param lc The LinphoneCore object
 *
 * @ingroup initializing
**/
LINPHONE_PUBLIC const char *linphone_core_get_root_ca(LinphoneCore *lc);

/**
 * Sets the path to a wav file used for ringing back.
 *
 * Ringback means the ring that is heard when it's ringing at the remote party.
 * The file must be a wav 16bit linear.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_core_set_ringback(LinphoneCore *lc, const char *path);

/**
 * Returns the path to the wav file used for ringing back.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const char * linphone_core_get_ringback(const LinphoneCore *lc);

/**
 * Specify a ring back tone to be played to far end during incoming calls.
 * @param[in] lc #LinphoneCore object
 * @param[in] ring The path to the ring back tone to be played.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_remote_ringback_tone(LinphoneCore *lc, const char *ring);

/**
 * Get the ring back tone played to far end during incoming calls.
 * @param[in] lc #LinphoneCore object
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const char *linphone_core_get_remote_ringback_tone(const LinphoneCore *lc);

/**
 * Enable or disable the ring play during an incoming early media call.
 * @param[in] lc #LinphoneCore object
 * @param[in] enable A boolean value telling whether to enable ringing during an incoming early media call.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC void linphone_core_set_ring_during_incoming_early_media(LinphoneCore *lc, bool_t enable);

/**
 * Tells whether the ring play is enabled during an incoming early media call.
 * @param[in] lc #LinphoneCore object
 * @ingroup media_paramaters
 */
LINPHONE_PUBLIC bool_t linphone_core_get_ring_during_incoming_early_media(const LinphoneCore *lc);

LINPHONE_PUBLIC int linphone_core_preview_ring(LinphoneCore *lc, const char *ring,LinphoneCoreCbFunc func,void * userdata);

/**
 * Returns the MSFactory (mediastreamer2 factory) used by the LinphoneCore to control mediastreamer2 library.
**/
LINPHONE_PUBLIC MSFactory* linphone_core_get_ms_factory(LinphoneCore* lc);

/**
 * Plays an audio file to the local user.
 * This function works at any time, during calls, or when no calls are running.
 * It doesn't request the underlying audio system to support multiple playback streams.
 * @param lc the linphone core
 * @param audiofile path to audio file in wav PCM 16 bit format.
 * @ingroup misc
**/
LINPHONE_PUBLIC int linphone_core_play_local(LinphoneCore *lc, const char *audiofile);

/**
 * Enables or disable echo cancellation. Value is saved and used for subsequent calls.
 * This actually controls software echo cancellation. If hardware echo cancellation is available, it will be always used and activated for calls, regardless
 * of the value passed to this function.
 * When hardware echo cancellation is available, the software one is of course not activated.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_core_enable_echo_cancellation(LinphoneCore *lc, bool_t val);

/**
 * Returns TRUE if echo cancellation is enabled.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	bool_t linphone_core_echo_cancellation_enabled(LinphoneCore *lc);

/**
 * Enables or disable echo limiter.
 * @param[in] lc #LinphoneCore object.
 * @param[in] val TRUE to enable echo limiter, FALSE to disable it.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_core_enable_echo_limiter(LinphoneCore *lc, bool_t val);

/**
 * Tells whether echo limiter is enabled.
 * @param[in] lc #LinphoneCore object.
 * @return TRUE if the echo limiter is enabled, FALSE otherwise.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	bool_t linphone_core_echo_limiter_enabled(const LinphoneCore *lc);

void linphone_core_enable_agc(LinphoneCore *lc, bool_t val);
bool_t linphone_core_agc_enabled(const LinphoneCore *lc);

/**
 * @deprecated Use #linphone_core_enable_mic instead.
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED void linphone_core_mute_mic(LinphoneCore *lc, bool_t muted);

/**
 * Get mic state.
 * @deprecated Use #linphone_core_mic_enabled instead
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED bool_t linphone_core_is_mic_muted(LinphoneCore *lc);

/**
 * Enable or disable the microphone.
 * @param[in] lc #LinphoneCore object
 * @param[in] enable TRUE to enable the microphone, FALSE to disable it.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_enable_mic(LinphoneCore *lc, bool_t enable);

/**
 * Tells whether the microphone is enabled.
 * @param[in] lc #LinphoneCore object
 * @return TRUE if the microphone is enabled, FALSE if disabled.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC bool_t linphone_core_mic_enabled(LinphoneCore *lc);

/**
 * Returns the RTP transmission status for an active stream.
 * If audio is muted and config parameter rtp_no_xmit_on_audio_mute
 * has been set on then the RTP transmission is also muted.
 * @param lc The #LinphoneCore.
 * @return TRUE if the RTP transmisison is muted.
 */
LINPHONE_PUBLIC bool_t linphone_core_is_rtp_muted(LinphoneCore *lc);

LINPHONE_PUBLIC bool_t linphone_core_get_rtp_no_xmit_on_audio_mute(const LinphoneCore *lc);
LINPHONE_PUBLIC void linphone_core_set_rtp_no_xmit_on_audio_mute(LinphoneCore *lc, bool_t val);


/*******************************************************************************
 * Call log related functions                                                  *
 ******************************************************************************/

/**
 * @addtogroup call_logs
 * @{
**/

/**
 * Get the list of call logs (past calls).
 * @param[in] lc LinphoneCore object
 * @return \bctbx_list{LinphoneCallLog}
**/
LINPHONE_PUBLIC const bctbx_list_t * linphone_core_get_call_logs(LinphoneCore *lc);

/**
 * Get the list of call logs (past calls) that matches the given #LinphoneAddress.
 * At the contrary of linphone_core_get_call_logs, it is your responsability to unref the logs and free this list once you are done using it.
 * @param[in] lc LinphoneCore object
 * @param[in] addr LinphoneAddress object
 * @return \bctbx_list{LinphoneCallLog}
**/
LINPHONE_PUBLIC bctbx_list_t * linphone_core_get_call_history_for_address(LinphoneCore *lc, const LinphoneAddress *addr);

/**
 * Get the latest outgoing call log.
 * @param[in] lc LinphoneCore object
 * @return {LinphoneCallLog}
**/
LINPHONE_PUBLIC LinphoneCallLog * linphone_core_get_last_outgoing_call_log(LinphoneCore *lc);

/**
 * Get the call log matching the call id, or NULL if can't be found.
 * @param[in] lc LinphoneCore object
 * @param[in] call_id Call id of the call log to find
 * @return {LinphoneCallLog}
**/
LINPHONE_PUBLIC LinphoneCallLog * linphone_core_find_call_log_from_call_id(LinphoneCore *lc, const char *call_id);

/**
 * Erase the call log.
 * @param[in] lc LinphoneCore object
**/
LINPHONE_PUBLIC void linphone_core_clear_call_logs(LinphoneCore *lc);

/**
 * Get the number of missed calls.
 * Once checked, this counter can be reset with linphone_core_reset_missed_calls_count().
 * @param[in] lc #LinphoneCore object.
 * @return The number of missed calls.
**/
LINPHONE_PUBLIC int linphone_core_get_missed_calls_count(LinphoneCore *lc);

/**
 * Reset the counter of missed calls.
 * @param[in] lc #LinphoneCore object.
**/
LINPHONE_PUBLIC void linphone_core_reset_missed_calls_count(LinphoneCore *lc);

/**
 * Remove a specific call log from call history list.
 * This function destroys the call log object. It must not be accessed anymore by the application after calling this function.
 * @param[in] lc #LinphoneCore object
 * @param[in] call_log #LinphoneCallLog object to remove.
**/
LINPHONE_PUBLIC void linphone_core_remove_call_log(LinphoneCore *lc, LinphoneCallLog *call_log);

/**
 * Sets the database filename where call logs will be stored.
 * If the file does not exist, it will be created.
 * @ingroup initializing
 * @param lc the linphone core
 * @param path filesystem path
**/
LINPHONE_PUBLIC void linphone_core_set_call_logs_database_path(LinphoneCore *lc, const char *path);

/**
 * Migrates the call logs from the linphonerc to the database if not done yet
 * @ingroup initializing
 * @param lc the linphone core
**/
LINPHONE_PUBLIC void linphone_core_migrate_logs_from_rc_to_db(LinphoneCore *lc);

/**
 * @}
**/

/**
 * Tells whether VCARD support is builtin.
 * @return TRUE if VCARD is supported, FALSE otherwise.
 * @ingroup misc
 */
LINPHONE_PUBLIC bool_t linphone_core_vcard_supported(void);

/* video support */
LINPHONE_PUBLIC bool_t linphone_core_video_supported(LinphoneCore *lc);

/**
 * Enables video globally.
 *
 * This function does not have any effect during calls. It just indicates LinphoneCore to
 * initiate future calls with video or not. The two boolean parameters indicate in which
 * direction video is enabled. Setting both to false disables video entirely.
 *
 * @param lc The LinphoneCore object
 * @param vcap_enabled indicates whether video capture is enabled
 * @param display_enabled indicates whether video display should be shown
 * @ingroup media_parameters
 * @deprecated Use #linphone_core_enable_video_capture and #linphone_core_enable_video_display instead.
**/
LINPHONE_PUBLIC	LINPHONE_DEPRECATED void linphone_core_enable_video(LinphoneCore *lc, bool_t vcap_enabled, bool_t display_enabled);

/**
 * Returns TRUE if either capture or display is enabled, FALSE otherwise.
 * same as  ( #linphone_core_video_capture_enabled | #linphone_core_video_display_enabled )
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC  bool_t linphone_core_video_enabled(LinphoneCore *lc);

/**
 * Enable or disable video capture.
 *
 * This function does not have any effect during calls. It just indicates the #LinphoneCore to
 * initiate future calls with video capture or not.
 * @param[in] lc #LinphoneCore object.
 * @param[in] enable TRUE to enable video capture, FALSE to disable it.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_enable_video_capture(LinphoneCore *lc, bool_t enable);

/**
 * Enable or disable video display.
 *
 * This function does not have any effect during calls. It just indicates the #LinphoneCore to
 * initiate future calls with video display or not.
 * @param[in] lc #LinphoneCore object.
 * @param[in] enable TRUE to enable video display, FALSE to disable it.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_enable_video_display(LinphoneCore *lc, bool_t enable);


/**
 * Enable or disable video source reuse when switching from preview to actual video call.
 *
 * This source reuse is useful when you always display the preview, even before calls are initiated.
 * By keeping the video source for the transition to a real video call, you will smooth out the
 * source close/reopen cycle.
 *
 * This function does not have any effect durfing calls. It just indicates the #LinphoneCore to
 * initiate future calls with video source reuse or not.
 * Also, at the end of a video call, the source will be closed whatsoever for now.
 * @param[in] lc #LinphoneCore object
 * @param[in] enable TRUE to enable video source reuse. FALSE to disable it for subsequent calls.
 * @ingroup media_parameters
 *
 */
LINPHONE_PUBLIC void linphone_core_enable_video_source_reuse(LinphoneCore* lc, bool_t enable);

/**
 * Tells whether video capture is enabled.
 * @param[in] lc #LinphoneCore object.
 * @return TRUE if video capture is enabled, FALSE if disabled.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC bool_t linphone_core_video_capture_enabled(LinphoneCore *lc);

/**
 * Tells whether video display is enabled.
 * @param[in] lc #LinphoneCore object.
 * @return TRUE if video display is enabled, FALSE if disabled.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC bool_t linphone_core_video_display_enabled(LinphoneCore *lc);

/**
 * Sets the default policy for video.
 * This policy defines whether:
 * - video shall be initiated by default for outgoing calls
 * - video shall be accepter by default for incoming calls
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC	void linphone_core_set_video_policy(LinphoneCore *lc, const LinphoneVideoPolicy *policy);

/**
 * Get the default policy for video.
 * See linphone_core_set_video_policy() for more details.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const LinphoneVideoPolicy *linphone_core_get_video_policy(const LinphoneCore *lc);

typedef struct MSVideoSizeDef{
	MSVideoSize vsize;
	const char *name;
}MSVideoSizeDef;
/**
 * Returns the zero terminated table of supported video resolutions.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const MSVideoSizeDef *linphone_core_get_supported_video_sizes(LinphoneCore *lc);

/**
 * Sets the preferred video size.
 *
 * @ingroup media_parameters
 * This applies only to the stream that is captured and sent to the remote party,
 * since we accept all standard video size on the receive path.
**/LINPHONE_PUBLIC void linphone_core_set_preferred_video_size(LinphoneCore *lc, MSVideoSize vsize);
/**
 * Sets the video size for the captured (preview) video.
 * This method is for advanced usage where a video capture must be set independently of the size of the stream actually sent through the call.
 * This allows for example to have the preview window with HD resolution even if due to bandwidth constraint the sent video size is small.
 * Using this feature increases the CPU consumption, since a rescaling will be done internally.
 * @ingroup media_parameters
 * @param lc the linphone core
 * @param vsize the video resolution choosed for capuring and previewing. It can be (0,0) to not request any specific preview size and let the core optimize the processing.
**/
LINPHONE_PUBLIC void linphone_core_set_preview_video_size(LinphoneCore *lc, MSVideoSize vsize);
/**
 * Sets the preview video size by its name. See linphone_core_set_preview_video_size() for more information about this feature.
 *
 * @ingroup media_parameters
 * Video resolution names are: qcif, svga, cif, vga, 4cif, svga ...
**/
LINPHONE_PUBLIC void linphone_core_set_preview_video_size_by_name(LinphoneCore *lc, const char *name);
/**
 * Returns video size for the captured video if it was previously set by linphone_core_set_preview_video_size(), otherwise returns a 0,0 size.
 * @see linphone_core_set_preview_video_size()
 * @ingroup media_parameters
 * @param lc the core
 * @return a MSVideoSize
**/
LINPHONE_PUBLIC MSVideoSize linphone_core_get_preview_video_size(const LinphoneCore *lc);
/**
 * Returns the effective video size for the captured video as provided by the camera.
 * When preview is disabled or not yet started, this function returns a zeroed video size.
 * @see linphone_core_set_preview_video_size()
 * @ingroup media_parameters
 * @param lc the core
 * @return a MSVideoSize
**/
LINPHONE_PUBLIC MSVideoSize linphone_core_get_current_preview_video_size(const LinphoneCore *lc);
/**
 * Returns the current preferred video size for sending.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC MSVideoSize linphone_core_get_preferred_video_size(const LinphoneCore *lc);

/**
 * Get the name of the current preferred video size for sending.
 * @param[in] lc #LinphoneCore object.
 * @return A string containing the name of the current preferred video size (to be freed with ms_free()).
 */
LINPHONE_PUBLIC char * linphone_core_get_preferred_video_size_name(const LinphoneCore *lc);
/**
 * Sets the preferred video size by its name.
 *
 * @ingroup media_parameters
 * This is identical to linphone_core_set_preferred_video_size() except
 * that it takes the name of the video resolution as input.
 * Video resolution names are: qcif, svga, cif, vga, 4cif, svga ...
**/
LINPHONE_PUBLIC void linphone_core_set_preferred_video_size_by_name(LinphoneCore *lc, const char *name);
/**
 * Set the preferred frame rate for video.
 * Based on the available bandwidth constraints and network conditions, the video encoder
 * remains free to lower the framerate. There is no warranty that the preferred frame rate be the actual framerate.
 * used during a call. Default value is 0, which means "use encoder's default fps value".
 * @ingroup media_parameters
 * @param lc the LinphoneCore
 * @param fps the target frame rate in number of frames per seconds.
**/
LINPHONE_PUBLIC void linphone_core_set_preferred_framerate(LinphoneCore *lc, float fps);
/**
 * Returns the preferred video framerate, previously set by linphone_core_set_preferred_framerate().
 * @ingroup media_parameters
 * @param lc the linphone core
 * @return frame rate in number of frames per seconds.
**/
LINPHONE_PUBLIC float linphone_core_get_preferred_framerate(LinphoneCore *lc);

/**
 * Controls video preview enablement.
 *
 * @ingroup media_parameters
 * Video preview refers to the action of displaying the local webcam image
 * to the user while not in call.
**/
LINPHONE_PUBLIC void linphone_core_enable_video_preview(LinphoneCore *lc, bool_t val);
/**
 * Take a photo of currently from capture device and write it into a jpeg file.
 * Note that the snapshot is asynchronous, an application shall not assume that the file is created when the function returns.
 * @ingroup misc
 * @param lc the linphone core
 * @param file a path where to write the jpeg content.
 * @return 0 if successfull, -1 otherwise (typically if jpeg format is not supported).
**/
LINPHONE_PUBLIC int linphone_core_take_preview_snapshot(LinphoneCore *lc, const char *file);

/**
 * Returns TRUE if video previewing is enabled.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC bool_t linphone_core_video_preview_enabled(const LinphoneCore *lc);

/**
 * Enables or disable self view during calls.
 *
 * @ingroup media_parameters
 * Self-view refers to having local webcam image inserted in corner
 * of the video window during calls.
 * This function works at any time, including during calls.
**/
LINPHONE_PUBLIC void linphone_core_enable_self_view(LinphoneCore *lc, bool_t val);

/**
 * Returns TRUE if self-view is enabled, FALSE otherwise.
 *
 * @ingroup media_parameters
 *
 * Refer to linphone_core_enable_self_view() for details.
**/
LINPHONE_PUBLIC bool_t linphone_core_self_view_enabled(const LinphoneCore *lc);


/**
 * Update detection of camera devices.
 *
 * Use this function when the application is notified of USB plug events, so that
 * list of available hardwares for video capture is updated.
 * @param[in] lc #LinphoneCore object.
 * @ingroup media_parameters
 **/
LINPHONE_PUBLIC void linphone_core_reload_video_devices(LinphoneCore *lc);

/**
 * Returns an null-terminated array of available video capture devices.
 *
 * @ingroup media_parameters
 * The array is NULL terminated.
**/
LINPHONE_PUBLIC const char**  linphone_core_get_video_devices(const LinphoneCore *lc);

/**
 * Sets the active video device.
 *
 * @ingroup media_parameters
 * @param lc The LinphoneCore object
 * @param id the name of the video device as returned by linphone_core_get_video_devices()
**/
LINPHONE_PUBLIC int linphone_core_set_video_device(LinphoneCore *lc, const char *id);

/**
 * Returns the name of the currently active video device.
 *
 * @param lc The LinphoneCore object
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const char *linphone_core_get_video_device(const LinphoneCore *lc);

/* Set and get static picture to be used when "Static picture" is the video device */
/**
 * Set the path to the image file to stream when "Static picture" is set as the video device.
 * @param[in] lc #LinphoneCore object.
 * @param[in] path The path to the image file to use.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC int linphone_core_set_static_picture(LinphoneCore *lc, const char *path);

/**
 * Get the path to the image file streamed when "Static picture" is set as the video device.
 * @param[in] lc #LinphoneCore object.
 * @return The path to the image file streamed when "Static picture" is set as the video device.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC const char *linphone_core_get_static_picture(LinphoneCore *lc);

/**
 * Set the frame rate for static picture.
 * @param[in] lc #LinphoneCore object.
 * @param[in] fps The new frame rate to use for static picture.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC int linphone_core_set_static_picture_fps(LinphoneCore *lc, float fps);

/**
 * Get the frame rate for static picture
 * @param[in] lc #LinphoneCore object.
 * @return The frame rate used for static picture.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC float linphone_core_get_static_picture_fps(LinphoneCore *lc);

/**
 * Returns the native window handle of the video window, casted as an unsigned long.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void * linphone_core_get_native_video_window_id(const LinphoneCore *lc);

/**
 * @ingroup media_parameters
 * For MacOS, Linux, Windows: core will create its own window
 * */
#define LINPHONE_VIDEO_DISPLAY_AUTO (void*)((unsigned long) 0)
/**
 * @ingroup media_parameters
 * For MacOS, Linux, Windows: do nothing
 * */

#define LINPHONE_VIDEO_DISPLAY_NONE (void*)((unsigned long) -1)
/**
 * @ingroup media_parameters
 * Set the native video window id where the video is to be displayed.
 * For MacOS, Linux, Windows: if not set or LINPHONE_VIDEO_DISPLAY_AUTO the core will create its own window, unless the special id LINPHONE_VIDEO_DISPLAY_NONE is given.
**/
LINPHONE_PUBLIC void linphone_core_set_native_video_window_id(LinphoneCore *lc, void *id);

/**
 * Returns the native window handle of the video preview window, casted as an unsigned long.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void * linphone_core_get_native_preview_window_id(const LinphoneCore *lc);

/**
 * @ingroup media_parameters
 * Set the native window id where the preview video (local camera) is to be displayed.
 * This has to be used in conjonction with linphone_core_use_preview_window().
 * MacOS, Linux, Windows: if not set or zero the core will create its own window, unless the special id -1 is given.
**/
LINPHONE_PUBLIC void linphone_core_set_native_preview_window_id(LinphoneCore *lc, void *id);

/**
 * Tells the core to use a separate window for local camera preview video, instead of
 * inserting local view within the remote video window.
 * @param[in] lc #LinphoneCore object.
 * @param[in] yesno TRUE to use a separate window, FALSE to insert the preview in the remote video window.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_use_preview_window(LinphoneCore *lc, bool_t yesno);

/**
 * Returns current device orientation.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC int linphone_core_get_device_rotation(LinphoneCore *lc );

/**
 * Tells the core the device current orientation.
 * This can be used by capture filters
 * on mobile devices to select between portrait/landscape mode and to produce properly
 * oriented images. The exact meaning of the value in rotation if left to each device
 * specific implementations.
 * @param lc  object.
 * @param rotation IOS supported values are 0 for UIInterfaceOrientationPortrait and 270 for UIInterfaceOrientationLandscapeRight.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_device_rotation(LinphoneCore *lc, int rotation);

/**
 * Get the camera sensor rotation.
 *
 * This is needed on some mobile platforms to get the number of degrees the camera sensor
 * is rotated relative to the screen.
 *
 * @param lc The linphone core related to the operation
 * @return The camera sensor rotation in degrees (0 to 360) or -1 if it could not be retrieved
 */
LINPHONE_PUBLIC int linphone_core_get_camera_sensor_rotation(LinphoneCore *lc);

/**
 * Can be used to disable video showing to free XV port
**/
void linphone_core_show_video(LinphoneCore *lc, bool_t show);

/** @deprecated Use linphone_core_set_use_files() instead. */
#define linphone_core_use_files(lc, yesno) linphone_core_set_use_files(lc, yesno)
/**
 * Ask the core to stream audio from and to files, instead of using the soundcard.
 * @ingroup media_parameters
 * @param[in] lc LinphoneCore object
 * @param[in] yesno A boolean value asking to stream audio from and to files or not.
**/
LINPHONE_PUBLIC void linphone_core_set_use_files(LinphoneCore *lc, bool_t yesno);

/**
 * Gets whether linphone is currently streaming audio from and to files, rather
 * than using the soundcard.
 * @ingroup media_parameters
 * @param[in] lc LinphoneCore object
 * @return A boolean value representing whether linphone is streaming audio from and to files or not.
**/
LINPHONE_PUBLIC bool_t linphone_core_get_use_files(LinphoneCore *lc);

/**
 * Get the wav file that is played when putting somebody on hold,
 * or when files are used instead of soundcards (see linphone_core_set_use_files()).
 *
 * The file is a 16 bit linear wav file.
 * @ingroup media_parameters
 * @param[in] lc LinphoneCore object
 * @return The path to the file that is played when putting somebody on hold.
 */
LINPHONE_PUBLIC const char * linphone_core_get_play_file(const LinphoneCore *lc);

/**
 * Sets a wav file to be played when putting somebody on hold,
 * or when files are used instead of soundcards (see linphone_core_set_use_files()).
 *
 * The file must be a 16 bit linear wav file.
 * @ingroup media_parameters
 * @param[in] lc LinphoneCore object
 * @param[in] file The path to the file to be played when putting somebody on hold.
**/
LINPHONE_PUBLIC void linphone_core_set_play_file(LinphoneCore *lc, const char *file);

/**
 * Get the wav file where incoming stream is recorded,
 * when files are used instead of soundcards (see linphone_core_set_use_files()).
 *
 * This feature is different from call recording (linphone_call_params_set_record_file())
 * The file is a 16 bit linear wav file.
 * @ingroup media_parameters
 * @param[in] lc LinphoneCore object
 * @return The path to the file where incoming stream is recorded.
**/
LINPHONE_PUBLIC const char * linphone_core_get_record_file(const LinphoneCore *lc);

/**
 * Sets a wav file where incoming stream is to be recorded,
 * when files are used instead of soundcards (see linphone_core_set_use_files()).
 *
 * This feature is different from call recording (linphone_call_params_set_record_file())
 * The file will be a 16 bit linear wav file.
 * @ingroup media_parameters
 * @param[in] lc LinphoneCore object
 * @param[in] file The path to the file where incoming stream is to be recorded.
**/
LINPHONE_PUBLIC void linphone_core_set_record_file(LinphoneCore *lc, const char *file);

/**
 * @ingroup media_parameters
 * Plays a dtmf sound to the local user.
 * @param lc #LinphoneCore
 * @param dtmf DTMF to play ['0'..'16'] | '#' | '#'
 * @param duration_ms duration in ms, -1 means play until next further call to #linphone_core_stop_dtmf()
**/
LINPHONE_PUBLIC void linphone_core_play_dtmf(LinphoneCore *lc, char dtmf, int duration_ms);

/**
 * @ingroup media_parameters
 *
 * Stops playing a dtmf started by linphone_core_play_dtmf().
**/
LINPHONE_PUBLIC void linphone_core_stop_dtmf(LinphoneCore *lc);

LINPHONE_PUBLIC	int linphone_core_get_current_call_duration(const LinphoneCore *lc);


LINPHONE_PUBLIC int linphone_core_get_mtu(const LinphoneCore *lc);

/**
 * Sets the maximum transmission unit size in bytes.
 * This information is useful for sending RTP packets.
 * Default value is 1500.
 *
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_mtu(LinphoneCore *lc, int mtu);

/**
 * @ingroup network_parameters
 * This method is called by the application to notify the linphone core library when network is reachable.
 * Calling this method with true trigger linphone to initiate a registration process for all proxies.
 * Calling this method disables the automatic network detection mode. It means you must call this method after each network state changes.
 */
LINPHONE_PUBLIC	void linphone_core_set_network_reachable(LinphoneCore* lc,bool_t value);
/**
 * @ingroup network_parameters
 * return network state either as positioned by the application or by linphone itself.
 */
LINPHONE_PUBLIC	bool_t linphone_core_is_network_reachable(LinphoneCore* lc);

/**
 * @ingroup network_parameters
 * This method is called by the application to notify the linphone core library when the SIP network is reachable.
 * This is for advanced usage, when SIP and RTP layers are required to use different interfaces.
 * Most applications just need linphone_core_set_network_reachable().
 */
LINPHONE_PUBLIC	void linphone_core_set_sip_network_reachable(LinphoneCore* lc,bool_t value);

/**
 * @ingroup network_parameters
 * This method is called by the application to notify the linphone core library when the media (RTP) network is reachable.
 * This is for advanced usage, when SIP and RTP layers are required to use different interfaces.
 * Most applications just need linphone_core_set_network_reachable().
 */
LINPHONE_PUBLIC	void linphone_core_set_media_network_reachable(LinphoneCore* lc,bool_t value);

/**
 *  @ingroup network_parameters
 *  enable signaling keep alive. small udp packet sent periodically to keep udp NAT association
 */
LINPHONE_PUBLIC	void linphone_core_enable_keep_alive(LinphoneCore* lc,bool_t enable);

/**
 *  @ingroup network_parameters
 * Is signaling keep alive
 */
LINPHONE_PUBLIC	bool_t linphone_core_keep_alive_enabled(LinphoneCore* lc);

/**
 * Retrieves the user pointer that was given to linphone_core_new()
 *
 * @ingroup initializing
**/
LINPHONE_PUBLIC	void *linphone_core_get_user_data(const LinphoneCore *lc);

/**
 * Associate a user pointer to the linphone core.
 *
 * @ingroup initializing
**/
LINPHONE_PUBLIC	void linphone_core_set_user_data(LinphoneCore *lc, void *userdata);

/**
 * Returns the LpConfig object used to manage the storage (config) file.
 *
 * @ingroup misc
 * The application can use the LpConfig object to insert its own private
 * sections and pairs of key=value in the configuration file.
**/
LINPHONE_PUBLIC LpConfig * linphone_core_get_config(LinphoneCore *lc);

/**
 * Create a LpConfig object from a user config file.
 * @param[in] lc #LinphoneCore object
 * @param[in] filename The filename of the config file to read to fill the instantiated LpConfig
 * @ingroup misc
 */
LINPHONE_PUBLIC LpConfig * linphone_core_create_lp_config(LinphoneCore *lc, const char *filename);

/**
 * Set a callback for some blocking operations, it takes you informed of the progress of the operation
 */
LINPHONE_PUBLIC void linphone_core_set_waiting_callback(LinphoneCore *lc, LinphoneCoreWaitingCallback cb, void *user_context);

/**
 * Returns the list of registered SipSetup (linphonecore plugins)
 */
LINPHONE_PUBLIC const bctbx_list_t * linphone_core_get_sip_setups(LinphoneCore *lc);

/**
 * Destroys a LinphoneCore
 *
 * @ingroup initializing
 * @deprecated Use linphone_core_unref() instead.
**/
LINPHONE_DEPRECATED LINPHONE_PUBLIC	void linphone_core_destroy(LinphoneCore *lc);

/*for advanced users:*/
typedef RtpTransport * (*LinphoneCoreRtpTransportFactoryFunc)(void *data, int port);
struct _LinphoneRtpTransportFactories{
	LinphoneCoreRtpTransportFactoryFunc audio_rtp_func;
	void *audio_rtp_func_data;
	LinphoneCoreRtpTransportFactoryFunc audio_rtcp_func;
	void *audio_rtcp_func_data;
	LinphoneCoreRtpTransportFactoryFunc video_rtp_func;
	void *video_rtp_func_data;
	LinphoneCoreRtpTransportFactoryFunc video_rtcp_func;
	void *video_rtcp_func_data;
};
typedef struct _LinphoneRtpTransportFactories LinphoneRtpTransportFactories;

void linphone_core_set_rtp_transport_factories(LinphoneCore* lc, LinphoneRtpTransportFactories *factories);

/**
 * Retrieve RTP statistics regarding current call.
 * @param lc the LinphoneCore
 * @param local RTP statistics computed locally.
 * @param remote RTP statistics computed by far end (obtained via RTCP feedback).
 *
 * @note Remote RTP statistics is not implemented yet.
 *
 * @return 0 or -1 if no call is running.
**/
int linphone_core_get_current_call_stats(LinphoneCore *lc, rtp_stats_t *local, rtp_stats_t *remote);

/**
 * Get the number of Call
 *
 * @ingroup call_control
**/
LINPHONE_PUBLIC	int linphone_core_get_calls_nb(const LinphoneCore *lc);

/**
 * Returns the current list of calls.
 * @param[in] lc The LinphoneCore object
 * @return \bctbx_list{LinphoneCall}
 *
 * Note that this list is read-only and might be changed by the core after a function call to linphone_core_iterate().
 * Similarly the LinphoneCall objects inside it might be destroyed without prior notice.
 * To hold references to LinphoneCall object into your program, you must use linphone_call_ref().
 *
 * @ingroup call_control
 */
LINPHONE_PUBLIC	const bctbx_list_t *linphone_core_get_calls(LinphoneCore *lc);

LINPHONE_PUBLIC LinphoneGlobalState linphone_core_get_global_state(const LinphoneCore *lc);
/**
 * force registration refresh to be initiated upon next iterate
 * @ingroup proxies
 */
LINPHONE_PUBLIC void linphone_core_refresh_registers(LinphoneCore* lc);

/**
 * Set the path to the file storing the zrtp secrets cache.
 * @param[in] lc #LinphoneCore object
 * @param[in] file The path to the file to use to store the zrtp secrets cache.
 * @ingroup initializing
 */
LINPHONE_PUBLIC void linphone_core_set_zrtp_secrets_file(LinphoneCore *lc, const char* file);

/**
 * Get the path to the file storing the zrtp secrets cache.
 * @param[in] lc #LinphoneCore object.
 * @return The path to the file storing the zrtp secrets cache.
 * @ingroup initializing
 */
LINPHONE_PUBLIC const char *linphone_core_get_zrtp_secrets_file(LinphoneCore *lc);

/**
 * Set the path to the directory storing the user's x509 certificates (used by dtls)
 * @param[in] lc #LinphoneCore object
 * @param[in] path The path to the directory to use to store the user's certificates.
 * @ingroup initializing
 */
LINPHONE_PUBLIC void linphone_core_set_user_certificates_path(LinphoneCore *lc, const char* path);

/**
 * Get the path to the directory storing the user's certificates.
 * @param[in] lc #LinphoneCore object.
 * @returns The path to the directory storing the user's certificates.
 * @ingroup initializing
 */
LINPHONE_PUBLIC const char *linphone_core_get_user_certificates_path(LinphoneCore *lc);

/**
 * Reload mediastreamer2 plugins from specified directory.
 * @param[in] lc #LinphoneCore object.
 * @param[in] path the path from where plugins are to be loaded, pass NULL to use default (compile-time determined) plugin directory.
 * @ingroup initializing
 */
LINPHONE_PUBLIC void linphone_core_reload_ms_plugins(LinphoneCore *lc, const char *path);

/**
 * Search from the list of current calls if a remote address match uri
 * @ingroup call_control
 * @param uri which should match call remote uri
 * @return LinphoneCall or NULL is no match is found
 */
LINPHONE_PUBLIC LinphoneCall* linphone_core_find_call_from_uri(const LinphoneCore *lc, const char *uri);

/**
 * @addtogroup call_control
 * @{
 */

/**
 * Create a conference
 * @param lc The #LinphoneCore instance where the conference will be created inside.
 * @param params Parameters of the conference. See #LinphoneConferenceParams.
 * @return A pointer on the freshly created conference. That object will be automatically
 * freed by the core after calling linphone_core_terminate_conference().
 */
LINPHONE_PUBLIC LinphoneConference *linphone_core_create_conference_with_params(LinphoneCore *lc, const LinphoneConferenceParams *params);
/**
 * Add a participant to the conference. If no conference is going on
 * a new internal conference context is created and the participant is
 * added to it.
 * @param lc #LinphoneCore
 * @param call The current call with the participant to add
 * @return 0 if succeeded. Negative number if failed
 */
LINPHONE_PUBLIC int linphone_core_add_to_conference(LinphoneCore *lc, LinphoneCall *call);
/**
 * Add all current calls into the conference. If no conference is running
 * a new internal conference context is created and all current calls
 * are added to it.
 * @param lc #LinphoneCore
 * @return 0 if succeeded. Negative number if failed
 */
LINPHONE_PUBLIC int linphone_core_add_all_to_conference(LinphoneCore *lc);
/**
 * Remove a call from the conference.
 * @param lc the linphone core
 * @param call a call that has been previously merged into the conference.
 *
 * After removing the remote participant belonging to the supplied call, the call becomes a normal call in paused state.
 * If one single remote participant is left alone together with the local user in the conference after the removal, then the conference is
 * automatically transformed into a simple call in StreamsRunning state.
 * The conference's resources are then automatically destroyed.
 *
 * In other words, unless linphone_core_leave_conference() is explicitly called, the last remote participant of a conference is automatically
 * put in a simple call in running state.
 *
 * @return 0 if successful, -1 otherwise.
 **/
 LINPHONE_PUBLIC int linphone_core_remove_from_conference(LinphoneCore *lc, LinphoneCall *call);
/**
 * Indicates whether the local participant is part of a conference.
 * @warning That function automatically fails in the case of conferences using a
 * conferencet server (focus). If you use such a conference, you should use
 * linphone_conference_remove_participant() instead.
 * @param lc the linphone core
 * @return TRUE if the local participant is in a conference, FALSE otherwise.
*/
LINPHONE_PUBLIC bool_t linphone_core_is_in_conference(const LinphoneCore *lc);
/**
 * Join the local participant to the running conference
 * @param lc #LinphoneCore
 * @return 0 if succeeded. Negative number if failed
 */
LINPHONE_PUBLIC int linphone_core_enter_conference(LinphoneCore *lc);
/**
 * Make the local participant leave the running conference
 * @param lc #LinphoneCore
 * @return 0 if succeeded. Negative number if failed
 */
LINPHONE_PUBLIC int linphone_core_leave_conference(LinphoneCore *lc);
/**
 * Get the set input volume of the local participant
 * @param lc #LinphoneCore
 * @return A value inside [0.0 ; 1.0]
 */
LINPHONE_PUBLIC float linphone_core_get_conference_local_input_volume(LinphoneCore *lc);
/**
 * Terminate the running conference. If it is a local conference, all calls
 * inside it will become back separate calls and will be put in #LinphoneCallPaused state.
 * If it is a conference involving a focus server, all calls inside the conference
 * will be terminated.
 * @param lc #LinphoneCore
 * @return 0 if succeeded. Negative number if failed
 */
LINPHONE_PUBLIC int linphone_core_terminate_conference(LinphoneCore *lc);
/**
 * Get the number of participant in the running conference. The local
 * participant is included in the count only if it is in the conference.
 * @param lc #LinphoneCore
 * @return The number of participant
 */
LINPHONE_PUBLIC int linphone_core_get_conference_size(LinphoneCore *lc);
/**
 * Start recording the running conference
 * @param lc #LinphoneCore
 * @param path Path to the file where the recording will be written
 * @return 0 if succeeded. Negative number if failed
 */
LINPHONE_PUBLIC int linphone_core_start_conference_recording(LinphoneCore *lc, const char *path);
/**
 * Stop recording the running conference
 * @param lc #LinphoneCore
 * @return 0 if succeeded. Negative number if failed
 */
LINPHONE_PUBLIC int linphone_core_stop_conference_recording(LinphoneCore *lc);
/**
 * Get a pointer on the internal conference object.
 * @param lc #LinphoneCore
 * @return A pointer on #LinphoneConference or NULL if no conference are going on
 */
LINPHONE_PUBLIC LinphoneConference *linphone_core_get_conference(LinphoneCore *lc);


/**
 * @}
 */

/**
 * Get the maximum number of simultaneous calls Linphone core can manage at a time. All new call above this limit are declined with a busy answer
 * @ingroup initializing
 * @param lc core
 * @return max number of simultaneous calls
 */
LINPHONE_PUBLIC int linphone_core_get_max_calls(LinphoneCore *lc);
/**
 * Set the maximum number of simultaneous calls Linphone core can manage at a time. All new call above this limit are declined with a busy answer
 * @ingroup initializing
 * @param lc core
 * @param max number of simultaneous calls
 */
LINPHONE_PUBLIC void linphone_core_set_max_calls(LinphoneCore *lc, int max);

/**
 * Check if a call will need the sound resources in near future (typically an outgoing call that is awaiting
 * response).
 * In liblinphone, it is not possible to have two independant calls using sound device or camera at the same time.
 * In order to prevent this situation, an application can use linphone_core_sound_resources_locked() to know whether
 * it is possible at a given time to start a new outgoing call.
 * When the function returns TRUE, an application should not allow the user to start an outgoing call.
 *
 * @ingroup call_control
 * @param lc The LinphoneCore
**/
LINPHONE_PUBLIC	bool_t linphone_core_sound_resources_locked(LinphoneCore *lc);
/**
 * @ingroup initializing
 * Check if a media encryption type is supported
 * @param lc core
 * @param menc LinphoneMediaEncryption
 * @return whether a media encryption scheme is supported by the LinphoneCore engine
**/

LINPHONE_PUBLIC	bool_t linphone_core_media_encryption_supported(const LinphoneCore *lc, LinphoneMediaEncryption menc);

/**
 * Choose the media encryption policy to be used for RTP packets.
 * @param[in] lc #LinphoneCore object.
 * @param[in] menc The media encryption policy to be used.
 * @return 0 if successful, any other value otherwise.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	int linphone_core_set_media_encryption(LinphoneCore *lc, LinphoneMediaEncryption menc);

/**
 * Get the media encryption policy being used for RTP packets.
 * @param[in] lc #LinphoneCore object.
 * @return The media encryption policy being used.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	LinphoneMediaEncryption linphone_core_get_media_encryption(LinphoneCore *lc);

/**
 * Get behaviour when encryption parameters negociation fails on outgoing call.
 * @param[in] lc #LinphoneCore object.
 * @return TRUE means the call will fail; FALSE means an INVITE will be resent with encryption disabled.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	bool_t linphone_core_is_media_encryption_mandatory(LinphoneCore *lc);

/**
 * Define behaviour when encryption parameters negociation fails on outgoing call.
 * @param[in] lc #LinphoneCore object.
 * @param[in] m If set to TRUE call will fail; if set to FALSE will resend an INVITE with encryption disabled.
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_media_encryption_mandatory(LinphoneCore *lc, bool_t m);

/**
 * Init call params using LinphoneCore's current configuration
 */
LINPHONE_PUBLIC	void linphone_core_init_default_params(LinphoneCore*lc, LinphoneCallParams *params);

/**
 * True if tunnel support was compiled.
 *  @ingroup tunnel
 */
LINPHONE_PUBLIC	bool_t linphone_core_tunnel_available(void);

/**
 * Linphone tunnel object.
 * @ingroup tunnel
 */
typedef struct _LinphoneTunnel LinphoneTunnel;

/**
* get tunnel instance if available
* @ingroup tunnel
* @param lc core object
* @returns LinphoneTunnel or NULL if not available
*/
LINPHONE_PUBLIC	LinphoneTunnel *linphone_core_get_tunnel(const LinphoneCore *lc);

/**
 * Set the DSCP field for SIP signaling channel.
 *
 * @ingroup network_parameters
 * * The DSCP defines the quality of service in IP packets.
 *
**/
LINPHONE_PUBLIC void linphone_core_set_sip_dscp(LinphoneCore *lc, int dscp);

/**
 * Get the DSCP field for SIP signaling channel.
 *
 * @ingroup network_parameters
 * * The DSCP defines the quality of service in IP packets.
 *
**/
LINPHONE_PUBLIC int linphone_core_get_sip_dscp(const LinphoneCore *lc);

/**
 * Set the DSCP field for outgoing audio streams.
 *
 * @ingroup network_parameters
 * The DSCP defines the quality of service in IP packets.
 *
**/
LINPHONE_PUBLIC void linphone_core_set_audio_dscp(LinphoneCore *lc, int dscp);

/**
 * Get the DSCP field for outgoing audio streams.
 *
 * @ingroup network_parameters
 * The DSCP defines the quality of service in IP packets.
 *
**/
LINPHONE_PUBLIC int linphone_core_get_audio_dscp(const LinphoneCore *lc);

/**
 * Set the DSCP field for outgoing video streams.
 *
 * @ingroup network_parameters
 * The DSCP defines the quality of service in IP packets.
 *
**/
LINPHONE_PUBLIC void linphone_core_set_video_dscp(LinphoneCore *lc, int dscp);

/**
 * Get the DSCP field for outgoing video streams.
 *
 * @ingroup network_parameters
 * The DSCP defines the quality of service in IP packets.
 *
**/
LINPHONE_PUBLIC int linphone_core_get_video_dscp(const LinphoneCore *lc);

/**
 * Get the name of the mediastreamer2 filter used for rendering video.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const char *linphone_core_get_video_display_filter(LinphoneCore *lc);

/**
 * Set the name of the mediastreamer2 filter to be used for rendering video.
 * This is for advanced users of the library, mainly to workaround hardware/driver bugs.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_video_display_filter(LinphoneCore *lc, const char *filtername);

/** Contact Providers
  */

typedef unsigned int ContactSearchID;

typedef struct _LinphoneContactSearch LinphoneContactSearch;
typedef struct _LinphoneContactProvider LinphoneContactProvider;

typedef void (*ContactSearchCallback)( LinphoneContactSearch* id, bctbx_list_t* friends, void* data );

/*
 * Remote provisioning
 */

/**
 * Set URI where to download xml configuration file at startup.
 * This can also be set from configuration file or factory config file, from [misc] section, item "config-uri".
 * Calling this function does not load the configuration. It will write the value into configuration so that configuration
 * from remote URI will take place at next LinphoneCore start.
 * @param lc the linphone core
 * @param uri the http or https uri to use in order to download the configuration. Passing NULL will disable remote provisioning.
 * @return -1 if uri could not be parsed, 0 otherwise. Note that this does not check validity of URI endpoint nor scheme and download may still fail.
 * @ingroup initializing
**/
LINPHONE_PUBLIC int linphone_core_set_provisioning_uri(LinphoneCore *lc, const char*uri);

/**
 * Get provisioning URI.
 * @param lc the linphone core
 * @return the provisioning URI.
 * @ingroup initializing
**/
LINPHONE_PUBLIC const char* linphone_core_get_provisioning_uri(const LinphoneCore *lc);

/**
 * Gets if the provisioning URI should be removed after it's been applied successfully
 * @param lc the linphone core
 * @return TRUE if the provisioning URI should be removed, FALSE otherwise
 */
LINPHONE_PUBLIC bool_t linphone_core_is_provisioning_transient(LinphoneCore *lc);

/**
 * Migrate configuration so that all SIP transports are enabled.
 * Versions of linphone < 3.7 did not support using multiple SIP transport simultaneously.
 * This function helps application to migrate the configuration so that all transports are enabled.
 * Existing proxy configuration are added a transport parameter so that they continue using the unique transport that was set previously.
 * This function must be used just after creating the core, before any call to linphone_core_iterate()
 * @param lc the linphone core
 * @return 1 if migration was done, 0 if not done because unnecessary or already done, -1 in case of error.
 * @ingroup initializing
**/
LINPHONE_PUBLIC int linphone_core_migrate_to_multi_transport(LinphoneCore *lc);


/**
 * Control when media offer is sent in SIP INVITE.
 * @param lc the linphone core
 * @param enable true if INVITE has to be sent whitout SDP.
 * @ingroup network_parameters
**/
LINPHONE_PUBLIC void linphone_core_enable_sdp_200_ack(LinphoneCore *lc, bool_t enable);
/**
 * Media offer control param for SIP INVITE.
 * @return true if INVITE has to be sent whitout SDP.
 * @ingroup network_parameters
**/
LINPHONE_PUBLIC bool_t linphone_core_sdp_200_ack_enabled(const LinphoneCore *lc);


/**
 * Enum listing frequent telephony tones.
**/
enum _LinphoneToneID{
	LinphoneToneUndefined, /**<Not a tone */
	LinphoneToneBusy, /**<Busy tone */
	LinphoneToneCallWaiting, /**Call waiting tone */
	LinphoneToneCallOnHold, /**Call on hold tone */
	LinphoneToneCallLost /**Tone played when call is abruptly disconnected (media lost)*/
};

/**
 * Enum typedef for representing frequent telephony tones.
**/
typedef enum _LinphoneToneID LinphoneToneID;


/**
 * Assign an audio file to be played locally upon call failure, for a given reason.
 * @param lc the core
 * @param reason the #LinphoneReason representing the failure error code.
 * @param audiofile a wav file to be played when such call failure happens.
 * @ingroup misc
**/
LINPHONE_PUBLIC void linphone_core_set_call_error_tone(LinphoneCore *lc, LinphoneReason reason, const char *audiofile);

/**
 * Assign an audio file to be played as a specific tone id.
 * This function typically allows to customize telephony tones per country.
 * @param lc the core
 * @param id the tone id
 * @param audiofile a wav file to be played.
**/
LINPHONE_PUBLIC void linphone_core_set_tone(LinphoneCore *lc, LinphoneToneID id, const char *audiofile);

/**
 * Globaly set an http file transfer server to be used for content type application/vnd.gsma.rcs-ft-http+xml. This value can also be set for a dedicated account using #linphone_proxy_config_set_file_transfer_server
 * @param[in] core #LinphoneCore to be modified
 * @param[in] server_url URL of the file server like https://file.linphone.org/upload.php
 * @ingroup misc
 * */
LINPHONE_PUBLIC void linphone_core_set_file_transfer_server(LinphoneCore *core, const char * server_url);

/**
 * Get the globaly set http file transfer server to be used for content type application/vnd.gsma.rcs-ft-http+xml.
 * @param[in] core #LinphoneCore from which to get the server_url
 * @return URL of the file server like https://file.linphone.org/upload.php
 * @ingroup misc
 * */
LINPHONE_PUBLIC const char * linphone_core_get_file_transfer_server(LinphoneCore *core);

/**
 * Returns a null terminated table of strings containing the file format extension supported for call recording.
 * @param core the core
 * @return the supported formats, typically 'wav' and 'mkv'
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const char ** linphone_core_get_supported_file_formats(LinphoneCore *core);

/**
 * Returns whether a specific file format is supported.
 * @see linphone_core_get_supported_file_formats
 * @param lc A #LinphoneCore object
 * @param fmt The format extension (wav, mkv).
 * @ingroup media_paramaters
**/
LINPHONE_PUBLIC bool_t linphone_core_file_format_supported(LinphoneCore *lc, const char *fmt);

/**
 * This function controls signaling features supported by the core.
 * They are typically included in a SIP Supported header.
 * @param core the LinphoneCore
 * @param tag the feature tag name
 * @ingroup initializing
**/
LINPHONE_PUBLIC void linphone_core_add_supported_tag(LinphoneCore *core, const char *tag);

/**
 * Remove a supported tag. @see linphone_core_add_supported_tag()
 * @param core the LinphoneCore
 * @param tag the tag to remove
 * @ingroup initializing
**/
LINPHONE_PUBLIC void linphone_core_remove_supported_tag(LinphoneCore *core, const char *tag);

/**
 * Enable RTCP feedback (also known as RTP/AVPF profile).
 * Setting LinphoneAVPFDefault is equivalent to LinphoneAVPFDisabled.
 * This setting can be overriden per LinphoneProxyConfig with linphone_proxy_config_set_avpf_mode().
 * The value set here is used for calls placed or received out of any proxy configured, or if the proxy config is configured with LinphoneAVPFDefault.
 * @param lc the LinphoneCore
 * @param mode the mode.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_avpf_mode(LinphoneCore *lc, LinphoneAVPFMode mode);

/**
 * Return AVPF enablement. See linphone_core_set_avpf_mode() .
 * @param lc the core
 * @return the avpf enablement mode.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC LinphoneAVPFMode linphone_core_get_avpf_mode(const LinphoneCore *lc);

/**
 * Set the avpf report interval in seconds.
 * This value can be overriden by the proxy config using linphone_proxy_config_set_avpf_rr_interval().
 * @param lc the core
 * @param interval interval in seconds.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_set_avpf_rr_interval(LinphoneCore *lc, int interval);

/**
 * Return the avpf report interval in seconds.
 * @param lc the LinphoneCore
 * @return the avpf report interval in seconds.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_get_avpf_rr_interval(const LinphoneCore *lc);

/**
 * Use to set multicast address to be used for audio stream.
 * @param core #LinphoneCore
 * @param ip an ipv4/6 multicast address
 * @return 0 in case of success
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_set_audio_multicast_addr(LinphoneCore *core, const char* ip);
/**
 * Use to set multicast address to be used for video stream.
 * @param lc #LinphoneCore
 * @param ip an ipv4/6 multicast address
 * @return 0 in case of success
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_set_video_multicast_addr(LinphoneCore *lc, const char *ip);

/**
 * Use to get multicast address to be used for audio stream.
 * @param core #LinphoneCore
 * @return an ipv4/6 multicast address or default value
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const char* linphone_core_get_audio_multicast_addr(const LinphoneCore *core);

/**
 * Use to get multicast address to be used for video stream.
 * @param core #LinphoneCore
 * @return an ipv4/6 multicast address, or default value
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const char* linphone_core_get_video_multicast_addr(const LinphoneCore *core);

/**
 * Use to set multicast ttl to be used for audio stream.
 * @param core #LinphoneCore
 * @param ttl value or -1 if not used. [0..255] default value is 1
 * @return 0 in case of success
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_set_audio_multicast_ttl(LinphoneCore *core, int ttl);

/**
 * Use to set multicast ttl to be used for video stream.
 * @param lc #LinphoneCore
 * @param  ttl value or -1 if not used. [0..255] default value is 1
 * @return 0 in case of success
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_set_video_multicast_ttl(LinphoneCore *lc, int ttl);

/**
 * Use to get multicast ttl to be used for audio stream.
 * @param core #LinphoneCore
 * @return a time to leave value
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_get_audio_multicast_ttl(const LinphoneCore *core);

/**
 * Use to get multicast ttl to be used for video stream.
 * @param core #LinphoneCore
 * @return a time to leave value
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_get_video_multicast_ttl(const LinphoneCore *core);


/**
 * Use to enable multicast rtp for audio stream.
 * * If enabled, outgoing calls put a multicast address from #linphone_core_get_video_multicast_addr into audio cline. In case of outgoing call audio stream is sent to this multicast address.
 * <br> For incoming calls behavior is unchanged.
 * @param core #LinphoneCore
 * @param yesno if yes, subsequent calls will propose multicast ip set by #linphone_core_set_audio_multicast_addr
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_enable_audio_multicast(LinphoneCore *core, bool_t yesno);

/**
 * Use to get multicast state of audio stream.
 * @param core #LinphoneCore
 * @return true if  subsequent calls will propose multicast ip set by #linphone_core_set_audio_multicast_addr
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC bool_t linphone_core_audio_multicast_enabled(const LinphoneCore *core);

/**
 * Use to enable multicast rtp for video stream.
 * If enabled, outgoing calls put a multicast address from #linphone_core_get_video_multicast_addr into video cline. In case of outgoing call video stream is sent to this  multicast address.
 * <br> For incoming calls behavior is unchanged.
 * @param core #LinphoneCore
 * @param yesno if yes, subsequent outgoing calls will propose multicast ip set by #linphone_core_set_video_multicast_addr
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC void linphone_core_enable_video_multicast(LinphoneCore *core, bool_t yesno);
/**
 * Use to get multicast state of video stream.
 * @param core #LinphoneCore
 * @return true if  subsequent calls will propose multicast ip set by #linphone_core_set_video_multicast_addr
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC bool_t linphone_core_video_multicast_enabled(const LinphoneCore *core);

/**
 * Set the network simulator parameters.
 * Liblinphone has the capabability of simulating the effects of a network (latency, lost packets, jitter, max bandwidth).
 * Please refer to the oRTP documentation for the meaning of the parameters of the OrtpNetworkSimulatorParams structure.
 * This function has effect for future calls, but not for currently running calls, though this behavior may be changed in future versions.
 * @warning Due to design of network simulation in oRTP, simulation is applied independently for audio and video stream. This means for example that a bandwidth
 * limit of 250kbit/s will have no effect on an audio stream running at 40kbit/s while a videostream targetting 400kbit/s will be highly affected.
 * @param lc the LinphoneCore
 * @param params the parameters used for the network simulation.
 * @return 0 if successful, -1 otherwise.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC int linphone_core_set_network_simulator_params(LinphoneCore *lc, const OrtpNetworkSimulatorParams *params);


/**
 * Get the previously set network simulation parameters.
 * @see linphone_core_set_network_simulator_params
 * @return a OrtpNetworkSimulatorParams structure.
 * @ingroup media_parameters
**/
LINPHONE_PUBLIC const OrtpNetworkSimulatorParams *linphone_core_get_network_simulator_params(const LinphoneCore *lc);

/**
 * Set the video preset to be used for video calls.
 * @param[in] lc LinphoneCore object
 * @param[in] preset The name of the video preset to be used (can be NULL to use the default video preset).
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC void linphone_core_set_video_preset(LinphoneCore *lc, const char *preset);

/**
 * Get the video preset used for video calls.
 * @param[in] lc LinphoneCore object
 * @return The name of the video preset used for video calls (can be NULL if the default video preset is used).
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC const char * linphone_core_get_video_preset(const LinphoneCore *lc);

/**
 * Gets if realtime text is enabled or not
 * @param[in] lc LinphoneCore object
 * @return true if realtime text is enabled, false otherwise
 * @ingroup media_parameters
 */
LINPHONE_PUBLIC bool_t linphone_core_realtime_text_enabled(LinphoneCore *lc);

/**
 * Set http proxy address to be used for signaling during next channel connection. Use #linphone_core_set_network_reachable FASLE/TRUE to force channel restart.
 * @param[in] lc LinphoneCore object
 * @param[in] host Hostname of IP adress of the http proxy (can be NULL to disable).
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	void linphone_core_set_http_proxy_host(LinphoneCore *lc, const char *host) ;

/**
 * Set http proxy port to be used for signaling.
 * @param[in] lc LinphoneCore object
 * @param[in] port of the http proxy.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC void linphone_core_set_http_proxy_port(LinphoneCore *lc, int port) ;

/**
 * Get http proxy address to be used for signaling.
 * @param[in] lc LinphoneCore object
 * @return hostname of IP adress of the http proxy (can be NULL to disable).
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	const char *linphone_core_get_http_proxy_host(const LinphoneCore *lc);

/**
 * Get http proxy port to be used for signaling.
 * @param[in] lc LinphoneCore object
 * @return port of the http proxy.
 * @ingroup network_parameters
 */
LINPHONE_PUBLIC	int linphone_core_get_http_proxy_port(const LinphoneCore *lc);

/**
 * Converts a LinphoneTransportType enum to a lowercase string.
 * @ingroup misc
**/
LINPHONE_PUBLIC const char* linphone_transport_to_string(LinphoneTransportType transport);

/**
 * Converts a lowercase string to a LinphoneTransportType enum.
 * @ingroup misc
 * @return Transport matching input, or LinphoneTransportUdp if nothing is found
**/
LINPHONE_PUBLIC LinphoneTransportType linphone_transport_parse(const char* transport);

typedef struct _LinphoneRingtonePlayer LinphoneRingtonePlayer;

LINPHONE_PUBLIC LinphoneRingtonePlayer *linphone_core_get_ringtoneplayer(LinphoneCore *lc);

/**
 * @ingroup network_parameters
 * Sets a TLS certificate used for TLS authentication
 * The certificate won't be stored, you have to set it after each LinphoneCore startup
 * @param lc LinphoneCore object
 * @param tls_cert the TLS certificate
 */
LINPHONE_PUBLIC void linphone_core_set_tls_cert(LinphoneCore *lc, const char *tls_cert);

/**
 * @ingroup network_parameters
 * Sets a TLS key used for TLS authentication
 * The key won't be stored, you have to set it after each LinphoneCore startup
 * @param lc LinphoneCore object
 * @param tls_key the TLS key
 */
LINPHONE_PUBLIC void linphone_core_set_tls_key(LinphoneCore *lc, const char *tls_key);

/**
 * @ingroup network_parameters
 * Sets a TLS certificate path used for TLS authentication
 * The path will be stored in the rc file and automatically restored on startup
 * @param lc LinphoneCore object
 * @param tls_cert_path path to the TLS certificate
 */
LINPHONE_PUBLIC void linphone_core_set_tls_cert_path(LinphoneCore *lc, const char *tls_cert_path);

/**
 * @ingroup network_parameters
 * Sets a TLS key path used for TLS authentication
 * The path will be stored in the rc file and automatically restored on startup
 * @param lc LinphoneCore object
 * @param tls_key_path path to the TLS key
 */
LINPHONE_PUBLIC void linphone_core_set_tls_key_path(LinphoneCore *lc, const char *tls_key_path);

/**
 * @ingroup network_parameters
 * Gets the TLS certificate
 * @param lc LinphoneCore object
 * @return the TLS certificate or NULL if not set yet
 */
LINPHONE_PUBLIC const char *linphone_core_get_tls_cert(const LinphoneCore *lc);

/**
 * @ingroup network_parameters
 * Gets the TLS key
 * @param lc LinphoneCore object
 * @return  the TLS key or NULL if not set yet
 */
LINPHONE_PUBLIC const char *linphone_core_get_tls_key(const LinphoneCore *lc);

/**
 * @ingroup network_parameters
 * Gets the path to the TLS certificate file
 * @param lc LinphoneCore object
 * @return the TLS certificate path or NULL if not set yet
 */
LINPHONE_PUBLIC const char *linphone_core_get_tls_cert_path(const LinphoneCore *lc);

/**
 * @ingroup network_parameters
 * Gets the path to the TLS key file
 * @param lc LinphoneCore object
 * @return the TLS key path or NULL if not set yet
 */
LINPHONE_PUBLIC const char *linphone_core_get_tls_key_path(const LinphoneCore *lc);


#include "linphone/ringtoneplayer.h"
#include "linphone/factory.h"

#ifdef __cplusplus
}
#endif

#endif
