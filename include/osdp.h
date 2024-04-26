/*
 * Copyright (c) 2019-2024 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Open Supervised Device Protocol (OSDP) public API header file.
 */

#ifndef _OSDP_H_
#define _OSDP_H_

#include <stdint.h>
#include <stdbool.h>
#include <osdp_export.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief OSDP setup flags. See osdp_pd_info_t::flags
 */

/**
 * @brief ENFORCE_SECURE: Make security conscious assumptions (see below) where
 * possible. Fail where these assumptions don't hold.
 *   - Don't allow use of SCBK-D.
 *   - Assume that a KEYSET was successful at an earlier time.
 *
 * @note This flag is recommended in production use.
 */
#define OSDP_FLAG_ENFORCE_SECURE 0x00010000

/**
 * @brief When set, the PD would allow one session of secure channel to be
 * setup with SCBK-D.
 *
 * @note In this mode, the PD is in a vulnerable state, the application is
 * responsible for making sure that the device enters this mode only during
 * controlled/provisioning-time environments.
 */
#define OSDP_FLAG_INSTALL_MODE 0x00020000

/**
 * @brief When set, CP will not error and fail when the PD sends an unknown,
 * unsolicited response (in response to osdp_POLL command).
 *
 * @note In PD mode this flag has no use.
 */
#define OSDP_FLAG_IGN_UNSOLICITED 0x00040000

/**
 * @brief Various PD capability function codes.
 */
enum osdp_pd_cap_function_code_e {
	/**
	 * Dummy.
	 */
	OSDP_PD_CAP_UNUSED,

	/**
	 * This function indicates the ability to monitor the status of a switch
	 * using a two-wire electrical connection between the PD and the switch.
	 * The on/off position of the switch indicates the state of an external
	 * device.
	 *
	 * The PD may simply resolve all circuit states to an open/closed
	 * status, or it may implement supervision of the monitoring circuit. A
	 * supervised circuit is able to indicate circuit fault status in
	 * addition to open/closed status.
	 */
	OSDP_PD_CAP_CONTACT_STATUS_MONITORING,

	/**
	 * This function provides a switched output, typically in the form of a
	 * relay. The Output has two states: active or inactive. The Control
	 * Panel (CP) can directly set the Output's state, or, if the PD
	 * supports timed operations, the CP can specify a time period for the
	 * activation of the Output.
	 */
	OSDP_PD_CAP_OUTPUT_CONTROL,

	/**
	 * This capability indicates the form of the card data is presented to
	 * the Control Panel.
	 */
	OSDP_PD_CAP_CARD_DATA_FORMAT,

	/**
	 * This capability indicates the presence of and type of LEDs.
	 */
	OSDP_PD_CAP_READER_LED_CONTROL,

	/**
	 * This capability indicates the presence of and type of an Audible
	 * Annunciator (buzzer or similar tone generator)
	 */
	OSDP_PD_CAP_READER_AUDIBLE_OUTPUT,

	/**
	 * This capability indicates that the PD supports a text display
	 * emulating character-based display terminals.
	 */
	OSDP_PD_CAP_READER_TEXT_OUTPUT,

	/**
	 * This capability indicates that the type of date and time awareness
	 * or time keeping ability of the PD.
	 */
	OSDP_PD_CAP_TIME_KEEPING,

	/**
	 * All PDs must be able to support the checksum mode. This capability
	 * indicates if the PD is capable of supporting CRC mode.
	 */
	OSDP_PD_CAP_CHECK_CHARACTER_SUPPORT,

	/**
	 * This capability indicates the extent to which the PD supports
	 * communication security (Secure Channel Communication)
	 */
	OSDP_PD_CAP_COMMUNICATION_SECURITY,

	/**
	 * This capability indicates the maximum size single message the PD can
	 * receive.
	 */
	OSDP_PD_CAP_RECEIVE_BUFFERSIZE,

	/**
	 * This capability indicates the maximum size multi-part message which
	 * the PD can handle.
	 */
	OSDP_PD_CAP_LARGEST_COMBINED_MESSAGE_SIZE,

	/**
	 * This capability indicates whether the PD supports the transparent
	 * mode used for communicating directly with a smart card.
	 */
	OSDP_PD_CAP_SMART_CARD_SUPPORT,

	/**
	 * This capability indicates the number of credential reader devices
	 * present. Compliance levels are bit fields to be assigned as needed.
	 */
	OSDP_PD_CAP_READERS,

	/**
	 * This capability indicates the ability of the reader to handle
	 * biometric input
	 */
	OSDP_PD_CAP_BIOMETRICS,

	/**
	 * Capability Sentinel
	 */
	OSDP_PD_CAP_SENTINEL
};

/**
 * @brief OSDP specified NAK codes
 */
enum osdp_pd_nak_code_e {
	OSDP_PD_NAK_NONE,     /**< No error */
	OSDP_PD_NAK_MSG_CHK,  /**< Message check character(s) error (bad cksum/crc) */
	OSDP_PD_NAK_CMD_LEN,  /**< Command length error */
	OSDP_PD_NAK_CMD_UNKNOWN, /**< Unknown Command Code – Command not implemented by PD */
	OSDP_PD_NAK_SEQ_NUM,  /**< Sequence number error */
	OSDP_PD_NAK_SC_UNSUP, /**< Secure Channel is not supported by PD */
	OSDP_PD_NAK_SC_COND,  /**< unsupported security block or security conditions not met */
	OSDP_PD_NAK_BIO_TYPE, /**< BIO_TYPE not supported */
	OSDP_PD_NAK_BIO_FMT,  /**< BIO_FORMAT not supported */
	OSDP_PD_NAK_RECORD,   /**< Unable to process command record */
	OSDP_PD_NAK_SENTINEL  /**< NAK codes max value */
};

/**
 * @brief PD capability structure. Each PD capability has a 3 byte
 * representation.
 */
struct osdp_pd_cap {
	/**
	 * Capability function code. See @ref osdp_pd_cap_function_code_e
	 */
	uint8_t function_code;
	/**
	 * A function_code dependent number that indicates what the PD can do
	 * with this capability.
	 */
	uint8_t compliance_level;
	/**
	 * Number of such capability entities in PD
	 */
	uint8_t num_items;
};

/**
 * @brief PD ID information advertised by the PD.
 */
struct osdp_pd_id {
	int version;               /**< 1-Byte Manufacturer's version number */
	int model;                 /**< 1-byte Manufacturer's model number */
	uint32_t vendor_code;      /**< 3-bytes IEEE assigned OUI */
	uint32_t serial_number;    /**< 4-byte serial number for the PD */
	uint32_t firmware_version; /**< 3-byte version (major, minor, build) */
};

/**
 * @brief pointer to function that copies received bytes into buffer. This
 * function should be non-blocking.
 *
 * @param data for use by underlying layers. osdp_channel::data is passed
 * @param buf byte array copy incoming data
 * @param maxlen sizeof `buf`. Can copy utmost `maxlen` bytes into `buf`
 *
 * @retval +ve: number of bytes copied on to `buf`. Must be <= `len`
 * @retval -ve on errors
 */
typedef int (*osdp_read_fn_t)(void *data, uint8_t *buf, int maxlen);

/**
 * @brief pointer to function that sends byte array into some channel. This
 * function should be non-blocking.
 *
 * @param data for use by underlying layers. osdp_channel::data is passed
 * @param buf byte array to be sent
 * @param len number of bytes in `buf`
 *
 * @retval +ve: number of bytes sent. must be <= `len`
 * @retval -ve on errors
 *
 * @note For now, LibOSDP expects method to write/queue all or no bytes over
 * the channel per-invocation; ie., it does not support partial writes and is a
 * known limitation. Since an OSDP packet isn't so large, and typical TX
 * buffers are much larger than that, it's not as bad as it sounds and hence
 * not on the priority list to be fixed.
 */
typedef int (*osdp_write_fn_t)(void *data, uint8_t *buf, int len);

/**
 * @brief pointer to function that drops all bytes in TX/RX fifo. This
 * function should be non-blocking.
 *
 * @param data for use by underlying layers. osdp_channel::data is passed
 */
typedef void (*osdp_flush_fn_t)(void *data);

/**
 * @brief User defined communication channel abstraction for OSDP devices.
 * The methods for read/write/flush are expected to be non-blocking.
 */
struct osdp_channel {
	/**
	 * pointer to a block of memory that will be passed to the
	 * send/receive/flush method. This is optional (can be set to NULL)
	 */
	void *data;
	/**
	 * channel_id; On multi-drop networks, more than one PD can share the
	 * same channel (read/write/flush pointers). On such networks, the
	 * channel_id is used to lock a PD to a channel. On multi-drop
	 * networks, this `id` must non-zero and be unique for each bus.
	 */
	int id;
	/**
	 * Pointer to function used to receive osdp packet data
	 */
	osdp_read_fn_t recv;
	/**
	 * Pointer to function used to send osdp packet data
	 */
	osdp_write_fn_t send;
	/**
	 * Pointer to function used to flush the channel (optional)
	 */
	osdp_flush_fn_t flush;
};

/**
 * @brief OSDP PD Information. This struct is used to describe a PD to LibOSDP.
 */
typedef struct {
	/**
	 * User provided name for this PD (log messages include this name)
	 */
	const char *name;
	/**
	 * Can be one of 9600/19200/38400/57600/115200/230400
	 */
	int baud_rate;
	/**
	 * 7 bit PD address. the rest of the bits are ignored. The special
	 * address 0x7F is used for broadcast. So there can be 2^7-1 devices on
	 * a multi-drop channel
	 */
	int address;
	/**
	 * Used to modify the way the context is setup. See `OSDP_FLAG_*`
	 * macros.
	 */
	int flags;
	/**
	 * Static information that the PD reports to the CP when it received a
	 * `CMD_ID`. These information must be populated by a PD application.
	 */
	struct osdp_pd_id id;
	/**
	 * This is a pointer to an array of structures containing the PD'
	 * capabilities. Use { -1, 0, 0 } to terminate the array. This is used
	 * only PD mode of operation
	 */
	const struct osdp_pd_cap *cap;
	/**
	 * Communication channel ops structure, containing send/recv function
	 * pointers
	 */
	struct osdp_channel channel;
	/**
	 * Pointer to 16 bytes of Secure Channel Base Key for the PD. If
	 * non-null, this is used to set-up the secure channel.
	 */
	const uint8_t *scbk;
} osdp_pd_info_t;

/**
 * @brief To keep the OSDP internal data structures from polluting the exposed
 * headers, they are typedefed to void before sending them to the upper layers.
 * This level of abstraction looked reasonable as _technically_ no one should
 * attempt to modify it outside of the LibOSDP and their definition may change
 * at any time.
 */
typedef void osdp_t;

/**
 * @brief OSDP Status report types
 */
enum osdp_status_report_type {
	/**
	 * @brief Status report of the inputs attached the PD
	 */
	OSDP_STATUS_REPORT_INPUT,
	/**
	 * @brief Status report of the output attached the PD
	 */
	OSDP_STATUS_REPORT_OUTPUT,
	/**
	 * @brief Local tamper and power status report
	 *
	 * Bit-0: tamper
	 * Bit-1: power
	 */
	OSDP_STATUS_REPORT_LOCAL,
	/**
	 * @brief Remote tamper and power status report
	 *
	 * Bit-0: tamper
	 * Bit-1: power
	 */
	OSDP_STATUS_REPORT_REMOTE,
};

/**
 * @brief Status report structure. Used by OSDP_CMD_STATUS and
 * OSDP_EVENT_STATUS. In case of command, it is used to send a query to the PD
 * while in the case of events, the PR responds back with this structure.
 *
 * This can is used by the PD to indicate various status change reports. Upto a
 * maximum of 32 statuses can be reported using this API.
 */
struct osdp_status_report {
	/**
	 * The kind of event to report see `enum osdp_event_status_type_e`
	 */
	enum osdp_status_report_type type;
	/**
	 * Number of valid bits in `status`
	 */
	int nr_entries;
	/**
	 * Status bit mask
	 */
	uint32_t mask;
};

/* ------------------------------- */
/*         OSDP Commands           */
/* ------------------------------- */

#define OSDP_CMD_TEXT_MAX_LEN          32
#define OSDP_CMD_KEYSET_KEY_MAX_LEN    32
#define OSDP_CMD_MFG_MAX_DATALEN       64

#define OSDP_CMD_FILE_TX_FLAG_CANCEL (1UL << 31)

/**
 * @brief Command sent from CP to Control digital output of PD.
 */
struct osdp_cmd_output {
	/**
	 * 0 = First Output, 1 = Second Output, etc.
	 */
	uint8_t output_no;
	/**
	 * One of the following:
	 *    0 - NOP – do not alter this output
	 *    1 - set the permanent state to OFF, abort timed operation (if any)
	 *    2 - set the permanent state to ON, abort timed operation (if any)
	 *    3 - set the permanent state to OFF, allow timed operation to complete
	 *    4 - set the permanent state to ON, allow timed operation to complete
	 *    5 - set the temporary state to ON, resume perm state on timeout
	 *    6 - set the temporary state to OFF, resume permanent state on timeout
	 */
	uint8_t control_code;
	/**
	 * Time in units of 100 ms
	 */
	uint16_t timer_count;
};

/**
 * @brief LED Colors as specified in OSDP for the on_color/off_color
 * parameters.
 */
enum osdp_led_color_e {
	OSDP_LED_COLOR_NONE,     /**< No color */
	OSDP_LED_COLOR_RED,      /**< Red */
	OSDP_LED_COLOR_GREEN,    /**< Green */
	OSDP_LED_COLOR_AMBER,    /**< Amber */
	OSDP_LED_COLOR_BLUE,     /**< Blue */
	OSDP_LED_COLOR_MAGENTA,  /**< Magenta */
	OSDP_LED_COLOR_CYAN,     /**< Cyan */
	OSDP_LED_COLOR_WHITE,    /**< White */
	OSDP_LED_COLOR_SENTINEL  /**< Max value */
};

/**
 * @brief LED params sub-structure. Part of LED command. See @ref osdp_cmd_led.
 */
struct osdp_cmd_led_params {
	/** Control code.
	 *
	 * Temporary Control Code:
	 * - 0 - NOP - do not alter this LED's temporary settings.
	 * - 1 - Cancel any temporary operation and display this LED's
	 *       permanent state immediately.
	 * - 2 - Set the temporary state as given and start timer immediately.
	 *
	 * Permanent Control Code:
	 * - 0 - NOP - do not alter this LED's permanent settings.
	 * - 1 - Set the permanent state as given.
	 */
	uint8_t control_code;
	/**
	 * The ON duration of the flash, in units of 100 ms.
	 */
	uint8_t on_count;
	/**
	 * The OFF duration of the flash, in units of 100 ms.
	 */
	uint8_t off_count;
	/**
	 * Color to set during the ON timer (see @ref osdp_led_color_e).
	 */
	uint8_t on_color;
	/**
	 * Color to set during the OFF timer (see @ref osdp_led_color_e).
	 */
	uint8_t off_color;
	/**
	 * Time in units of 100 ms (only for temporary mode).
	 */
	uint16_t timer_count;
};

/**
 * @brief Sent from CP to PD to control the behaviour of it's on-board LEDs
 */
struct osdp_cmd_led {
	/**
	 * Reader number. 0 = First Reader, 1 = Second Reader, etc.
	 */
	uint8_t reader;
	/**
	 * LED number. 0 = first LED, 1 = second LED, etc.
	 */
	uint8_t led_number;
	/**
	 * Ephemeral LED status descriptor.
	 */
	struct osdp_cmd_led_params temporary;
	/**
	 * Permanent LED status descriptor.
	 */
	struct osdp_cmd_led_params permanent;
};

/**
 * @brief Sent from CP to control the behaviour of a buzzer in the PD.
 */
struct osdp_cmd_buzzer {
	/**
	 * Reader number. 0 = First Reader, 1 = Second Reader, etc.
	 */
	uint8_t reader;
	/**
	 * Control code.
	 * - 0 - no tone
	 * - 1 - off
	 * - 2 - default tone
	 * - 3+ - TBD
	 */
	uint8_t control_code;
	/**
	 * The ON duration of the sound, in units of 100 ms.
	 */
	uint8_t on_count;
	/**
	 * The OFF duration of the sound, in units of 100 ms.
	 */
	uint8_t off_count;
	/**
	 * The number of times to repeat the ON/OFF cycle; 0: forever.
	 */
	uint8_t rep_count;
};

/**
 * @brief Command to manipulate any display units that the PD supports.
 */
struct osdp_cmd_text {
	/**
	 * Reader number. 0 = First Reader, 1 = Second Reader, etc.
	 */
	uint8_t reader;
	/**
	 * Control code.
	 * - 1 - permanent text, no wrap
	 * - 2 - permanent text, with wrap
	 * - 3 - temp text, no wrap
	 * - 4 - temp text, with wrap
	 */
	uint8_t control_code;
	/**
	 * Duration to display temporary text, in seconds
	 */
	uint8_t temp_time;
	/**
	 * Row to display the first character (1-indexed)
	 */
	uint8_t offset_row;
	/**
	 * Column to display the first character (1-indexed)
	 */
	uint8_t offset_col;
	/**
	 * Number of characters in the string
	 */
	uint8_t length;
	/**
	 * The string to display
	 */
	uint8_t data[OSDP_CMD_TEXT_MAX_LEN];
};

/**
 * @brief Sent in response to a COMSET command. Set communication parameters to
 * PD. Must be stored in PD non-volatile memory.
 */
struct osdp_cmd_comset {
	/**
	 * Unit ID to which this PD will respond after the change takes effect.
	 */
	uint8_t address;
	/**
	 * Baud rate.
	 *
	 * Valid values: 9600, 19200, 38400, 115200, 230400.
	 */
	uint32_t baud_rate;
};

/**
 * @brief This command transfers an encryption key from the CP to a PD.
 */
struct osdp_cmd_keyset {
	/**
	 * Type of keys:
	 * - 0x01 – Secure Channel Base Key
	 */
	uint8_t type;
	/**
	 * Number of bytes of key data - (Key Length in bits + 7) / 8
	 */
	uint8_t length;
	/**
	 * Key data
	 */
	uint8_t data[OSDP_CMD_KEYSET_KEY_MAX_LEN];
};

/**
 * @brief Manufacturer Specific Commands
 */
struct osdp_cmd_mfg {
	/**
	 * 3-byte IEEE assigned OUI. Most Significant 8-bits are unused
	 */
	uint32_t vendor_code;
	/**
	 * 1-byte manufacturer defined osdp command
	 */
	uint8_t command;
	/**
	 * length Length of command data (optional)
	 */
	uint8_t length;
	/**
	 * Command data (optional)
	 */
	uint8_t data[OSDP_CMD_MFG_MAX_DATALEN];
};

/**
 * @brief File transfer start command
 */
struct osdp_cmd_file_tx {
	/**
	 * Pre-agreed file ID between CP and PD
	 */
	int id;
	/**
	 * Reserved and set to zero by OSDP spec.
	 *
	 * @note: The upper bits are used by libosdp as:
	 *    bit-31 - OSDP_CMD_FILE_TX_FLAG_CANCEL: cancel an ongoing transfer
	 */
	uint32_t flags;
};

/**
 * @brief OSDP application exposed commands
 */
enum osdp_cmd_e {
	OSDP_CMD_OUTPUT = 1,  /**< Output control command */
	OSDP_CMD_LED,         /**< Reader LED control command */
	OSDP_CMD_BUZZER,      /**< Reader buzzer control command */
	OSDP_CMD_TEXT,        /**< Reader text output command */
	OSDP_CMD_KEYSET,      /**< Encryption Key Set Command */
	OSDP_CMD_COMSET,      /**< PD communication configuration command */
	OSDP_CMD_MFG,         /**< Manufacturer specific command */
	OSDP_CMD_FILE_TX,     /**< File transfer comand */
	OSDP_CMD_STATUS,      /**< Status report eommand */
	OSDP_CMD_SENTINEL     /**< Max command value */
};

/**
 * @brief OSDP Command Structure. This is a wrapper for all individual OSDP
 * commands.
 */
struct osdp_cmd {
	/**
	 * Command ID. Used to select specific commands in union.
	 */
	enum osdp_cmd_e id;
	/** Command */
	union {
		struct osdp_cmd_led led;          /**< LED command structure */
		struct osdp_cmd_buzzer buzzer;    /**< Buzzer command structure */
		struct osdp_cmd_text text;        /**< Text command structure */
		struct osdp_cmd_output output;    /**< Output command structure */
		struct osdp_cmd_comset comset;    /**< Comset command structure */
		struct osdp_cmd_keyset keyset;    /**< Keyset command structure */
		struct osdp_cmd_mfg mfg;          /**< Manufacturer specific command structure */
		struct osdp_cmd_file_tx file_tx;  /**< File transfer command structure */
		struct osdp_status_report status; /**< Status report command structure */
	};
};

/* ------------------------------- */
/*          OSDP Events            */
/* ------------------------------- */

#define OSDP_EVENT_CARDREAD_MAX_DATALEN   256
#define OSDP_EVENT_KEYPRESS_MAX_DATALEN   64
#define OSDP_EVENT_MFGREP_MAX_DATALEN     256

/**
 * @brief Various card formats that a PD can support. This is sent to CP
 * when a PD must report a card read.
 */
enum osdp_event_cardread_format_e {
	OSDP_CARD_FMT_RAW_UNSPECIFIED, /**< Unspecified card format */
	OSDP_CARD_FMT_RAW_WIEGAND,     /**< Wiegand card format */
	OSDP_CARD_FMT_ASCII,           /**< ASCII card format */
	OSDP_CARD_FMT_SENTINEL         /**< Max card format value */
};

/**
 * @brief OSDP event cardread
 *
 * @note When @a format is set to OSDP_CARD_FMT_RAW_UNSPECIFIED or
 * OSDP_CARD_FMT_RAW_WIEGAND, the length is expressed in bits. OTOH, when it is
 * set to OSDP_CARD_FMT_ASCII, the length is in bytes. The number of bytes to
 * read from the @a data field must be interpreted accordingly.
 */
struct osdp_event_cardread {
	/**
	 * Reader number. 0 = First Reader, 1 = Second Reader, etc.
	 */
	int reader_no;
	/**
	 * Format of the card being read.
	 */
	enum osdp_event_cardread_format_e format;
	/**
	 * Direction of data in @a data array.
	 * - 0 - Forward
	 * - 1 - Backward
	 */
	int direction;
	/**
	 * Length of card data in bytes or bits depending on @a format
	 */
	int length;
	/**
	 * Card data of @a length bytes or bits bits depending on @a format
	 */
	uint8_t data[OSDP_EVENT_CARDREAD_MAX_DATALEN];
};

/**
 * @brief OSDP Event Keypad
 */
struct osdp_event_keypress {
	/**
	 * Reader number in context of sub-readers attached to current PD; this
	 * number indicates the number of that reader. This is not supported by
	 * LibOSDP.
	 */
	int reader_no;
	/**
	 * Length of keypress data in bytes
	 */
	int length;
	/**
	 * Keypress data of @a length bytes
	 */
	uint8_t data[OSDP_EVENT_KEYPRESS_MAX_DATALEN];
};

/**
 * @brief OSDP Event Manufacturer Specific Command
 *
 * @note OSDP spec v2.2 makes this structure fixed at 4 bytes (3-byte vendor
 * code and 1-byte data). LibOSDP allows for some additional data to be passed
 * in this command using the @a data and @a length fields while using the
 * 1-byte data (as specified in the specification) as @a command. To be fully
 * compliant with the specification, you can set @a length to 0.
 */
struct osdp_event_mfgrep {
	/**
	 * 3-bytes IEEE assigned OUI of manufacturer
	 */
	uint32_t vendor_code;
	/**
	 * 1-byte reply code
	 */
	uint8_t command;
	/**
	 * Length of manufacturer data in bytes (optional)
	 */
	uint16_t length;
	/**
	 * Manufacturer data of `length` bytes (optional)
	 */
	uint8_t data[OSDP_EVENT_MFGREP_MAX_DATALEN];
};

/**
 * @brief OSDP PD Events
 */
enum osdp_event_type {
	OSDP_EVENT_CARDREAD = 1,  /**< Card read event */
	OSDP_EVENT_KEYPRESS,      /**< Keypad press event */
	OSDP_EVENT_MFGREP,        /**< Manufacturer specific reply event */
	OSDP_EVENT_STATUS,        /**< Status event */

	// TODO: to add in FMT QR response for smarfid reader
	OSDP_EVENT_QR_CODE,
	OSDP_EVENT_PD_OFFLINE,
	OSDP_EVENT_PD_ONLINE,
	OSDP_EVENT_PD_ONLINE_WITH_SC,
	OSDP_EVENT_PD_SC_ESTABLISH,
	OSDP_EVENT_SENTINEL       /**< Max event value */
};

/**
 * @brief OSDP Event structure.
 */
struct osdp_event {
	/**
	 * Event type. Used to select specific event in union.
	 */
	enum osdp_event_type type;
	/** Event */
	union {
		struct osdp_event_keypress keypress; /**< Keypress event structure */
		struct osdp_event_cardread cardread; /**< Card read event structure */
		struct osdp_event_mfgrep mfgrep;     /**< Manufacturer spefific response event struture */
		struct osdp_status_report status;    /**< Status report event strcture */
	};
};

/**
 * @brief Callback for PD command notifications. After it has been registered
 * with `osdp_pd_set_command_callback`, this method is invoked when the PD
 * receives a command from the CP.
 *
 * @param arg pointer that will was passed to the arg param of
 * `osdp_pd_set_command_callback`.
 * @param cmd pointer to the received command.
 *
 * @retval 0 if LibOSDP must send a `osdp_ACK` response
 * @retval -ve if LibOSDP must send a `osdp_NAK` response
 * @retval +ve and modify the passed `struct osdp_cmd *cmd` if LibOSDP must
 * send a specific response. This is useful for sending manufacturer specific
 * reply `osdp_MFGREP`.
 */
typedef int (*pd_command_callback_t)(void *arg, struct osdp_cmd *cmd);

/**
 * @brief Callback for CP event notifications. After it has been registered
 * with `osdp_cp_set_event_callback`, this method is invoked when the CP
 * receives an event from the PD.
 *
 * @param arg Opaque pointer provided by the application during callback
 * registration.
 * @param pd PD offset (0-indexed) of this PD in `osdp_pd_info_t *` passed to
 * osdp_cp_setup()
 * @param ev pointer to osdp_event struct (filled by libosdp).
 *
 * @retval 0 on handling the event successfully.
 * @retval -ve on errors.
 */
typedef int (*cp_event_callback_t)(void *arg, int pd, struct osdp_event *ev);

/* ------------------------------- */
/*            PD Methods           */
/* ------------------------------- */

/**
 * @brief This method is used to setup a device in PD mode. Application must
 * store the returned context pointer and pass it back to all OSDP functions
 * intact.
 *
 * @param info Pointer to info struct populated by application.
 *
 * @retval OSDP Context on success
 * @retval NULL on errors
 */
OSDP_EXPORT
osdp_t *osdp_pd_setup(const osdp_pd_info_t *info);

/**
 * @brief Periodic refresh method. Must be called by the application at least
 * once every 50ms to meet OSDP timing requirements.
 *
 * @param ctx OSDP context
 */
OSDP_EXPORT
void osdp_pd_refresh(osdp_t *ctx);

/**
 * @brief Cleanup all osdp resources. The context pointer is no longer valid
 * after this call.
 *
 * @param ctx OSDP context
 */
OSDP_EXPORT
void osdp_pd_teardown(osdp_t *ctx);

/**
 * @brief Set PD's capabilities
 *
 * @param ctx OSDP context
 * @param cap pointer to array of cap (`struct osdp_pd_cap`) terminated by a
 * capability with cap->function_code set to 0.
 */
OSDP_EXPORT
void osdp_pd_set_capabilities(osdp_t *ctx, const struct osdp_pd_cap *cap);

/**
 * @brief Set callback method for PD command notification. This callback is
 * invoked when the PD receives a command from the CP.
 *
 * @param ctx OSDP context
 * @param cb The callback function's pointer
 * @param arg A pointer that will be passed as the first argument of `cb`
 */
OSDP_EXPORT
void osdp_pd_set_command_callback(osdp_t *ctx, pd_command_callback_t cb,
				  void *arg);

/**
 * @brief API to notify PD events to CP. These events are sent to the CP as an
 * alternate response to a POLL command.
 *
 * @param ctx OSDP context
 * @param event pointer to event struct. Must be filled by application.
 *
 * @retval 0 on success
 * @retval -1 on failure
 */
OSDP_EXPORT
int osdp_pd_notify_event(osdp_t *ctx, const struct osdp_event *event);

/**
 * @brief Deletes all events from the PD's event queue.
 *
 * @param ctx OSDP context
 * @return int Count of events dequeued.
 */
OSDP_EXPORT
int osdp_pd_flush_events(osdp_t *ctx);

/* ------------------------------- */
/*            CP Methods           */
/* ------------------------------- */

/**
 * @brief This method is used to setup a device in CP mode. Application must
 * store the returned context pointer and pass it back to all OSDP functions
 * intact.
 *
 * @param num_pd Number of PDs connected to this CP. The `osdp_pd_info_t *` is
 * treated as an array of length num_pd.
 * @param info Pointer to info struct populated by application.
 *
 * @retval OSDP Context on success
 * @retval NULL on errors
 */
OSDP_EXPORT
osdp_t *osdp_cp_setup(int num_pd, const osdp_pd_info_t *info);

/**
 * @brief Periodic refresh method. Must be called by the application at least
 * once every 50ms to meet OSDP timing requirements.
 *
 * @param ctx OSDP context
 */
OSDP_EXPORT
void osdp_cp_refresh(osdp_t *ctx);

/**
 * @brief Cleanup all osdp resources. The context pointer is no longer valid
 * after this call.
 *
 * @param ctx OSDP context
 */
OSDP_EXPORT
void osdp_cp_teardown(osdp_t *ctx);

/**
 * @brief Generic command enqueue API.
 *
 * @param ctx OSDP context
 * @param pd PD offset (0-indexed) of this PD in `osdp_pd_info_t *` passed to
 * osdp_cp_setup()
 * @param cmd command pointer. Must be filled by application.
 *
 * @retval 0 on success
 * @retval -1 on failure
 *
 * @note This method only adds the command on to a particular PD's command
 * queue. The command itself can fail due to various reasons.
 */
OSDP_EXPORT
int osdp_cp_send_command(osdp_t *ctx, int pd, const struct osdp_cmd *cmd);

/**
 * @brief Deletes all commands queued for a give PD
 *
 * @param ctx OSDP context
 * @param pd PD offset (0-indexed) of this PD in `osdp_pd_info_t *` passed to
 * osdp_cp_setup()
 * @return int Count of events dequeued
 */
OSDP_EXPORT
int osdp_cp_flush_commands(osdp_t *ctx, int pd);

/**
 * @brief Get the SCBK 
 *
 * @param ctx OSDP context
 * @param pd PD offset (0-indexed) of this PD in `osdp_pd_info_t *` passed to
 * osdp_cp_setup()
 * @param scbk A pointer to the scbk
 *
 * @retval 0 on success
 * @retval -1 on failure
 */
OSDP_EXPORT
int osdp_cp_get_pd_scbk(const osdp_t *ctx, int pd, uint8_t *scbk);

/**
 * @brief Get PD ID information as reported by the PD. Calling this method
 * before the CP has had a the chance to get this information will return
 * invalid/stale results.
 *
 * @param ctx OSDP context
 * @param pd PD offset (0-indexed) of this PD in `osdp_pd_info_t *` passed to
 * osdp_cp_setup()
 * @param id A pointer to struct osdp_pd_id that will be filled with the
 * PD ID information that the PD last returned.
 *
 * @retval 0 on success
 * @retval -1 on failure
 */
OSDP_EXPORT
int osdp_cp_get_pd_id(const osdp_t *ctx, int pd, struct osdp_pd_id *id);

/**
 * @brief Get capability associated to a function_code that the PD reports in
 * response to osdp_CAP(0x62) command. Calling this method before the CP has
 * had a the chance to get this information will return invalid/stale results.
 *
 * @param ctx OSDP context
 * @param pd PD offset (0-indexed) of this PD in `osdp_pd_info_t *` passed to
 * osdp_cp_setup()
 * @param cap in/out; struct osdp_pd_cap pointer with osdp_pd_cap::function_code
 * set to the function code to get data for.
 *
 * @retval 0 on success
 * @retval -1 on failure
 */
OSDP_EXPORT
int osdp_cp_get_capability(const osdp_t *ctx, int pd, struct osdp_pd_cap *cap);

/**
 * @brief Set callback method for CP event notification. This callback is
 * invoked when the CP receives an event from the PD.
 *
 * @param ctx OSDP context
 * @param cb The callback function's pointer
 * @param arg A pointer that will be passed as the first argument of `cb`
 */
OSDP_EXPORT
void osdp_cp_set_event_callback(osdp_t *ctx, cp_event_callback_t cb, void *arg);

/**
 * @brief Set or clear OSDP public flags
 *
 * @param ctx OSDP context
 * @param pd PD offset (0-indexed) of this PD in `osdp_pd_info_t *` passed to
 * osdp_cp_setup()
 * @param flags One or more of the public flags (OSDP_FLAG_XXX) exported from
 * osdp.h. Any other bits will cause this method to fail.
 * @param do_set when true: set `flags` in ctx; when false: clear `flags` in ctx
 *
 * @retval 0 on success
 * @retval -1 on failure
 *
 * @note It doesn't make sense to call some initialization time flags during
 * runtime. This method is for dynamic flags that can be turned on/off at runtime.
 */
OSDP_EXPORT
int osdp_cp_modify_flag(osdp_t *ctx, int pd, uint32_t flags, bool do_set);

/* ------------------------------- */
/*          Common Methods         */
/* ------------------------------- */

/**
 * @brief Different levels of log messages; based on importance of the message
 * with LOG_EMERG being most critical to LOG_DEBUG being the least.
 */
enum osdp_log_level_e {
	OSDP_LOG_EMERG,     /**< Log level Emergency */
	OSDP_LOG_ALERT,     /**< Log level Alert */
	OSDP_LOG_CRIT,      /**< Log level Critical */
	OSDP_LOG_ERROR,     /**< Log level Error */
	OSDP_LOG_WARNING,   /**< Log level Warning */
	OSDP_LOG_NOTICE,    /**< Log level Notice */
	OSDP_LOG_INFO,      /**< Log level Info */
	OSDP_LOG_DEBUG,     /**< Log level Debug */
	OSDP_LOG_MAX_LEVEL  /**< Log level max value */
};

/**
 * @brief Puts a string to the logging medium
 *
 * @param msg a null-terminated char buffer.
 *
 * @retval 0 on success; -ve on errors
 */
typedef int (*osdp_log_puts_fn_t)(const char *msg);

/**
 * @brief A callback function to be used with external loggers
 *
 * @param log_level A syslog style log level. See `enum osdp_log_level_e`
 * @param file Relative path to file which produced the log message
 * @param line Line number in `file` which produced the log message
 * @param msg The log message
 */
typedef void (*osdp_log_callback_fn_t)(int log_level, const char *file,
				       unsigned long line, const char *msg);

/**
 * @brief Configure OSDP Logging.
 *
 * @param name A soft name for this module; will appear in all the log lines.
 * @param log_level OSDP log levels of type `enum osdp_log_level_e`. Default is
 * LOG_INFO.
 * @param puts_fn A puts() like function that will be invoked to write the log
 * buffer. Can be handy if you want to log to file on a UART device without
 * putchar redirection. See `osdp_log_puts_fn_t` definition to see the
 * behavioral expectations. When this is set to NULL, LibOSDP will log to
 * stderr.
 *
 * Note: This function has to be called before osdp_{cp,pd}_setup(). Otherwise
 *       it will be ignored.
 */
OSDP_EXPORT
void osdp_logger_init(const char *name, int log_level,
		      osdp_log_puts_fn_t puts_fn);

/**
 * @brief A callback function that gets called when LibOSDP wants to emit a log
 * line. All messages (of all log levels) are passed on to this callback
 * without any log formatting. This API is for users who may already have a
 * logger configured in their application.
 *
 * @param cb The callback function. See `osdp_log_callback_fn_t` for more
 * details.
 *
 * @note This function has to be called before osdp_{cp,pd}_setup(). Otherwise
 * it will be ignored.
 */
OSDP_EXPORT
void osdp_set_log_callback(osdp_log_callback_fn_t cb);

/**
 * @brief Get LibOSDP version as a `const char *`. Used in diagnostics.
 *
 * @retval version string
 */
OSDP_EXPORT
const char *osdp_get_version();

/**
 * @brief Get LibOSDP source identifier as a `const char *`. This string has
 * info about the source tree from which this version of LibOSDP was built.
 * Used in diagnostics.
 *
 * @retval source identifier string
 */
OSDP_EXPORT
const char *osdp_get_source_info();

/**
 * @brief Get a bit mask of number of PD that are online currently.
 *
 * @param ctx OSDP context
 * @param bitmask pointer to an array of bytes. must be as large as
 * (num_pds + 7 / 8).
 */
OSDP_EXPORT
void osdp_get_status_mask(const osdp_t *ctx, uint8_t *bitmask);

/**
 * @brief Get a bit mask of number of PD that are online and have an active
 * secure channel currently.
 *
 * @param ctx OSDP context
 * @param bitmask pointer to an array of bytes. must be as large as
 * (num_pds + 7 / 8).
 */
OSDP_EXPORT
void osdp_get_sc_status_mask(const osdp_t *ctx, uint8_t *bitmask);

/**
 * @brief Open a pre-agreed file
 *
 * @param arg Opaque pointer that was provided in @ref osdp_file_ops when the
 * ops struct was registered.
 * @param file_id File ID of pre-agreed file between this CP and PD
 * @param size Size of the file that was opened (filled by application)
 *
 * @retval 0 on success
 * @retval -1 on errors
 */
typedef int (*osdp_file_open_fn_t)(void *arg, int file_id, int *size);

/**
 * @brief Read a chunk of file data into buffer
 *
 * @param arg Opaque pointer that was provided in @ref osdp_file_ops when the
 * ops struct was registered.
 * @param buf Buffer to store file data read
 * @param size Number of bytes to read from file into buffer
 * @param offset Number of bytes from the beginning of the file to
 * start reading from.
 *
 * @retval Number of bytes read
 * @retval 0 on EOF
 * @retval -ve on errors.
 *
 * @note LibOSDP will guarantee that size and offset params are always
 * positive and size is always greater than or equal to offset.
 */
typedef int (*osdp_file_read_fn_t)(void *arg, void *buf, int size, int offset);

/**
 * @brief Write a chunk of file data from buffer to disk.
 *
 * @param arg Opaque pointer that was provided in @ref osdp_file_ops when the
 * ops struct was registered.
 * @param buf Buffer with file data to be stored to disk
 * @param size Number of bytes to write to disk
 * @param offset Number of bytes from the beginning of the file to
 * start writing too.
 *
 * @retval Number of bytes written
 * @retval 0 on EOF
 * @retval -ve on errors.
 *
 * @note LibOSDP will guarantee that size and offset params are always
 * positive and size is always greater than or equal to offset.
 */
typedef int (*osdp_file_write_fn_t)(void *arg, const void *buf,
				   int size, int offset);

/**
 * @brief Close file that corresponds to a given file descriptor
 *
 * @param arg Opaque pointer that was provided in @ref osdp_file_ops when the
 * ops struct was registered.
 *
 * @retval 0 on success
 * @retval -1 on errors.
 */
typedef int (*osdp_file_close_fn_t)(void *arg);

/**
 * @brief OSDP File operations struct that needs to be filled by the CP/PD
 * application and registered with LibOSDP using osdp_file_register_ops()
 * before a file transfer command can be initiated.
 */
struct osdp_file_ops {
	/**
	 * @brief A opaque pointer to private data that can be filled by the
	 * application which will be passed as the first argument for each of
	 * the below functions. Applications can keep their file context info
	 * such as the open file descriptors or any other private data here.
	 */
	void *arg;
	osdp_file_open_fn_t open;   /**< open handler function */
	osdp_file_read_fn_t read;   /**< read handler function */
	osdp_file_write_fn_t write; /**< write handler function */
	osdp_file_close_fn_t close; /**< close handler function */
};

/**
 * @brief Register a global file operations struct with OSDP. Both CP and PD
 * modes should have done so already before CP can sending a OSDP_CMD_FILE_TX.
 *
 * @param ctx OSDP context
 * @param pd PD number in case of CP. This param is ignored in PD mode
 * @param ops Populated file operations struct
 *
 * @retval 0 on success. -1 on errors.
 */
OSDP_EXPORT
int osdp_file_register_ops(osdp_t *ctx, int pd,
			   const struct osdp_file_ops *ops);

/**
 * @brief Query file transfer status if one is in progress. Calling this method
 * when there is no file transfer progressing will return error.
 *
 * @param ctx OSDP context
 * @param pd PD number in case of CP. This param is ignored in PD mode
 * @param size Total size of the file (as obtained from file_ops->open())
 * @param offset Offset into the file that has been sent/received (CP/PD)
 * @retval 0 on success. -1 on errors.
 */
OSDP_EXPORT
int osdp_get_file_tx_status(const osdp_t *ctx, int pd, int *size, int *offset);

#ifdef __cplusplus
}
#endif

#endif	/* _OSDP_H_ */
