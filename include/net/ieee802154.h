/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file
 * @brief IEEE 802.15.4 L2 stack public header
 */

#ifndef __IEEE802154_H__
#define __IEEE802154_H__

#include <net/net_mgmt.h>

#define IEEE802154_MAX_ADDR_LENGTH	8

/* This not meant to be used by any code but 802.15.4 L2 stack */
struct ieee802154_context {
	uint16_t pan_id;
	uint16_t channel;
	struct k_sem ack_lock;
#ifdef CONFIG_NET_L2_IEEE802154_MGMT
	struct ieee802154_req_params *scan_ctx;
	union {
		struct k_sem res_lock;
		struct k_sem req_lock;
	};
	union {
		uint8_t ext_addr[IEEE802154_MAX_ADDR_LENGTH];
		uint16_t short_addr;
	} coord;
	uint8_t coord_addr_len;
#endif
	uint8_t sequence;
	uint8_t ack_received	: 1;
	uint8_t ack_requested	: 1;
	uint8_t associated	: 1;
	uint8_t _unused		: 5;
} __packed;


/* Management part definitions */

#define _NET_IEEE802154_LAYER	NET_MGMT_LAYER_L2
#define _NET_IEEE802154_CODE	0x154
#define _NET_IEEE802154_BASE	(NET_MGMT_IFACE_BIT |			\
				 NET_MGMT_LAYER(_NET_IEEE802154_LAYER) |\
				 NET_MGMT_LAYER_CODE(_NET_IEEE802154_CODE))
#define _NET_IEEE802154_EVENT	(_NET_IEEE802154_BASE | NET_MGMT_EVENT_BIT)

enum net_request_ieee802154_cmd {
	NET_REQUEST_IEEE802154_CMD_SET_ACK = 1,
	NET_REQUEST_IEEE802154_CMD_UNSET_ACK,
	NET_REQUEST_IEEE802154_CMD_PASSIVE_SCAN,
	NET_REQUEST_IEEE802154_CMD_ACTIVE_SCAN,
	NET_REQUEST_IEEE802154_CMD_CANCEL_SCAN,
	NET_REQUEST_IEEE802154_CMD_ASSOCIATE,
	NET_REQUEST_IEEE802154_CMD_DISASSOCIATE,
};


#define NET_REQUEST_IEEE802154_SET_ACK					\
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_SET_ACK)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_SET_ACK);

#define NET_REQUEST_IEEE802154_UNSET_ACK				\
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_UNSET_ACK)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_UNSET_ACK);

#define NET_REQUEST_IEEE802154_PASSIVE_SCAN				\
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_PASSIVE_SCAN)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_PASSIVE_SCAN);

#define NET_REQUEST_IEEE802154_ACTIVE_SCAN				\
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_ACTIVE_SCAN)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_ACTIVE_SCAN);

#define NET_REQUEST_IEEE802154_CANCEL_SCAN				\
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_CANCEL_SCAN)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_CANCEL_SCAN);

#define NET_REQUEST_IEEE802154_ASSOCIATE				\
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_ASSOCIATE)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_ASSOCIATE);

#define NET_REQUEST_IEEE802154_DISASSOCIATE				\
	(_NET_IEEE802154_BASE | NET_REQUEST_IEEE802154_CMD_DISASSOCIATE)

NET_MGMT_DEFINE_REQUEST_HANDLER(NET_REQUEST_IEEE802154_DISASSOCIATE);


enum net_event_ieee802154_cmd {
	NET_EVENT_IEEE802154_CMD_SCAN_RESULT = 1,
};


#define NET_EVENT_IEEE802154_SCAN_RESULT				\
	(_NET_IEEE802154_EVENT | NET_EVENT_IEEE802154_CMD_SCAN_RESULT)


#define IEEE802154_IS_CHAN_SCANNED(_channel_set, _chan)	\
	(_channel_set & BIT(_chan - 1))
#define IEEE802154_IS_CHAN_UNSCANNED(_channel_set, _chan)	\
	(!IEEE802154_IS_CHAN_SCANNED(_channel_set, _chan))

/* Useful define to request all channels to be scanned,
 * from 11 to 26 included.
 */
#define IEEE802154_ALL_CHANNELS	(0x03FFFC00)

/**
 * @brief Scanning parameters
 *
 * Used to request a scan and get results as well
 */
struct ieee802154_req_params {
	/** The set of channels to scan, use above macros to manage it */
	uint32_t channel_set;

	/** Duration of scan, per-channel, in milliseconds */
	uint32_t duration;

	/** Current channel in use as a result */
	uint16_t channel;
	/** Current pan_id in use as a result */
	uint16_t pan_id;

	/** Result address */
	union {
		uint8_t addr[IEEE802154_MAX_ADDR_LENGTH];
		uint16_t short_addr;
	};

	/** length of address */
	uint8_t len;
	/** Link quality information, between 0 and 255 */
	uint8_t lqi;
} __packed;

#endif /* __IEEE802154_H__ */
